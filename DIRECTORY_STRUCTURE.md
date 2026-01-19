# Guitar Pedal - Directory Structure

## 📁 Organized Project Layout

```
Guitar-Pedal/
├── 📄 Readme.md                    # Main project documentation
├── 📄 Makefile                     # Build system for C audio processor
├── 📄 .gitignore                   # Git ignore rules
├── 📄 DIRECTORY_STRUCTURE.md       # This file
│
├── 📁 src/                         # C Audio Processor Source Code
│   ├── convert.c                   # Main audio processing program
│   ├── echo.h                      # Echo/delay effect implementation
│   ├── effects.h                   # Shared effect state and utilities
│   ├── lfo.h                       # Low-frequency oscillator
│   ├── util.h                      # Math helpers and delay buffer
│   ├── gensin.c                    # Sine table generator
│   └── gensin.h                    # Pre-computed sine lookup table
│
├── 📁 web/                         # Web Frontend (Flask Application)
│   ├── app.py                      # Flask web server
│   ├── requirements.txt            # Python dependencies
│   ├── templates/                  # HTML templates
│   │   └── index.html              # Main web interface
│   └── static/                     # Static assets
│       ├── css/
│       │   └── style.css           # Stylesheet
│       └── js/
│           └── app.js              # Frontend JavaScript
│
├── 📁 scripts/                     # Setup and Utility Scripts
│   ├── setup_and_run.sh            # One-command setup and launch
│   └── start_server.sh             # Quick server start (if already set up)
│
├── 📁 docs/                        # Documentation
│   ├── ARCHITECTURE.md             # System architecture and design
│   ├── SETUP_FLOW.md               # Setup script flow diagram
│   ├── SETUP_INSTRUCTIONS.md       # Detailed setup instructions
│
├── 📁 examples/                    # Sample Audio Files
│   ├── BassForLinus.mp3            # Example bass audio
│   └── input.mp3                   # Sample input file
│
├── 📁 venv/                        # Python virtual environment (auto-generated)
│   ├── bin/                        # Python executables
│   ├── lib/                        # Installed packages
│   └── pyvenv.cfg                  # Environment configuration
│
├── 📁 uploads/                     # Temporary uploaded files (auto-generated)
│   └── (user uploaded audio files) # Cleaned up after processing
│
├── 📁 outputs/                     # Processed audio files (auto-generated)
│   └── (processed audio files)     # Can be cleaned up periodically
│
└── 🔧 convert                      # Compiled C binary (auto-generated)
```

---

## 📖 Directory Descriptions

### **Root Directory**
Contains only essential configuration and main documentation files.

| File | Purpose |
|------|---------|
| `Readme.md` | Main project overview and quick start |
| `Makefile` | Build automation for C audio processor |
| `.gitignore` | Files to exclude from version control |
| `DIRECTORY_STRUCTURE.md` | This documentation file |
| `convert` | Compiled audio processor binary (gitignored) |

---

### **📁 src/** - C Audio Processor Source

Contains all C source code for the audio processing engine.

| File | Description | Lines |
|------|-------------|-------|
| `convert.c` | Main program - reads audio, applies effects, writes output | ~100 |
| `echo.h` | Echo/delay effect with feedback control | ~40 |
| `effects.h` | Shared state (delay, feedback, LFO parameters) | ~35 |
| `lfo.h` | Low-frequency oscillator (sine/triangle/sawtooth) | ~100 |
| `util.h` | Delay buffer, soft limiter, math utilities | ~130 |
| `gensin.c` | Generates sine lookup table | ~35 |
| `gensin.h` | Pre-computed quarter-wave sine table | Large |

**Purpose**: High-performance audio DSP processing
**Language**: C (compiled with gcc/clang)
**Dependencies**: Standard C library, math library (-lm)

---

### **📁 web/** - Flask Web Application

Contains the Python/Flask web interface for easier UX.

| Component | Files | Purpose |
|-----------|-------|---------|
| **Backend** | `app.py` | Flask server, API routes, file handling |
| **Dependencies** | `requirements.txt` | Python packages (Flask, Werkzeug) |
| **Templates** | `templates/index.html` | Main web interface HTML |
| **Styles** | `static/css/style.css` | Modern dark theme styling |
| **Frontend Logic** | `static/js/app.js` | Interactive knobs, file upload, AJAX |

**Features**:
- Drag-and-drop file upload
- Visual knob controls
- Effect presets
- Side-by-side audio comparison
- Download processed files

**Tech Stack**: Python 3.7+, Flask 3.0, Vanilla JavaScript

---

### **📁 scripts/** - Automation Scripts

Bash scripts for easy setup and running.

| Script | Purpose | Features |
|--------|---------|----------|
| `setup_and_run.sh` | Complete automated setup | ✅ Checks prerequisites<br>✅ Builds C binary<br>✅ Sets up venv<br>✅ Installs dependencies<br>✅ Starts server<br>✅ Opens browser |
| `start_server.sh` | Quick server start | For when setup is already done |

**Usage**: `cd Guitar-Pedal && ./scripts/setup_and_run.sh`

---

### **📁 docs/** - Documentation

Comprehensive guides and reference materials.

| Document | Focus | Audience |
|----------|-------|----------|
| `QUICK_START.md` | Getting started ASAP | New users |
| `SETUP_INSTRUCTIONS.md` | Detailed setup guide | All users |
| `FRONTEND_SETUP.md` | Web interface setup | Web developers |
| `ARCHITECTURE.md` | System design | Developers |
| `SETUP_FLOW.md` | Script flow diagrams | Advanced users |
| `TROUBLESHOOTING.md` | Problem solving | All users |

