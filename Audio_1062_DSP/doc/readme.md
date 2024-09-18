Overview
========
A single sai instance record the audio data from input and playbacks the audio data.
Effects can be applied to the audio input data to be played back.
SDK version
===========
- Version: 2.16.000

Toolchain supported
===================
- IAR embedded Workbench  9.60.1
- Keil MDK  5.39.0
- GCC ARM Embedded  13.2.1
- MCUXpresso  11.10.0

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKB board
- Personal Computer
- Headphone(OMTP standard) for monitoring output

Board settings
==============
For on board codec (WM8960 codec):
2.Make sure J41 is installed

Prepare
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
Steps for WM8960:
4. Insert the headphones into the headphone jack on MIMXRT1060-EVKB board (J34).

Running the program
================
Note: This demo uses both headphone mic and board main mic(P1) as input source of WM8960.  But currenly only the 
board mike input is processed and output as both the left and right channels.  It is possible to use white noise as the input.
See process.c to see how to do this. When run displays the following message:
~~~~~~~~~~~~~~~~~~~
SAI EDMA ping pong buffer started!
~~~~~~~~~~~~~~~~~~~

