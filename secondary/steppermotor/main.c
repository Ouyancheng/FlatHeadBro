#include "gpio.h"
#include "kernelsdk.h"

enum note_idx {
    C  = 0,
    Cs = 1,
    D  = 2,
    Ds = 3, 
    E  = 4, 
    F  = 5, 
    Fs = 6,
    G  = 7, 
    Gs = 8, 
    A  = 9,
    As = 10,
    B  = 11,
};

float frequency_table[9][12] = {
    //     C      C#     D      D#     E      F      F#     G      G#     A      A#     B
    [4] = {261.6, 277.2, 293.7, 311.1, 329.6, 349.2, 370.0, 392.0, 415.3, 440.0, 466.2, 493.9}
};

void build_frequency_table(void) {
    for (int octave = 3; octave >= 0; --octave) {
        for (int i = 0; i < 12; ++i) {
            frequency_table[octave][i] = frequency_table[octave+1][i] / 2.0f;
        }
    }
    for (int octave = 5; octave < 9; ++octave) {
        for (int i = 0; i < 12; ++i) {
            frequency_table[octave][i] = frequency_table[octave-1][i] * 2.0f;
        }
    }
}

void play_frequency(float frequency, int duration_us) {
    float fperiod = (1000000.0f/frequency / 2.0f);
    int period_us = (int)fperiod;
    int pulses = duration_us / (period_us * 2);
    // STEP -- min 444, 460, 475 us, depends on condition
    while (pulses --> 0) {
        gpio_write(gpio_pe, 16, 1);
        delay_us(period_us);
        gpio_write(gpio_pe, 16, 0);
        delay_us(period_us);
    }
}

void main(void) {
    uart0_ctl = uart_init(0, 1); 
    printf("hello!\n");
    build_frequency_table();
    gpio_set_config(gpio_pe, 16, gpio_config_output);
    gpio_set_config(gpio_pe, 12, gpio_config_output);
    gpio_set_pull(gpio_pe, 16, gpio_pull_down);
    gpio_set_pull(gpio_pe, 12, gpio_pull_down);
    // DIR 
    gpio_write(gpio_pe, 12, 0);
    gpio_write(gpio_pe, 16, 0);
    delay_us(1000);
    int max_octave = 9;
    enum note_idx ntable[7] = {C, D, E, F, G, A, B};
    for (int octave = 0; octave < max_octave; ++octave) {
        for (int note = 0; note < 7; ++note) {
            play_frequency(frequency_table[octave][(unsigned)ntable[note]], 500000);
        }
    }
    
    return;
}




