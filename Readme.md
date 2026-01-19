# Digital Pedal Board: Simple Audio Effects Processor

A lightweight C audio effects processor for adding echo/delay effects to MP3 files.

## 🎨 NEW: Web Interface Available!

**Want an easier way to use this?** We now have a beautiful web interface with drag-and-drop file upload, visual knobs, and presets!

### ⚡ ONE-COMMAND Setup (Recommended!)

```bash
./scripts/setup_and_run.sh
```

This automatically:
- ✅ Builds everything
- ✅ Installs dependencies
- ✅ Starts the server
- ✅ **Opens your browser!**

📖 See **[docs/SETUP_INSTRUCTIONS.md](docs/SETUP_INSTRUCTIONS.md)** for details.

### Alternative: Manual Start

If already set up:
```bash
./scripts/start_server.sh
```

📖 See **[docs/FRONTEND_SETUP.md](docs/FRONTEND_SETUP.md)** for detailed frontend documentation.

---

## 📁 **NEW: Organized Repository Structure!**

The repository is now cleanly organized:
- `src/` - C audio processor source code
- `web/` - Flask web application
- `scripts/` - Setup and utility scripts
- `docs/` - All documentation
- `examples/` - Sample audio files

📖 See **[DIRECTORY_STRUCTURE.md](DIRECTORY_STRUCTURE.md)** for complete details.

---

## Requirements

- GCC or Clang compiler
- **ffmpeg** (for MP3 conversion)
- **Python 3.7+** (for web interface only)

### Installing Dependencies

**macOS (Homebrew):**
```bash
brew install ffmpeg
```

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install ffmpeg gcc
```

## Quick Start

### Step 1: Build the program

```bash
make
```

### Step 2: Convert MP3 to raw audio

```bash
ffmpeg -v fatal -i input.mp3 -f s32le -ar 48000 -ac 1 input.raw
```

### Step 3: Apply the echo effect

```bash
./convert echo 0.3 0 0 0.5 < input.raw > output.raw
```

### Step 4: Convert raw audio back to MP3

```bash
ffmpeg -v fatal -f s32le -ar 48000 -ac 1 -i output.raw output.mp3
```

### Step 5: Clean up temporary files (optional)

```bash
rm input.raw output.raw
```

Your processed MP3 is saved as `output.mp3`!

## Echo Effect Parameters

| Parameter | Range | Description |
|-----------|-------|-------------|
| pot1 | 0.0-1.0 | Delay time (0-1000ms) |
| pot2 | 0.0-1.0 | (unused) |
| pot3 | 0.0-1.0 | LFO modulation depth (0-4ms) |
| pot4 | 0.0-1.0 | Feedback amount (0-100%) |

## Examples

### Basic Echo (300ms delay, 50% feedback)

```bash
# Step 1: Convert to raw
ffmpeg -v fatal -i song.mp3 -f s32le -ar 48000 -ac 1 song.raw

# Step 2: Apply effect
./convert echo 0.3 0 0 0.5 < song.raw > song_processed.raw

# Step 3: Convert back to MP3
ffmpeg -v fatal -f s32le -ar 48000 -ac 1 -i song_processed.raw song_echo.mp3

# Step 4: Clean up
rm song.raw song_processed.raw
```

### Short Slapback Delay (100ms, no feedback)

Great for vocals and guitars:

```bash
# Step 1: Convert to raw
ffmpeg -v fatal -i vocals.mp3 -f s32le -ar 48000 -ac 1 vocals.raw

# Step 2: Apply effect
./convert echo 0.1 0 0 0 < vocals.raw > vocals_processed.raw

# Step 3: Convert back to MP3
ffmpeg -v fatal -f s32le -ar 48000 -ac 1 -i vocals_processed.raw vocals_slapback.mp3

# Step 4: Clean up
rm vocals.raw vocals_processed.raw
```

### Long Ambient Echo (500ms, 70% feedback)

Creates a spacey, reverberant sound:

```bash
# Step 1: Convert to raw
ffmpeg -v fatal -i ambient.mp3 -f s32le -ar 48000 -ac 1 ambient.raw

# Step 2: Apply effect
./convert echo 0.5 0 0.2 0.7 < ambient.raw > ambient_processed.raw

# Step 3: Convert back to MP3
ffmpeg -v fatal -f s32le -ar 48000 -ac 1 -i ambient_processed.raw ambient_long_echo.mp3

# Step 4: Clean up
rm ambient.raw ambient_processed.raw
```

## Output Quality Options

When converting back to MP3, you can adjust quality:

### Higher Quality MP3 (320 kbps)

```bash
ffmpeg -v fatal -f s32le -ar 48000 -ac 1 -i output.raw -b:a 320k output.mp3
```

### Variable Bitrate (VBR) for Best Quality

```bash
ffmpeg -v fatal -f s32le -ar 48000 -ac 1 -i output.raw -q:a 0 output.mp3
```

## Project Structure

```
Guitar-Pedal/
├── 📄 Readme.md                  # You are here
├── 📄 Makefile                   # Build system
├── 📄 DIRECTORY_STRUCTURE.md     # Complete structure guide
│
├── 📁 src/                       # C Audio Processor
│   ├── convert.c                 # Main processing program
│   ├── echo.h                    # Echo effect
│   ├── effects.h                 # Effect framework
│   ├── lfo.h                     # Oscillator
│   └── util.h                    # Utilities
│
├── 📁 web/                       # Web Frontend
│   ├── app.py                    # Flask server
│   ├── requirements.txt          # Dependencies
│   ├── templates/index.html      # UI
│   └── static/                   # CSS & JS
│
├── 📁 scripts/                   # Automation
│   ├── setup_and_run.sh          # One-command setup
│   └── start_server.sh           # Quick start
│
├── 📁 docs/                      # Documentation
│   ├── QUICK_START.md
│   ├── SETUP_INSTRUCTIONS.md
│   ├── TROUBLESHOOTING.md
│   └── (more...)
│
└── 📁 examples/                  # Sample audio files
```

📖 **Full structure**: See [DIRECTORY_STRUCTURE.md](DIRECTORY_STRUCTURE.md)

## Building

### Build the program

```bash
make
```

### Clean up (remove executable)

```bash
make clean
```

### Rebuild from scratch

```bash
make clean
make
```

## Troubleshooting

### "Permission denied" when running ./convert

```bash
chmod +x convert
```

### ffmpeg not found

Make sure ffmpeg is installed and in your PATH:

```bash
which ffmpeg
```

### No sound in output

Check that your input MP3 file is valid:

```bash
ffmpeg -i input.mp3
```

## Technical Notes

- Sample rate: 48000 Hz (standard high-quality audio)
- Maximum delay: ~1.36 seconds (65536 samples)
- The soft limiter prevents clipping when feedback accumulates
- Stereo MP3s are converted to mono for processing

## License

See original repository for license information.

## Credits

Based on Linus Torvalds' AudioNoise toolkit.
