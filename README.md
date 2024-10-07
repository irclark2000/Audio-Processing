# Audio-Processing
MCUExpresso hosted c code for digital signal processing on the NXP iMX RT 1062 processor.  Audio is sampled via a WM8960 codec using I2S/SAI and DMA. Reads five potentiometers using ETC_ADC with DMA.

Effects include the following
- Delay Based Effects
    - Echo
    - Flanger (alpha)
    - Vibrato
    - Chorus (alpha)
- Filters
    - Peaking EQ Filter
    - Comb Filter
    - All Pass
    - Low Pass
    - Second Order All Pass Filter
- Dynamic Range Controls
    - Noise Gate*
    - Compressor
    - Expander
    - Limiter
- Reverberation
    - Schroeder Reverb
    - Freeverb
- Delay Base Effects
    - Phaser (under development)
- Miscellaneous
    - Pitch Shifting
    - Tremolo*
    - Overdrive*
    - FFT (alpha)
- Effects Components
    - Low Frequency Oscillator
    - Mixer
    - Variable Delay (fractional)
    - Circular Buffer
    - State Variable Filter
    - Variable Band Pass/Stop Filter
    - First Order All Pass Filter
    

*The code for the Tremolo, Noise Gate, and Overdrive effects is based on code provided by 
Philip Salmony on his Phil's Lab YouTube channel, and is provided with his permission.

