#!/usr/bin/env python3
"""
Guitar Pedal Web Frontend
A Flask web application for the audio effects processor
"""

from flask import Flask, render_template, request, jsonify, send_file
import subprocess
import os
import uuid
from pathlib import Path
import shutil

app = Flask(__name__)

# Configuration
app.config['SECRET_KEY'] = 'guitar-pedal-secret-key-change-in-production'
app.config['MAX_CONTENT_LENGTH'] = 100 * 1024 * 1024  # 100MB max file size
app.config['UPLOAD_FOLDER'] = 'uploads'
app.config['OUTPUT_FOLDER'] = 'outputs'

UPLOAD_FOLDER = Path('uploads')
OUTPUT_FOLDER = Path('outputs')
ALLOWED_EXTENSIONS = {'mp3', 'wav', 'ogg', 'm4a'}

# Create necessary directories
UPLOAD_FOLDER.mkdir(exist_ok=True)
OUTPUT_FOLDER.mkdir(exist_ok=True)

def allowed_file(filename):
    """Check if file extension is allowed"""
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

def convert_to_raw(input_file, output_file):
    """Convert audio file to raw format using ffmpeg"""
    cmd = [
        'ffmpeg', '-v', 'fatal', '-i', str(input_file),
        '-f', 's32le', '-ar', '48000', '-ac', '1',
        '-y',  # Overwrite output file
        str(output_file)
    ]
    subprocess.run(cmd, check=True)

def apply_effect(input_raw, output_raw, effect, pot1, pot2, pot3, pot4):
    """Apply audio effect using the C converter program"""
    # Path to convert binary (in parent directory)
    convert_path = Path(__file__).parent.parent / 'convert'
    cmd = [
        str(convert_path), effect,
        str(pot1), str(pot2), str(pot3), str(pot4)
    ]
    
    with open(input_raw, 'rb') as infile, open(output_raw, 'wb') as outfile:
        subprocess.run(cmd, stdin=infile, stdout=outfile, check=True)

def convert_to_mp3(input_raw, output_file, bitrate='320k'):
    """Convert raw audio back to MP3 with high quality"""
    cmd = [
        'ffmpeg', '-v', 'fatal',
        '-f', 's32le', '-ar', '48000', '-ac', '1',
        '-i', str(input_raw),
        '-b:a', bitrate,
        '-y',  # Overwrite output file
        str(output_file)
    ]
    subprocess.run(cmd, check=True)

@app.route('/')
def index():
    """Serve the main page"""
    return render_template('index.html')

@app.route('/upload', methods=['POST'])
def upload_file():
    """Handle file upload and processing"""
    try:
        # Check if file was uploaded
        if 'file' not in request.files:
            return jsonify({'error': 'No file uploaded'}), 400
        
        file = request.files['file']
        
        if file.filename == '':
            return jsonify({'error': 'No file selected'}), 400
        
        if not allowed_file(file.filename):
            return jsonify({'error': f'Invalid file type. Allowed: {", ".join(ALLOWED_EXTENSIONS)}'}), 400
        
        # Generate unique session ID
        session_id = str(uuid.uuid4())
        
        # Save uploaded file
        original_ext = file.filename.rsplit('.', 1)[1].lower()
        input_file = UPLOAD_FOLDER / f'{session_id}_input.{original_ext}'
        file.save(input_file)
        
        # Get effect parameters
        effect = request.form.get('effect', 'echo')
        pot1 = float(request.form.get('pot1', 0.3))
        pot2 = float(request.form.get('pot2', 0.0))
        pot3 = float(request.form.get('pot3', 0.0))
        pot4 = float(request.form.get('pot4', 0.5))
        
        # Validate parameters (0.0 to 1.0)
        for pot in [pot1, pot2, pot3, pot4]:
            if not (0.0 <= pot <= 1.0):
                return jsonify({'error': 'Parameters must be between 0.0 and 1.0'}), 400
        
        # Process audio
        input_raw = UPLOAD_FOLDER / f'{session_id}_input.raw'
        output_raw = OUTPUT_FOLDER / f'{session_id}_output.raw'
        output_file = OUTPUT_FOLDER / f'{session_id}_output.mp3'
        
        # Convert input to raw
        convert_to_raw(input_file, input_raw)
        
        # Apply effect
        apply_effect(input_raw, output_raw, effect, pot1, pot2, pot3, pot4)
        
        # Convert output to MP3
        convert_to_mp3(output_raw, output_file)
        
        # Clean up raw files
        input_raw.unlink()
        output_raw.unlink()
        
        return jsonify({
            'success': True,
            'session_id': session_id,
            'original_file': str(input_file.name),
            'output_file': str(output_file.name),
            'download_url': f'/download/{session_id}'
        })
    
    except subprocess.CalledProcessError as e:
        return jsonify({'error': f'Processing error: {str(e)}'}), 500
    except Exception as e:
        return jsonify({'error': f'Server error: {str(e)}'}), 500

