// util.h - Utility functions for audio processing

// Macro to get array length
// sizeof(arr) gives total bytes, divide by size of one element
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

// Constants we'll use throughout
#define TWO_POW_32 (4294967296.0f) // 2^32 as a float
#define LN2 0.69314718055994530942  // Natural log of 2

// Fast approximation of 2^x - 1 using Taylor series
// Why "minus 1"? Because 2^0 = 1, and for small x, 2^x ≈ 1 + small_value
// Subtracting 1 gives us just the "small_value" part, which is more precise
//
// Taylor series: 2^x = 1 + x*ln(2) + x²*ln(2)²/2! + x³*ln(2)³/3! + ...
// So: 2^x - 1 = x*ln(2) + x²*ln(2)²/2 + x³*ln(2)³/6 + x⁴*ln(2)⁴/24 + ...
static inline float fastpow2_m1(float x)
{
    // Pre-computed coefficients
    const float c1 = LN2,               // ln(2)
                c2 = LN2*LN2/2,         // ln(2)²/2
                c3 = LN2*LN2*LN2/6,     // ln(2)³/6
                c4 = LN2*LN2*LN2/24;    // ln(2)⁴/24 (note: should be LN2^4/24)
    
    float x2 = x*x;      // x squared
    float x3 = x2*x;     // x cubed
    
    // Sum up the Taylor series terms
    return c1*x + c2*x2 + c3*x3 + c4*x2*x2;
}

// Fast approximation of a^b using bit manipulation
// This exploits how floating-point numbers are stored in memory (IEEE 754)
// The exponent bits of a float approximate log2(value)
static inline float fastpow(float a, float b)
{
    union { float f; int i; } u = { a };
    // Magic numbers come from IEEE 754 float format
    u.i = (int) (b * (u.i - 1072632447) + 1072632447.0f);
    return u.f;
}

//Soft limiter: smoothly constrains values to approximately -1 to +1
// 
// Without limiting, adding two signals could give values > 1.0, causing
// harsh digital clipping (distortion). This function smoothly compresses
// the signal as it approaches the limits.
//
// The polynomial was designed so that:
// - limit_value(0) = 0
// - limit_value(1) ≈ 0.83
// - limit_value(2) ≈ 0.98
// - Values smoothly approach but never quite reach ±1
static float limit_value(float x)
{
    float x2 = x*x;
    float x4 = x2*x2;
    // This polynomial flattens out as |x| increases
    return x*(1 - 0.19*x2 + 0.0162*x4);
}

// Convert unsigned integer to fraction 0.0 to 1.0
// Used for phase accumulators in the LFO
static inline float uint_to_fraction(unsigned int val)
{
    return (1.0/TWO_POW_32) * val;
}

// Convert fraction to unsigned integer (inverse of above)
static inline unsigned int fraction_to_uint(float val)
{
    return (unsigned int) (val * TWO_POW_32);
}

//
// DELAY BUFFER
//
// Many effects (echo, flanger, chorus) need to "remember" past samples
// and look them up later. We store samples in a circular buffer.
//

// Buffer size: 65536 samples = ~1.36 seconds at 48kHz
// Power of 2 makes wraparound math fast (just AND with mask)
#define SAMPLE_ARRAY_SIZE 65536
#define SAMPLE_ARRAY_MASK (SAMPLE_ARRAY_SIZE-1)


// The actual buffer and current write position
// 'extern' means these are defined elsewhere (in effect.h)
extern float sample_array[SAMPLE_ARRAY_SIZE];
extern int sample_array_index;


// Write a sample to the delay buffer
// We increment the index, then write (so index always points to newest sample)
static inline void sample_array_write(float val)
{
    // Increment and wrap around using bitwise AND
    // e.g., if index is 65535, adding 1 gives 65536
    // 65536 & 65535 = 0, so we wrap back to start
    unsigned int idx = SAMPLE_ARRAY_MASK & ++sample_array_index;
    sample_array[idx] = val;
}

// Read a sample from 'delay' samples ago
// Supports fractional delays using linear interpolation
static inline float sample_array_read(float delay)
{
    int i = (int) delay;           // Integer part of delay
    float frac = delay - i;        // Fractional part (0.0 to 1.0)
    int idx = sample_array_index - i;  // Position of sample 'i' samples ago
    
    // Get two adjacent samples
    float a = sample_array[SAMPLE_ARRAY_MASK & idx];      // Sample at position
    float b = sample_array[SAMPLE_ARRAY_MASK & (idx+1)];  // Next sample
    
    // Linear interpolation: blend between a and b based on fraction
    // If frac=0, return a. If frac=1, return b. In between, blend.
    return a + (b-a)*frac;
}

// We can calculate sin/cos at the same time using
// the table lookup. It's "GoodEnough(tm)" and with
// 256 entries it's good to about 4.5 digits of
// precision if I tested it right.
//
// Don't use this for real work. For audio? It's fine.
#include "gensin.h"

#define QUARTER_SINE_STEPS (1<< QUARTER_SINE_STEP_SHIFT)
