# Audio-Processing
MCUExpresso hosted c code for digital signal processing on the NXP iMX RT 1062 processor.  Audio is sampled via a WM8960 codec using I2S/SAI and DMA. Reads five potentiometers using ETC_ADC with DMA.

Effects include the following
- Delay Based Effects
    - Echo
    - Flanger (alpha)
    - Phaser  (alpha)
- Filters
    - Peaking EQ Filter
    - Comb Filter
    - All Pass
    - Low Pass
- Dynamic Range Controls
    - Noise Gate*
    - Compressor
    - Expander
    - Limiter
- Reverberation    
    - Schroeder Reverb
    - Freeverb
- Miscellaneous    
    - Pitch Shifting
    - Tremolo*
    - Overdrive*
    - FFT (alpha)
- Effects Components
    - Low Frequency Oscillator
    - Mixer
    - Variable Delay
    - Circular Buffer
    - State Variable Filter
    

*The code for the Tremolo, Noise Gate, and Overdrive effects is based on code provided by 
Philip Salmony on his Phil's Lab YouTube channel, and is provided with his permission.
