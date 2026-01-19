// effect.h - Common state and utilities for audio effects
//
// Effects don't have to use these, but they make common operations simple.

// Allocate the delay buffer (declared extern in util.h)
float sample_array[SAMPLE_ARRAY_SIZE];
int sample_array_index;

// Shared effect parameters
static float effect_feedback;                    // How much output feeds back to input
static float effect_delay, target_effect_delay;  // Current and target delay in samples
static float effect_depth;                       // Modulation depth (0-1)
static struct lfo_state effect_lfo;              // LFO for modulation

// Convenience macros for setting parameters
#define effect_set_lfo(f)       set_lfo_freq(&effect_lfo, f)
#define effect_set_lfo_ms(ms)   set_lfo_ms(&effect_lfo, ms)
#define effect_set_depth(d)     effect_depth = (d)
#define effect_set_feedback(fb) effect_feedback = (fb)

// Convert milliseconds to samples
// 48000 samples/sec * 0.001 sec/ms = 48 samples/ms
#define SAMPLES_PER_MSEC (SAMPLES_PER_SEC * 0.001)

// Set delay time in milliseconds
// Uses a "target" so we can smoothly transition (avoids clicks)
static inline void effect_set_delay(float ms)
{
    float samples = ms * SAMPLES_PER_MSEC;
    
    // Sanity check: delay must be positive and fit in buffer
    if (samples > 0 && samples < SAMPLE_ARRAY_SIZE)
        target_effect_delay = samples;
}