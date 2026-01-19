# Setup Script Flow Diagram

## Visual Overview of `setup_and_run.sh`

```
                    ./setup_and_run.sh
                            │
                            │
                            ▼
        ┌───────────────────────────────────────┐
        │   Display ASCII Art Banner            │
        │   🎸 GUITAR PEDAL SETUP 🎸           │
        └───────────────────┬───────────────────┘
                            │
                            ▼
        ┌───────────────────────────────────────┐
        │   Step 1: Check Prerequisites         │
        │   ├─ Python 3.7+ ?                   │
        │   ├─ ffmpeg installed?               │
        │   └─ GCC/Clang available?            │
        └───────────────────┬───────────────────┘
                            │
                ┌───────────┴───────────┐
                │                       │
            [PASS]                  [FAIL]
                │                       │
                │                       ▼
                │           Show error + install instructions
                │                       │
                │                       ▼
                │                     EXIT
                │
                ▼
        ┌───────────────────────────────────────┐
        │   Step 2: Build C Audio Processor     │
        │   ├─ make clean                       │
        │   ├─ make                             │
        │   └─ chmod +x convert                 │
        └───────────────────┬───────────────────┘
                            │
                            ▼
        ┌───────────────────────────────────────┐
        │   Step 3: Virtual Environment         │
        │                                       │
        │   venv/ exists?                       │
        └───────────────────┬───────────────────┘
                            │
                ┌───────────┴───────────┐
                │                       │
              [YES]                   [NO]
                │                       │
                │                       ▼
                │           python3 -m venv venv
                │                       │
                └───────────┬───────────┘
                            │
                            ▼
        ┌───────────────────────────────────────┐
        │   Step 4: Install Dependencies        │
        │   ├─ source venv/bin/activate         │
        │   ├─ pip install --upgrade pip        │
        │   └─ pip install -r requirements.txt  │
        └───────────────────┬───────────────────┘
                            │
                            ▼
        ┌───────────────────────────────────────┐
        │   Step 5: Create Directories          │
        │   ├─ mkdir -p uploads                 │
        │   └─ mkdir -p outputs                 │
        └───────────────────┬───────────────────┘
                            │
                            ▼
        ┌───────────────────────────────────────┐
        │   Step 6: Port Management             │
        │                                       │
        │   Port 5000 in use?                   │
        └───────────────────┬───────────────────┘
                            │
                ┌───────────┴───────────┐
                │                       │
              [YES]                   [NO]
                │                       │
                ▼                       │
            Kill process                │
            on port 5000                │
                │                       │
                └───────────┬───────────┘
                            │
                            ▼
        ┌───────────────────────────────────────┐
        │   Display Success Banner              │
        │   ✅ SETUP COMPLETE ✅                │
        └───────────────────┬───────────────────┘
                            │
                            ▼
        ┌───────────────────────────────────────┐
        │   Show Usage Instructions             │
        │   • How to upload files               │
        │   • Keyboard shortcuts                │
        │   • Tips and tricks                   │
        └───────────────────┬───────────────────┘
                            │
                            ▼
        ┌───────────────────────────────────────┐
        │   Launch Browser (Background)         │
        │                                       │
        │   OS Detection:                       │
        │   ├─ macOS    → open                  │
        │   ├─ Linux    → xdg-open             │
        │   └─ Windows  → start                │
        └───────────────────┬───────────────────┘
                            │
                            │ (wait 2 seconds)
                            │
                            ▼
        ┌───────────────────────────────────────┐
        │   Start Flask Server                  │
        │   python3 app.py                      │
        │                                       │
        │   Server running on:                  │
        │   http://localhost:5000               │
        └───────────────────┬───────────────────┘
                            │
                            │
            ┌───────────────┴───────────────┐
            │                               │
        User uses                      User presses
        web interface                    Ctrl+C
            │                               │
            │                               ▼
            │                   ┌───────────────────────┐
            │                   │   Cleanup & Exit      │
            │                   │   deactivate venv     │
            │                   └───────────────────────┘
            │                               │
            │                               ▼
            │                             END
            │
            ▼
    Processes audio files...
```

---

## Detailed Step Breakdown

### Step 1: Prerequisites Check 🔍

```
Check Python 3:
├─ Found? → Show version ✅
└─ Not found? → Show install command ❌ → EXIT

Check ffmpeg:
├─ Found? → Show version ✅
└─ Not found? → Show install command ❌ → EXIT

Check C Compiler:
├─ GCC found? → Use gcc ✅
├─ Clang found? → Use clang ✅
└─ None found? → Show error ❌ → EXIT
```

### Step 2: Build Audio Processor 🔨

```
Clean old builds:
    make clean (or rm -f convert)
    
Compile:
    make
    └─ gcc -O2 -o convert src/convert.c -lm
    
Set permissions:
    chmod +x convert
    
Verify:
    ├─ Success? → Continue ✅
    └─ Failed? → Show error ❌ → EXIT
```

### Step 3: Virtual Environment 📦

```
Check if venv/ exists:
├─ Exists → Reuse it ♻️
└─ Doesn't exist → Create new
    └─ python3 -m venv venv
    
Activate:
    source venv/bin/activate
    
Verify:
    ├─ Activated? → Continue ✅
    └─ Failed? → Show error ❌ → EXIT
```

### Step 4: Install Dependencies 📥

