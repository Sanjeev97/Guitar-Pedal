#!/bin/bash
# Quick start script for Guitar Pedal web interface

set -e  # Exit on error

echo "🎸 Guitar Pedal Server Setup"
echo "=============================="
echo ""

# Check if convert binary exists
if [ ! -f "./convert" ]; then
    echo "❌ Error: convert binary not found"
    echo "Run 'make' first to build the audio processor"
    exit 1
fi

# Check if ffmpeg is installed
if ! command -v ffmpeg &> /dev/null; then
    echo "❌ Error: ffmpeg not found"
    echo "Install it with: brew install ffmpeg (macOS) or sudo apt install ffmpeg (Linux)"
    exit 1
fi

# Check if Python 3 is installed
if ! command -v python3 &> /dev/null; then
    echo "❌ Error: python3 not found"
    echo "Please install Python 3.7 or higher"
    exit 1
fi

echo "✅ Prerequisites checked"
echo ""

# Check if virtual environment exists
if [ ! -d "venv" ]; then
    echo "📦 Creating virtual environment..."
    python3 -m venv venv
    echo "✅ Virtual environment created"
else
    echo "✅ Virtual environment found"
fi

echo ""
echo "📦 Installing Python dependencies..."

# Activate virtual environment and install dependencies
source venv/bin/activate

# Install/upgrade pip
pip install --upgrade pip -q

# Install requirements
pip install -r requirements.txt -q

echo "✅ Dependencies installed"
echo ""

# Create necessary directories
mkdir -p uploads outputs

echo "🚀 Starting Guitar Pedal server..."
echo ""
echo "================================================"
echo "   Open your browser and visit:"
echo "   👉 http://localhost:5000"
echo "================================================"
echo ""
echo "Press Ctrl+C to stop the server"
echo ""

# Run the Flask app
python3 app.py

