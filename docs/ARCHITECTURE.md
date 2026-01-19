# Guitar Pedal - System Architecture

## Overview

The Guitar Pedal project consists of two main components:
1. **C Backend** - High-performance audio DSP processor (in `src/`)
2. **Python/Flask Frontend** - Web-based user interface (in `web/`)

**Note**: This project follows an organized directory structure. See [DIRECTORY_STRUCTURE.md](../DIRECTORY_STRUCTURE.md) for complete file organization details.

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                        USER BROWSER                          │
│  ┌────────────────────────────────────────────────────┐     │
│  │              index.html (UI)                       │     │
│  │  ┌──────────┐  ┌──────────┐  ┌──────────┐        │     │
│  │  │  Upload  │  │  Knobs   │  │  Audio   │        │     │
│  │  │   Area   │  │ Controls │  │  Players │        │     │
│  │  └──────────┘  └──────────┘  └──────────┘        │     │
│  └────────────────────────────────────────────────────┘     │
│                          │                                   │
│                    app.js (Logic)                            │
│                          │                                   │
│                    AJAX/Fetch API                            │
└──────────────────────────┼──────────────────────────────────┘
                           │ HTTP
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                   FLASK WEB SERVER                           │
│                      (app.py)                                │
│  ┌──────────────────────────────────────────────────────┐   │
│  │  Routes:                                             │   │
│  │  • /          → Serve HTML                          │   │
│  │  • /upload    → Handle file upload                  │   │
│  │  • /presets   → Return effect presets               │   │
│  │  • /play/*    → Stream audio                        │   │
│  │  • /download  → Send processed file                 │   │
│  └──────────────────────────────────────────────────────┘   │
│                          │                                   │
│                    Process Flow:                             │
│                          │                                   │
│  ┌───────────────────────┼───────────────────────────────┐  │
│  │ 1. Save uploaded file │                               │  │
│  │ 2. Convert to raw  ───┼──► ffmpeg                    │  │
│  │ 3. Apply effect    ───┼──► ./convert (C program)     │  │
│  │ 4. Convert to MP3  ───┼──► ffmpeg                    │  │
│  │ 5. Return result      │                               │  │
│  └───────────────────────┼───────────────────────────────┘  │
└──────────────────────────┼──────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                   C AUDIO PROCESSOR                          │
│                    (./convert)                               │
│  ┌──────────────────────────────────────────────────────┐   │
│  │  Main Loop (convert.c)                              │   │
│  │    • Read sample from stdin                         │   │
│  │    • Convert to float                               │   │
│  │    • Call effect_step()                             │   │
│  │    • Convert back to int                            │   │
│  │    • Write to stdout                                │   │
│  └──────────────────────┬───────────────────────────────┘   │
│                         │                                    │
│  ┌──────────────────────▼───────────────────────────────┐   │
│  │  Echo Effect (echo.h)                               │   │
│  │    • Read delayed sample                            │   │
│  │    • Mix with feedback                              │   │
│  │    • Write to delay buffer                          │   │
│  │    • Return mix of dry/wet                          │   │
│  └──────────────────────┬───────────────────────────────┘   │
│                         │                                    │
│  ┌──────────────────────▼───────────────────────────────┐   │
│  │  Utilities                                          │   │
│  │  ┌──────────┐  ┌──────────┐  ┌──────────┐        │   │
│  │  │  Delay   │  │   LFO    │  │  Soft    │        │   │
│  │  │  Buffer  │  │  (lfo.h) │  │ Limiter  │        │   │
│  │  │ (util.h) │  │          │  │ (util.h) │        │   │
│  │  └──────────┘  └──────────┘  └──────────┘        │   │
│  └──────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
                           │
                           ▼
                    ┌──────────────┐
                    │  File System │
                    │              │
                    │  uploads/    │
                    │  outputs/    │
                    └──────────────┘
```

## Data Flow

### 1. File Upload Flow

```
User Browser
    │
    ├─ User drops MP3 file
    │
    ▼
JavaScript (app.js)
    │
    ├─ Create FormData
    ├─ Add file + parameters
    │
    ▼
POST /upload (Flask)
    │
    ├─ Validate file type
    ├─ Generate session ID
    ├─ Save to uploads/
    │
    ▼
Processing Pipeline
```

### 2. Audio Processing Pipeline

```
Input File (MP3/WAV)
    │
    ▼
┌───────────────────┐
│  ffmpeg convert   │  Convert to raw format
│  MP3 → RAW       │  • 32-bit signed int
└─────────┬─────────┘  • 48kHz sample rate
          │            • Mono channel
          ▼
┌───────────────────┐
│  ./convert echo   │  Apply audio effect
│  stdin → stdout   │  • Read samples
└─────────┬─────────┘  • Process through DSP
          │            • Write samples
          ▼
┌───────────────────┐
│  ffmpeg convert   │  Convert to MP3
│  RAW → MP3       │  • 320kbps bitrate
└─────────┬─────────┘  • Standard MP3 format
          │
          ▼
Output File (MP3)
```

### 3. Audio Effect Processing (Per Sample)

```
Input Sample (float: -1.0 to +1.0)
    │
    ▼
┌───────────────────────────────┐
│  Read Delayed Sample          │
│  position = current - delay   │
│  sample = buffer[position]    │
└───────────┬───────────────────┘
            │
            ▼
┌───────────────────────────────┐
│  Apply Feedback               │
│  mixed = input + (delayed *   │
│          feedback_amount)     │
└───────────┬───────────────────┘
            │
            ▼
┌───────────────────────────────┐
│  Soft Limiter                 │
│  prevent clipping from        │
│  feedback accumulation        │
└───────────┬───────────────────┘
            │
            ▼
┌───────────────────────────────┐
│  Write to Delay Buffer        │
│  buffer[current] = mixed      │
└───────────┬───────────────────┘
            │
            ▼
┌───────────────────────────────┐
│  Return Output                │
│  output = (input + delayed)/2 │
└───────────┬───────────────────┘
            │
            ▼
Output Sample (float: -1.0 to +1.0)
```

## Component Details

### Frontend Components

#### 1. HTML (templates/index.html)
- **Purpose**: UI structure
- **Sections**:
  - Header with title
  - Upload area (drag-and-drop)
  - Effect controls (knobs/sliders)
  - Audio players (original/processed)
  - Download button

#### 2. CSS (static/css/style.css)
- **Purpose**: Styling and layout
- **Features**:
  - Dark modern theme
  - Responsive grid layout
  - 3D-style knobs
  - Smooth animations
  - Mobile-friendly

#### 3. JavaScript (static/js/app.js)
- **Purpose**: Client-side logic
- **Responsibilities**:
  - File upload handling
  - Knob interaction
  - Preset management
  - AJAX communication
  - Error handling

### Backend Components

#### 1. Flask Server (app.py)
- **Purpose**: Web server and orchestration
- **Responsibilities**:
  - Serve static files
  - Handle file uploads
  - Coordinate processing
  - Stream audio
  - Manage sessions

#### 2. C Audio Processor (convert.c)
- **Purpose**: Real-time audio DSP
- **Responsibilities**:
  - Sample-by-sample processing
  - Effect application
  - High-performance computation

#### 3. DSP Modules

**Delay Buffer (util.h)**
- Circular buffer (65,536 samples)
- Read/write with interpolation
- ~1.36 seconds at 48kHz

**LFO (lfo.h)**
- Phase accumulator
- Sine/triangle/saw waveforms
- Used for modulation

**Echo Effect (echo.h)**
- Main effect algorithm
- Configurable parameters
- Feedback control

## Technology Stack

### Frontend
- **HTML5**: Structure
- **CSS3**: Styling with Grid/Flexbox
- **Vanilla JavaScript**: No frameworks (lightweight)
- **Web Audio API**: Audio playback

### Backend
- **Python 3.7+**: Server language
- **Flask 3.0**: Web framework
- **C (GCC/Clang)**: Audio processing
- **ffmpeg**: Audio conversion

## Performance Characteristics

### Processing Speed
- **Sample Rate**: 48,000 samples/second
- **Processing**: Real-time capable (~1-2x file duration)
- **Latency**: N/A (offline processing)

### Memory Usage
- **Delay Buffer**: 256 KB (65,536 × 4 bytes)
- **Python Server**: ~50-100 MB
- **Per Session**: ~2× input file size

### File Size Limits
- **Recommended**: < 50 MB input files
- **Maximum**: Depends on system RAM
- **Output**: Similar to input (320 kbps MP3)

## Security Considerations

### Current Implementation (Development)
- ⚠️ No authentication
- ⚠️ No rate limiting
- ⚠️ No file size validation
- ⚠️ Session IDs are UUIDs (good)

### Production Requirements
- ✓ Add authentication
- ✓ Implement rate limiting
- ✓ Validate file sizes
- ✓ Sanitize file names
- ✓ Add CSRF protection
- ✓ Use HTTPS
- ✓ Regular cleanup of temp files

## Scalability

### Current Limitations
- **Single-threaded**: Processes one file at a time
- **In-memory**: No persistent storage
- **Local**: Runs on one machine

### Scaling Options

**Horizontal Scaling**
- Add Celery for background processing
- Use Redis for queue management
- Deploy multiple worker instances
- Load balance with nginx

**Vertical Scaling**
- Increase CPU cores
- More RAM for larger buffers
- Faster storage (SSD)

**Cloud Deployment**
- AWS Lambda for serverless
- S3 for file storage
- CloudFront for distribution

## Extension Points

### Adding New Effects

1. Create new effect header (e.g., `flanger.h`)
2. Implement `<effect>_init()` and `<effect>_step()`
3. Register in `convert.c`:
   ```c
   struct effect effects[] = {
       EFF(echo),
       EFF(flanger),  // Add here
   };
   ```
4. Update Flask routes if needed
5. Add presets in `app.py`

### Adding Real-time Preview

Replace offline processing with:
- WebAssembly compilation of C code
- Web Audio API worklets
- Client-side processing

### Adding Waveform Visualization

- Use Web Audio API `AnalyserNode`
- Draw to HTML5 Canvas
- Display during playback

## File Structure Summary

```
Guitar-Pedal/
├── 📄 Readme.md                # Main documentation
├── 📄 Makefile                 # Build system
├── 📄 DIRECTORY_STRUCTURE.md   # Complete structure guide
├── 📄 .gitignore               # Git rules
│
├── 📁 src/                     # C Audio Processor
│   ├── convert.c               # Main processor
│   ├── echo.h                  # Echo effect
│   ├── effects.h               # Effect framework
│   ├── lfo.h                   # LFO module
│   ├── util.h                  # Utilities
│   ├── gensin.c                # Sine table generator
│   └── gensin.h                # Sine lookup table
│
├── 📁 web/                     # Flask Web Application
│   ├── app.py                  # Flask server
│   ├── requirements.txt        # Python dependencies
│   ├── templates/
│   │   └── index.html          # Main UI
│   └── static/
│       ├── css/
│       │   └── style.css       # Styling
│       └── js/
│           └── app.js          # Frontend logic
│
├── 📁 scripts/                 # Automation Scripts
│   ├── setup_and_run.sh        # One-command setup
│   └── start_server.sh         # Quick start
│
├── 📁 docs/                    # Documentation
│   ├── ARCHITECTURE.md         # This file
│   ├── FRONTEND_SETUP.md       # Frontend setup guide
│   ├── QUICK_START.md          # Quick reference
│   ├── SETUP_FLOW.md           # Setup flow diagrams
│   ├── SETUP_INSTRUCTIONS.md   # Detailed instructions
│   └── TROUBLESHOOTING.md      # Problem solving
│
├── 📁 examples/                # Sample Audio Files
│   ├── BassForLinus.mp3        # Bass example
│   └── input.mp3               # General sample
│
├── 📁 venv/                    # Virtual environment (auto-generated)
├── 📁 uploads/                 # Temp uploads (auto-generated)
├── 📁 outputs/                 # Processed files (auto-generated)
└── 🔧 convert                  # Compiled binary (auto-generated)
```

**See [DIRECTORY_STRUCTURE.md](../DIRECTORY_STRUCTURE.md) for complete details.**

## Key Design Decisions

### Why C for Audio Processing?
- **Performance**: Sample-by-sample processing needs speed
- **Control**: Direct memory management for buffers
- **Legacy**: Based on Linus Torvalds' toolkit
- **Portability**: Compiles everywhere

### Why Flask for Backend?
- **Simplicity**: Easy to understand and extend
- **Lightweight**: No heavy framework overhead
- **Python**: Great for orchestration and file handling
- **Flexibility**: Easy to add features

### Why Vanilla JavaScript?
- **No Dependencies**: Faster load times
- **Simplicity**: Easy to understand
- **Control**: Full control over behavior
- **Compatibility**: Works everywhere

## Future Enhancements

### Short Term
- [ ] Add more effects (flanger, chorus, distortion)
- [ ] Waveform visualization
- [ ] Batch processing
- [ ] Stereo support

### Medium Term
- [ ] Real-time preview
- [ ] User presets (save/load)
- [ ] Effect chaining
- [ ] Mobile app (React Native)

### Long Term
- [ ] Plugin system
- [ ] Cloud deployment
- [ ] Social sharing
- [ ] Effect marketplace

---

**Last Updated**: January 2026

