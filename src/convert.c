//  convert.c - Main audio processing program
//
// Reads raw 32-bit audio from stdin, processes through an effect,
// writes processed audio to stdout.
//
// Usage: ./convert <effect_name> <pot1> <pot2> <pot3> <pot4>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Type definitions for clarity
typedef int s32;              // Signed 32-bit integer
typedef unsigned int u32;     // Unsigned 32-bit
typedef unsigned int uint;    // Shorthand

// Audio format: 48kHz sample rate
#define SAMPLES_PER_SEC (48000.0)

// Include our DSP building blocks
#include "util.h"      // Math helpers, delay buffer
#include "lfo.h"       // Low-frequency oscillator
#include "effects.h"    // Shared effect state

// Include effects
#include "echo.h"

// Effect registry - maps names to init/step functions
#define EFF(x) { #x, x##_init, x##_step }

struct effect {
    const char *name;                               // Effect name for command line
    void (*init)(float,float,float,float);          // Initialization function
    float (*step)(float);                           // Per-sample processing function
} effects[] = {
    EFF(echo),
    // EFF(flanger),
};

//  Macro for smooth parameter transitions
#define UPDATE(x) x += 0.001 * (target_##x - x)

int main(int argc, char **argv)
{
    float pot[4];                    // Four control parameters
    struct effect *eff = &effects[0];  // Default to first effect
    s32 sample;                      // Current audio sample
    
    // Check command line arguments
    if (argc < 6) {
        fprintf(stderr, "Usage: %s <effect> <pot1> <pot2> <pot3> <pot4>\n", argv[0]);
        fprintf(stderr, "Effects: ");
        for (int i = 0; i < ARRAY_SIZE(effects); i++)
            fprintf(stderr, "%s ", effects[i].name);
        fprintf(stderr, "\n");
        return 1;
    }
    
    // Find the requested effect by name
    const char *name = argv[1];
    for (int i = 0; i < ARRAY_SIZE(effects); i++) {
        if (!strcmp(name, effects[i].name))
            eff = effects + i;
    }
    
    // Parse the four pot values
    for (int i = 0; i < 4; i++)
        pot[i] = atof(argv[2+i]);
    
    // Print what we're doing
    fprintf(stderr, "Playing %s(%f,%f,%f,%f)\n",
        eff->name, pot[0], pot[1], pot[2], pot[3]);
    
    // Initialize the effect
    eff->init(pot[0], pot[1], pot[2], pot[3]);
    
    // Main processing loop - one sample at a time
    while (fread(&sample, 4, 1, stdin) == 1) {
        // Convert 32-bit integer to float (-1.0 to +1.0)
        // 0x80000000 = 2^31 = 2,147,483,648
        float in = sample / (float)0x80000000;
        
        // Smoothly update delay toward target (prevents clicks)
        UPDATE(effect_delay);
        
        // Process through the effect
        float out = eff->step(in);
        
        // Convert back to 32-bit integer
        sample = (int)(out * 0x80000000);
        
        // Write to stdout
        if (fwrite(&sample, 4, 1, stdout) != 1)
            return 1;
    }
    
    return 0;
}