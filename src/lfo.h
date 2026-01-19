// lfo.h - Low Frequency Oscillator
//
// The LFO generates periodic waveforms (sine, triangle, sawtooth)
// at frequencies typically below 20 Hz.

// This constant converts frequency to step size
// At 48kHz sample rate, a full 32-bit cycle takes:
//   2^32 / 48000 ≈ 89,478 samples ≈ 1.86 seconds
// So step = frequency * (2^32 / 48000)

#define F_STEP (TWO_POW_32/SAMPLES_PER_SEC)

// Types of waveforms the LFO can produce
enum lfo_type {
    lfo_sinewave, // Smooth, natural sounding
    lfo_triangle, // Linear ramps ups and down
    lfo_sawtooth, // Ramp up, then drops
};

// LFO state, just a phase position and increment
struct lfo_state {
    unsigned int idx; // Current position in the cycle (0 to 2^32-1)
    unsigned int step; // How much to advance each sample
};

// Macro for initializing an LFO at a specific frequency
#define LFO_FREQ(x) .step = (x)*F_STEP

// Set LFO step directly
static inline void set_lfo_step(struct lfo_state *lfo, float step)
{
    lfo->step = (unsigned int) rintf(step); // rintf rounds to the nearest int
}

// Set LFO by frequency in Hz
// Example: set_lfo_freq(&lfo, 2.0) makes the LFO complete 2 cycles per second
void set_lfo_freq(struct lfo_state *lfo, float freq)
{
    set_lfo_step(lfo, freq * F_STEP);
}

// Set LFO by period in milliseconds
// Example: set_lfo_ms(&lfo, 500) makes one cycle take 500ms
void set_lfo_ms(struct lfo_state *lfo, float ms)
{
    // Clamp to minimum 0.1ms (10kHz max) to prevent division issues
    if (ms < 0.1)
        ms = 0.1;
    // period = 1000/freq, so freq = 1000/period
    // step = freq * F_STEP = 1000 * F_STEP / ms
    set_lfo_step(lfo, 1000 * F_STEP / ms);
}

// Advance the LFO by one sample and return current value (-1 to +1)
float lfo_step(struct lfo_state *lfo, enum lfo_type type)
{
    unsigned int now = lfo->idx;
    unsigned int next = now + lfo->step;
    
    lfo->idx = next;  // Update for next call
    
    // Sawtooth is simplest: just convert the index to 0.0-1.0
    if (type == lfo_sawtooth)
        return uint_to_fraction(now);
    
    float val;
    
    // For sine and triangle, we use quarter-cycle symmetry
    // Top 2 bits tell us which quarter we're in (0-3)
    unsigned int quarter = now >> 30;
    
    // Shift left by 2 to get position within the quarter (0 to 2^32-1)
    now <<= 2;
    
    // In quarters 1 and 3, we go backwards (mirror image)
    if (quarter & 1)
        now = ~now;  // Bitwise NOT flips the direction
    
    if (type == lfo_sinewave) {
        // Look up in sine table
        unsigned int idx = now >> (32 - QUARTER_SINE_STEP_SHIFT);
        float a = quarter_sin[idx];
        float b = quarter_sin[idx+1];
        
        // Get fractional part for interpolation
        now <<= QUARTER_SINE_STEP_SHIFT;
        val = a + (b-a)*uint_to_fraction(now);
    } else {
        // Triangle: just linear ramp
        val = uint_to_fraction(now);
    }
    
    // In quarters 2 and 3, the value is negative
    if (quarter & 2)
        val = -val;
    
    return val;
}

