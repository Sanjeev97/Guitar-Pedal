// echo.h - Simple echo/delay effect
//
// Echo = mix the current sound with a delayed copy of itself
// With feedback, the echo repeats and fades out

// Initialize echo effect
// pot1-pot4 are "potentiometer" values from 0.0 to 1.0
// (simulating physical knobs on a guitar pedal)
static inline void echo_init(float pot1, float pot2, float pot3, float pot4)
{
    effect_set_delay(pot1 * 1000);   // pot1: delay 0-1000ms
    effect_set_lfo_ms(pot3 * 4);     // pot3: subtle LFO modulation 0-4ms
    effect_set_feedback(pot4);        // pot4: feedback amount 0-100%
    
    // Print settings to stderr for debugging
    fprintf(stderr, "echo:");
    fprintf(stderr, " delay=%g ms", pot1 * 1000);
    fprintf(stderr, " lfo=%g ms", pot3 * 4);
    fprintf(stderr, " feedback=%g\n", pot4);
}

// Process one sample through the echo effect
static inline float echo_step(float in)
{
    // Calculate current delay (add 1 to ensure we're always looking back)
    float d = 1 + effect_delay;
    float out;
    
    // Read the delayed sample
    out = sample_array_read(d);
    
    // Write current input mixed with feedback to the delay buffer
    // The limit_value prevents the feedback from growing unbounded
    sample_array_write(limit_value(in + out * effect_feedback));
    
    // Output is mix of dry (input) and wet (delayed) signal
    return (in + out) / 2;
}