```
Upgrade pip:
    pip install --upgrade pip -q
    
Install requirements:
    pip install -r web/requirements.txt -q
    ├─ Flask 3.0.0
    ├─ Werkzeug 3.0.1
    ├─ Jinja2
    ├─ MarkupSafe
    ├─ itsdangerous
    ├─ click
    └─ blinker
    
Verify:
    ├─ All installed? → Continue ✅
    └─ Failed? → Show error ❌ → EXIT
```

### Step 5: Directory Setup 📁

```
Create directories:
├─ mkdir -p uploads/
└─ mkdir -p outputs/

Always succeeds ✅
```

### Step 6: Port Management 🔌

```
Check port 5000:
├─ Available? → Continue ✅
└─ In use?
    └─ Find process PID
        └─ kill -9 $PID
            └─ Port freed ✅
```

### Browser Launch 🌐

```
Detect Operating System:

macOS:
    open http://127.0.0.1:5000
    
Linux:
    xdg-open http://127.0.0.1:5000
    (or sensible-browser, google-chrome, firefox)
    
Windows:
    start http://127.0.0.1:5000

Runs in background process ⚙️
```

### Server Start 🚀

```
Start Flask:
    cd web && python3 app.py
    
Server initializes:
├─ Load routes (/, /upload, /presets, etc.)
├─ Bind to 127.0.0.1:5000
└─ Ready to accept connections
    
Shows:
    * Running on http://127.0.0.1:5000
    * Debug mode: on
```

---

## Timing Diagram

```
Time    Action                          Duration
─────────────────────────────────────────────────
0:00    Script starts                   instant
0:01    Check prerequisites             1-2 sec
0:03    Build C processor               1-2 sec
0:05    Setup/check venv                2-5 sec*
0:10    Install dependencies            10-30 sec*
0:40    Create directories              instant
0:40    Check/free port                 1 sec
0:41    Display success message         instant
0:42    Launch browser (background)     instant
0:43    Start Flask server              1-2 sec
0:45    Browser opens                   (background)
─────────────────────────────────────────────────
TOTAL:  First run: 30-60 seconds
        Subsequent: 5-10 seconds

* Faster on subsequent runs (skips if exists)
```

---

## Error Handling Flow

```
Any Step Fails:
    │
    ├─ Print error message in RED
    ├─ Show helpful fix instructions
    ├─ EXIT with code 1
    └─ User fixes issue
        └─ Re-run ./setup_and_run.sh
            └─ Script continues from that step
```

---

## State Management

### First Run
```
Before:                     After:
Guitar-Pedal/              Guitar-Pedal/
├── src/                   ├── src/
├── web/                   ├── web/
├── scripts/               ├── scripts/
└── ...                    ├── convert ✨ (built)
                           ├── venv/ ✨ (created)
                           ├── uploads/ ✨
                           ├── outputs/ ✨
                           └── ...
```

### Subsequent Runs
```
Checks:
├─ convert exists? → Skip build
├─ venv/ exists? → Reuse
└─ Just start server
    └─ Opens browser
        └─ Much faster!
```

---

## Color Coding

The script uses colors for better readability:

| Color | Meaning | Usage |
|-------|---------|-------|
| 🔵 BLUE | Info | [INFO] messages |
| 🟢 GREEN | Success | [✓] confirmations |
| 🔴 RED | Error | [✗] failures |
| 🟡 YELLOW | Warning | [⚠] warnings |
| 🟣 PURPLE | Banner | ASCII art headers |
| 🔷 CYAN | Highlight | Important URLs/commands |

---

## Exit Codes

| Code | Meaning |
|------|---------|
| 0 | Success (user pressed Ctrl+C to stop) |
| 1 | Error (missing prerequisite or build failed) |

---

## Script Features

### ✨ Smart Detection
- Checks what's already installed
- Skips unnecessary steps
- Reuses existing setup

### 🛡️ Safety
- `set -e` → Exits on any error
- Validates each step
- Shows clear error messages

### 🎨 User-Friendly
- Colorful output
- Progress indicators
- Usage instructions
- Auto-browser launch

### ⚡ Performance
- First run: ~30-60 seconds
- Next runs: ~5-10 seconds
- Parallel browser launch

---

## Comparison: Scripts Overview

| Feature | setup_and_run.sh | start_server.sh |
|---------|------------------|-----------------|
| **Auto-build** | ✅ | ❌ |
| **Check prereqs** | ✅ | ✅ |
| **Setup venv** | ✅ | ✅ |
| **Install deps** | ✅ | ✅ |
| **Port mgmt** | ✅ | ❌ |
| **Open browser** | ✅ | ❌ |
| **Colors** | ✅ | Minimal |
| **Usage guide** | ✅ | ❌ |
| **Complete setup** | ✅ | Partial |

**Recommendation**: Use `setup_and_run.sh` for first-time and regular use!

---

## Summary

The `setup_and_run.sh` script provides:
- 🎯 **One-command setup** - Everything automated
- 🔍 **Prerequisite checking** - Ensures system is ready
- 🔨 **Automatic building** - Compiles C processor
- 📦 **Isolated environment** - Virtual environment
- 🌐 **Browser automation** - Opens automatically
- ✨ **Smart execution** - Fast on subsequent runs
- 🎨 **Beautiful output** - Easy to follow
- 🛡️ **Error handling** - Clear messages

**Result**: From zero to running web interface in under 1 minute! 🚀