**Total Documentation**: ~3000+ lines covering everything

---

### **📁 examples/** - Sample Audio

Example audio files for testing and demonstration.

| File | Description | Use Case |
|------|-------------|----------|
| `BassForLinus.mp3` | Bass guitar recording | Test echo on bass |
| `input.mp3` | General purpose sample | Quick testing |

**Note**: Add your own audio files here for easy access

---

### **📁 venv/** - Virtual Environment

Python virtual environment (auto-generated, gitignored).

**Contains**:
- Python interpreter (isolated)
- Installed packages (Flask, Werkzeug, etc.)
- Scripts for activation

**Created by**: `python3 -m venv venv`
**Activate**: `source venv/bin/activate`

---

### **📁 uploads/** - Temporary Uploads

Temporary storage for uploaded audio files (auto-generated, gitignored).

**Lifecycle**:
1. User uploads file
2. File saved with UUID name
3. Converted to raw format
4. Processed
5. Original and raw deleted (optional)

**Cleanup**: Can be emptied anytime - files are temporary

---

### **📁 outputs/** - Processed Audio

Storage for processed audio files (auto-generated, gitignored).

**Contents**: Processed MP3 files ready for download

**Naming**: `{session-uuid}_output.mp3`

**Cleanup**: Can be emptied periodically to save disk space

---

## 🔄 File Flow Diagram

```
User uploads file
        │
        ▼
   uploads/*.mp3
        │
        ▼
   uploads/*.raw  (converted by ffmpeg)
        │
        ▼
   ./convert      (C binary processes audio)
        │
        ▼
   outputs/*.raw  (processed audio)
        │
        ▼
   outputs/*.mp3  (converted by ffmpeg)
        │
        ▼
   User downloads
```

---

## 🛠️ Build Artifacts

Files generated during build/run (all gitignored):

| File/Dir | Generated By | Purpose |
|----------|--------------|---------|
| `convert` | `make` | Compiled C audio processor |
| `gensin` | `make` (if built) | Sine table generator |
| `venv/` | `python3 -m venv` | Virtual environment |
| `uploads/` | Flask app | Temporary uploads |
| `outputs/` | Flask app | Processed files |
| `__pycache__/` | Python | Bytecode cache |
| `*.pyc` | Python | Compiled Python |

**Clean up**: `make clean && rm -rf venv uploads outputs`

---

## 📏 Size Reference

Typical directory sizes:

```
src/          ~50 KB    (source code)
web/          ~100 KB   (Flask app + frontend)
docs/         ~200 KB   (documentation)
examples/     ~2 MB     (sample audio)
venv/         ~20 MB    (Python packages)
uploads/      varies    (user files)
outputs/      varies    (processed files)
```

**Total (excluding audio)**: ~20-25 MB

---

## 🎯 What Goes Where?

### **Add New Audio Effect?**
→ Create `src/new_effect.h`
→ Register in `src/convert.c`
→ Add preset in `web/app.py`

### **Add New Documentation?**
→ Create `docs/NEW_DOC.md`

### **Add New Script?**
→ Create `scripts/new_script.sh`
→ Make executable: `chmod +x`

### **Add Example Files?**
→ Put in `examples/`

### **Modify Web UI?**
→ HTML: `web/templates/index.html`
→ CSS: `web/static/css/style.css`
→ JS: `web/static/js/app.js`

### **Modify Backend?**
→ Python: `web/app.py`
→ C: `src/*.c` and `src/*.h`

---

## 🔍 Finding Files

### "Where is the..."

| Looking for... | Location |
|----------------|----------|
| Main README | `Readme.md` |
| C source code | `src/` |
| Web interface | `web/` |
| Setup scripts | `scripts/` |
| Documentation | `docs/` |
| Sample audio | `examples/` |
| Compiled binary | `convert` (root) |
| Virtual environment | `venv/` |

---

## 🧹 Cleanup Commands

```bash
# Clean C build artifacts
make clean

# Clean Python cache
rm -rf __pycache__ web/__pycache__

# Clean virtual environment
rm -rf venv

# Clean temporary files
rm -rf uploads/* outputs/*

# Complete cleanup (keeps source only)
make clean && rm -rf venv uploads outputs __pycache__ web/__pycache__
```

---

## 📊 Organization Benefits

### Before (Messy):
```
❌ 20+ files at root
❌ Code mixed with docs
❌ No clear separation
❌ Hard to navigate
❌ Confusing for new users
```

### After (Organized):
```
✅ Clear separation by purpose
✅ Easy to find files
✅ Logical grouping
✅ Self-documenting structure
✅ Professional appearance
✅ Easier to maintain
```

---

## 🎓 For New Contributors

When contributing:
1. **C code** → Put in `src/`
2. **Web code** → Put in `web/`
3. **Docs** → Put in `docs/`
4. **Scripts** → Put in `scripts/`
5. **Examples** → Put in `examples/`

Keep the root directory clean!

### Quick Reference Commands

```bash
# Build C binary
make

# Run web server
cd web && python3 app.py

# Quick setup (from root)
./scripts/setup_and_run.sh

# Quick start (if already setup)
./scripts/start_server.sh
```

---

## 📱 Multi-Platform Note

This structure works on:
- ✅ macOS
- ✅ Linux
- ✅ Windows (with WSL)

All paths are relative and portable!

---

**Last Updated**: January 2026
**Maintained**: Automatically with structure changes

