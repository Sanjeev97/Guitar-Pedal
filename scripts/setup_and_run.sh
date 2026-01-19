#!/bin/bash
# Guitar Pedal - Complete Setup and Launch Script
# This script will:
# 1. Check prerequisites
# 2. Build the C audio processor
# 3. Set up Python virtual environment
# 4. Install dependencies
# 5. Start the Flask server
# 6. Open the web browser automatically

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Banner
clear
echo -e "${PURPLE}"
cat << 'EOF'
╔════════════════════════════════════════════════════════════╗
║                                                            ║
║               🎸  GUITAR PEDAL SETUP  🎸                  ║
║                                                            ║
║          Digital Audio Effects Processor                   ║
║                                                            ║
╚════════════════════════════════════════════════════════════╝
EOF
echo -e "${NC}"
echo ""

# Function to print status messages
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[✓]${NC} $1"
}

print_error() {
    echo -e "${RED}[✗]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[⚠]${NC} $1"
}

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Step 1: Check Prerequisites
print_status "Step 1/6: Checking prerequisites..."
echo ""

# Check Python 3
if command_exists python3; then
    PYTHON_VERSION=$(python3 --version 2>&1 | awk '{print $2}')
    print_success "Python 3 found: $PYTHON_VERSION"
else
    print_error "Python 3 not found!"
    echo "Please install Python 3.7 or higher:"
    echo "  macOS: brew install python3"
    echo "  Linux: sudo apt install python3"
    exit 1
fi

# Check ffmpeg
if command_exists ffmpeg; then
    FFMPEG_VERSION=$(ffmpeg -version 2>&1 | head -n 1 | awk '{print $3}')
    print_success "ffmpeg found: $FFMPEG_VERSION"
else
    print_error "ffmpeg not found!"
    echo "Please install ffmpeg:"
    echo "  macOS: brew install ffmpeg"
    echo "  Linux: sudo apt install ffmpeg"
    exit 1
fi

# Check GCC/Clang
if command_exists gcc; then
    print_success "GCC compiler found"
    COMPILER="gcc"
elif command_exists clang; then
    print_success "Clang compiler found"
    COMPILER="clang"
else
    print_error "No C compiler found!"
    echo "Please install gcc or clang"
    exit 1
fi

echo ""

# Step 2: Build C Audio Processor
print_status "Step 2/6: Building C audio processor..."
echo ""

if [ -f "src/convert.c" ]; then
    print_status "Cleaning old builds..."
    make clean 2>/dev/null || rm -f convert
    
    print_status "Compiling convert.c..."
    if make; then
        print_success "C audio processor built successfully"
        chmod +x convert
    else
        print_error "Failed to build C audio processor"
        exit 1
    fi
else
    print_error "src/convert.c not found!"
    exit 1
fi

echo ""

# Step 3: Set Up Virtual Environment
print_status "Step 3/6: Setting up Python virtual environment..."
echo ""

if [ -d "venv" ]; then
    print_warning "Virtual environment already exists, using existing one"
else
    print_status "Creating virtual environment..."
    if python3 -m venv venv; then
        print_success "Virtual environment created"
    else
        print_error "Failed to create virtual environment"
        exit 1
    fi
fi

echo ""

# Step 4: Install Dependencies
print_status "Step 4/6: Installing Python dependencies..."
echo ""

# Activate virtual environment
source venv/bin/activate

# Upgrade pip
print_status "Upgrading pip..."
pip install --upgrade pip -q

# Install requirements
if [ -f "web/requirements.txt" ]; then
    print_status "Installing Flask and dependencies..."
    if pip install -r web/requirements.txt -q; then
        print_success "All dependencies installed"
    else
        print_error "Failed to install dependencies"
        exit 1
    fi
else
    print_error "web/requirements.txt not found!"
    exit 1
fi

echo ""

# Step 5: Create Necessary Directories
print_status "Step 5/6: Setting up directories..."
echo ""

mkdir -p uploads outputs
print_success "Created uploads/ directory"
print_success "Created outputs/ directory"

echo ""

# Step 6: Start the Server
print_status "Step 6/6: Starting Flask server..."
echo ""

# Check if port 5000 is available
if lsof -Pi :5000 -sTCP:LISTEN -t >/dev/null 2>&1; then
    print_warning "Port 5000 is already in use!"
    print_status "Attempting to free port 5000..."
    PID=$(lsof -Pi :5000 -sTCP:LISTEN -t)
    if [ ! -z "$PID" ]; then
        kill -9 $PID 2>/dev/null || true
        sleep 1
        print_success "Port 5000 freed"
    fi
fi

# Success message
echo ""
echo -e "${GREEN}"
cat << 'EOF'
╔════════════════════════════════════════════════════════════╗
║                    ✅  SETUP COMPLETE  ✅                 ║
╚════════════════════════════════════════════════════════════╝
EOF
echo -e "${NC}"
echo ""

print_success "All systems ready!"
echo ""
echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${CYAN}  🌐  Opening web browser...${NC}"
echo -e "${CYAN}  📡  Server will start at: http://localhost:5000${NC}"
echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""
echo -e "${YELLOW}📝  Usage Instructions:${NC}"
echo "   1. Drag & drop your audio file (MP3, WAV, OGG, M4A)"
echo "   2. Adjust the effect knobs or select a preset"
echo "   3. Click 'Process Audio'"
echo "   4. Listen to the result and download"
echo ""
echo -e "${YELLOW}⌨️   Keyboard Shortcuts:${NC}"
echo "   • Ctrl+C to stop the server"
echo "   • Refresh browser to restart"
echo ""
echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# Function to open browser based on OS
open_browser() {
    local url="http://127.0.0.1:5000"
    
    # Wait a moment for server to start
    sleep 3
    
    # Detect OS and open browser
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        open "$url" 2>/dev/null
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        # Linux
        if command_exists xdg-open; then
            xdg-open "$url" 2>/dev/null
        elif command_exists sensible-browser; then
            sensible-browser "$url" 2>/dev/null
        elif command_exists google-chrome; then
            google-chrome "$url" 2>/dev/null
        elif command_exists firefox; then
            firefox "$url" 2>/dev/null
        fi
    elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
        # Windows
        start "$url" 2>/dev/null
    fi
}

# Open browser in background
open_browser &

# Start Flask server
print_status "Starting Flask server..."
echo ""
echo -e "${GREEN}════════════════════════════════════════════════════════════${NC}"
echo -e "${GREEN}              🚀  Server Running  🚀${NC}"
echo -e "${GREEN}════════════════════════════════════════════════════════════${NC}"
echo ""

# Run the Flask app (from web directory)
cd web && python3 app.py

# Cleanup on exit
deactivate 2>/dev/null || true

