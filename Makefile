# Makefile for Guitar Pedal Audio Processor

# Compiler settings
CC = gcc
CFLAGS = -O2 -Wall
LIBS = -lm

# Directories
SRC_DIR = src
BUILD_DIR = .

# Source files
SOURCES = $(SRC_DIR)/convert.c
HEADERS = $(SRC_DIR)/echo.h $(SRC_DIR)/effects.h $(SRC_DIR)/lfo.h $(SRC_DIR)/util.h $(SRC_DIR)/gensin.h

# Output binary
TARGET = convert

# Default target
all: $(TARGET)

# Build the convert binary
$(TARGET): $(SOURCES) $(HEADERS)
	@echo "🔨 Building audio processor..."
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)
	@echo "✅ Build complete: ./$(TARGET)"

# Clean build artifacts
clean:
	@echo "🧹 Cleaning build artifacts..."
	rm -f $(TARGET)
	@echo "✅ Clean complete"

# Rebuild from scratch
rebuild: clean all

# Help target
help:
	@echo "Guitar Pedal Audio Processor - Makefile"
	@echo ""
	@echo "Usage:"
	@echo "  make          Build the audio processor"
	@echo "  make clean    Remove build artifacts"
	@echo "  make rebuild  Clean and rebuild"
	@echo "  make help     Show this help message"
	@echo ""
	@echo "After building, use:"
	@echo "  ./convert echo <pot1> <pot2> <pot3> <pot4> < input.raw > output.raw"

.PHONY: all clean rebuild help
