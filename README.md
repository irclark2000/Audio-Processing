# Audio-Processing
MCUXpresso hosted c code for digital signal processing on the IMX RT 1062 processor.  Audio is sampled via a WM8960 codec using I2S/SAI and DMA. Reads five potentiometers using ETC_ADC with DMA.
Effects include the following
1. Shroeder Reverb
2. Freeverb
3. Pitch Shifting
4. Peaking EQ Filter
5. Tremolo
6. Noise Gate
7. Overdrive
8. Expander
9. Compressor
10. Limiter


The code for the Tremolo, Noise Gate, and Overdrive effects are based on code provided by 
Philip Salmony on his Phil's Lab YouTube channel, and is provided with his permission.
