# 🎸 Guitar Pedal - One-Command Setup

## 🚀 Quickest Way to Get Started

### **Run this ONE command:**

```bash
./scripts/setup_and_run.sh
```

That's it! The script will:
1. ✅ Check all prerequisites (Python, ffmpeg, gcc)
2. ✅ Build the C audio processor
3. ✅ Create virtual environment
4. ✅ Install all dependencies
5. ✅ Start the web server
6. ✅ **Open your browser automatically!**

---

## 📋 What Happens When You Run It

```
╔════════════════════════════════════════════════════════════╗
║               🎸  GUITAR PEDAL SETUP  🎸                  ║
║          Digital Audio Effects Processor                   ║
╚════════════════════════════════════════════════════════════╝

[INFO] Step 1/6: Checking prerequisites...
[✓] Python 3 found: 3.13.3
[✓] ffmpeg found: 8.0.1
[✓] GCC compiler found

[INFO] Step 2/6: Building C audio processor...
[✓] C audio processor built successfully

[INFO] Step 3/6: Setting up Python virtual environment...
[✓] Virtual environment created

[INFO] Step 4/6: Installing Python dependencies...
[✓] All dependencies installed

[INFO] Step 5/6: Setting up directories...
[✓] Created uploads/ directory
[✓] Created outputs/ directory

[INFO] Step 6/6: Starting Flask server...

╔════════════════════════════════════════════════════════════╗
║                    ✅  SETUP COMPLETE  ✅                 ║
╚════════════════════════════════════════════════════════════╝

🌐  Opening web browser...
📡  Server running at: http://localhost:5000
```

Your browser will open automatically!

---

## 🎯 First Time Setup vs. Subsequent Runs

### **First Time** (takes ~30 seconds)
- Builds C binary
- Creates virtual environment
- Installs Python packages
- Opens browser

### **Next Times** (takes ~5 seconds)
- Uses existing virtual environment
- Skips dependency installation
- Just starts server and opens browser

The script is **smart** - it detects what's already set up!

---

## 🛠️ Prerequisites (Auto-Checked)

The script will check for:
- **Python 3.7+** (if missing, tells you how to install)
- **ffmpeg** (if missing, provides install commands)
- **GCC or Clang** (for compiling C code)

### If Prerequisites Are Missing:

**macOS:**
```bash
brew install python3 ffmpeg
```

**Ubuntu/Linux:**
```bash
sudo apt update
sudo apt install python3 ffmpeg gcc
```

---

## 🔧 Troubleshooting

### Port 5000 Already In Use
The script automatically detects and frees port 5000.

### Browser Doesn't Open
If your browser doesn't open automatically:
```
Manually visit: http://localhost:5000
```

### Permission Denied
If you get "Permission denied":
```bash
chmod +x setup_and_run.sh
```

### Script Fails to Start
Try running with verbose output:
```bash
bash -x setup_and_run.sh
```

---

## ⚙️ What the Script Does (Technical)

### 1. Prerequisites Check
```bash
✓ Checks Python 3 (python3 --version)
✓ Checks ffmpeg (ffmpeg -version)
✓ Checks C compiler (gcc/clang)
```

### 2. Build C Processor
```bash
make clean
make
chmod +x convert
```

### 3. Virtual Environment
```bash
python3 -m venv venv
source venv/bin/activate
pip install --upgrade pip
```

### 4. Install Dependencies
```bash
pip install -r requirements.txt
```

### 5. Create Directories
```bash
mkdir -p uploads outputs
```

### 6. Launch Server
```bash
python3 app.py          # Start Flask
open http://localhost:5000  # Open browser
```

---

## 🎮 Using the Web Interface

Once the browser opens:

### 1. **Upload Audio**
- Drag & drop your MP3/WAV file
- Or click to browse

### 2. **Adjust Effects**
Choose a preset:
- **Slapback** - Quick echo (vocals)
- **Standard** - Balanced delay
- **Ambient** - Long spacey echo
- **Chorus** - Thick shimmer
- **Tape** - Vintage warmth

