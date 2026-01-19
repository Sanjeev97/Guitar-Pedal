// gensin.c - Generate sine wave lookup table at compile time
// This program outputs C code that defines the table

#include <math.h>
#include <stdio.h>

// Number of steps in our quarter-wave table
// 256 gives good precision for audio (about 4.5 digits)
#define STEP_SHIFT 9
#define STEPS (1 << STEP_SHIFT) // 2^8 = 256, using bit operation
// 1 left shift 8 == 2^8

int main(int argc, char **argv)
{
    // Output the shift constant so other code knows the table size
    printf("#define QUARTER_SINE_STEP_SHIFT %d\n", STEP_SHIFT);
    
    // Start the array definition
    printf("const float quarter_sin[] = {");

    // Generate 257 values (0 to 256 inclusive, for interpolation)
    for (int i = 0; i < STEPS+1; i++) {
        // Add newline and tab every 4 values for readability
        printf("%s%+.8ff,",
            !(i & 3) ? "\n\t" : " ",       // Newline every 4 values
            sin(i*M_PI/STEPS/2));          // sin(0) to sin(π/2)
    }
    
    // Final value (sin(π/2) = 1.0) for interpolation at the boundary
    printf(" %+.8ff\n};\n", 1.0);
    
    return 0;    

}