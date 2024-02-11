# Generator DTMF
### Destination
The aim of the project was to create a DTMF generator in order to learn about tone control and STM32 microcontrollers. The project has an analog part consisting of an opamp in a combiner configuration.
### Description
The 4x4 keyboard simulates the buttons on your phone, by clicking a specific one, two sine waves are produced using a DAC and then summed by an amplifier and the speaker module produces the appropriate sound. the code is written in a non-blocking manner using timers and interrupts.
### Technologies
- STM32CubeIDE
- C language
### Hardware
- microconteoller STM32l432kc
- amplifier lm741c
- keypad 4x4
- speaker module