Or manually adjust:
- **Delay Time**: 0-1000ms
- **LFO Mod**: 0-4ms (adds wobble)
- **Feedback**: 0-100% (echo repeats)

### 3. **Process**
Click "Process Audio" and wait ~5-15 seconds

### 4. **Compare**
Listen to Original vs. Processed side-by-side

### 5. **Download**
Click "Download Processed Audio" to save

---

## 🛑 Stopping the Server

Press **`Ctrl+C`** in the terminal where the server is running.

The script will clean up automatically.

---

## 🔄 Running Again Later

Just run the same command:
```bash
./setup_and_run.sh
```

It will:
- Skip building if already built
- Use existing virtual environment
- Skip installing if already installed
- Just start the server and open browser

**Much faster the second time!**

---

## 📊 System Requirements

| Component | Requirement |
|-----------|-------------|
| **OS** | macOS, Linux, Windows (WSL) |
| **Python** | 3.7 or higher |
| **RAM** | 2GB minimum |
| **Disk** | 100MB for app + space for audio files |
| **Browser** | Any modern browser |

---

## 🎵 Example Workflow

```bash
# 1. Run setup (first time)
./setup_and_run.sh

# Browser opens automatically to http://localhost:5000

# 2. Upload your guitar recording

# 3. Select "Slapback" preset

# 4. Click "Process Audio"

# 5. Listen to the result

# 6. Download your processed audio

# 7. Stop server (Ctrl+C)

# 8. Next day, run again
./setup_and_run.sh    # Much faster!
```

---

## 🌟 Features of This Setup Script

- ✨ **One Command**: Everything automated
- 🔍 **Smart Detection**: Skips already-completed steps
- 🎨 **Colorful Output**: Easy to read progress
- 🛡️ **Error Handling**: Exits with helpful messages
- 🌐 **Auto Browser**: Opens automatically
- 🔄 **Port Management**: Frees port 5000 if needed
- 📦 **Isolated**: Uses virtual environment (no global pollution)
- 🧹 **Clean**: Cleans up on exit

---

## 📚 Alternative Setup Methods

### Manual Setup (if you prefer)
See: **FRONTEND_SETUP.md**

### Quick Start Script (without auto-browser)
```bash
./scripts/start_server.sh
```

### Command Line Only
See: **Readme.md** for C program usage

**Note**: With the new organized structure:
- Scripts are in `scripts/` directory
- C source is in `src/` directory  
- Web app is in `web/` directory
- Documentation is in `docs/` directory

---

## 🎯 What's Different from start_server.sh?

| Feature | setup_and_run.sh | start_server.sh |
|---------|------------------|-----------------|
| Auto-build C binary | ✅ | ❌ |
| Check prerequisites | ✅ | ✅ |
| Setup venv | ✅ | ✅ |
| Install dependencies | ✅ | ✅ |
| **Auto-open browser** | ✅ | ❌ |
| Colorful output | ✅ | ❌ |
| Port management | ✅ | ❌ |
| **Complete automation** | ✅ | Partial |

**Recommendation**: Use `setup_and_run.sh` for the best experience!

---

## 💡 Tips

1. **First Time**: May take 30-60 seconds (installing packages)
2. **Subsequent Runs**: Only takes 5 seconds
3. **Keep Terminal Open**: Server runs in foreground
4. **Multiple Files**: Upload and process as many as you want
5. **Experiment**: Try different presets and settings

---

## 🐛 Known Issues

### Browser Opens But Shows Error
Wait a few seconds - server needs time to start

### Script Hangs
Press Ctrl+C and run again

### Virtual Environment Issues
Delete `venv/` folder and run again:
```bash
rm -rf venv
./setup_and_run.sh
```

---

## 🎸 Ready to Rock!

Your complete setup command:

```bash
./setup_and_run.sh
```

Then enjoy your Guitar Pedal web interface! 🎵

---

## 📞 Need Help?

- **Quick Start**: This file (you're reading it!)
- **Detailed Setup**: FRONTEND_SETUP.md
- **Testing Guide**: TESTING.md
- **Architecture**: ARCHITECTURE.md
- **Main Docs**: Readme.md

---

**Made with ❤️ for musicians and audio enthusiasts**