@app.route('/download/<session_id>')
def download_file(session_id):
    """Download processed audio file"""
    try:
        output_file = OUTPUT_FOLDER / f'{session_id}_output.mp3'
        
        if not output_file.exists():
            return jsonify({'error': 'File not found'}), 404
        
        return send_file(
            output_file,
            mimetype='audio/mpeg',
            as_attachment=True,
            download_name='processed_audio.mp3'
        )
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/play/<session_id>/<file_type>')
def play_file(session_id, file_type):
    """Stream audio for in-browser playback"""
    try:
        if file_type == 'original':
            # Find the original file (could be mp3, wav, etc.)
            for ext in ALLOWED_EXTENSIONS:
                file_path = UPLOAD_FOLDER / f'{session_id}_input.{ext}'
                if file_path.exists():
                    return send_file(file_path, mimetype=f'audio/{ext}')
            return jsonify({'error': 'Original file not found'}), 404
        
        elif file_type == 'processed':
            file_path = OUTPUT_FOLDER / f'{session_id}_output.mp3'
            if not file_path.exists():
                return jsonify({'error': 'Processed file not found'}), 404
            return send_file(file_path, mimetype='audio/mpeg')
        
        else:
            return jsonify({'error': 'Invalid file type'}), 400
    
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/cleanup/<session_id>', methods=['POST'])
def cleanup(session_id):
    """Clean up files for a session"""
    try:
        # Remove uploaded files
        for ext in ALLOWED_EXTENSIONS:
            input_file = UPLOAD_FOLDER / f'{session_id}_input.{ext}'
            if input_file.exists():
                input_file.unlink()
        
        # Remove output files
        output_file = OUTPUT_FOLDER / f'{session_id}_output.mp3'
        if output_file.exists():
            output_file.unlink()
        
        return jsonify({'success': True})
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/presets')
def get_presets():
    """Return available effect presets"""
    presets = {
        'slapback': {
            'name': 'Slapback Delay',
            'description': 'Classic rockabilly delay',
            'pot1': 0.1, 'pot2': 0.0, 'pot3': 0.0, 'pot4': 0.0
        },
        'standard': {
            'name': 'Standard Echo',
            'description': 'Balanced delay with repeats',
            'pot1': 0.3, 'pot2': 0.0, 'pot3': 0.0, 'pot4': 0.5
        },
        'ambient': {
            'name': 'Ambient Echo',
            'description': 'Long spacey delay',
            'pot1': 0.5, 'pot2': 0.0, 'pot3': 0.2, 'pot4': 0.7
        },
        'chorus': {
            'name': 'Chorus Effect',
            'description': 'Short delay with modulation',
            'pot1': 0.02, 'pot2': 0.0, 'pot3': 0.5, 'pot4': 0.3
        },
        'tape': {
            'name': 'Tape Echo',
            'description': 'Warm tape-style delay',
            'pot1': 0.25, 'pot2': 0.0, 'pot3': 0.1, 'pot4': 0.6
        }
    }
    return jsonify(presets)

if __name__ == '__main__':
    # Check if convert binary exists (in parent directory)
    convert_path = Path(__file__).parent.parent / 'convert'
    if not convert_path.exists():
        print("Error: 'convert' binary not found. Run 'make' from project root first!")
        exit(1)
    
    # Check if ffmpeg is installed
    try:
        subprocess.run(['ffmpeg', '-version'], capture_output=True, check=True)
    except (subprocess.CalledProcessError, FileNotFoundError):
        print("Error: ffmpeg not found. Please install ffmpeg!")
        exit(1)
    
    print("🎸 Guitar Pedal Server Starting...")
    print("📡 Access the app at:")
    print("   • http://localhost:5000")
    print("   • http://127.0.0.1:5000")
    print("")
    print("Press Ctrl+C to stop the server")
    print("")
    
    # Run with threaded=True for better concurrent handling
    app.run(debug=True, host='127.0.0.1', port=5000, threaded=True, use_reloader=False)

