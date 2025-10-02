// /*
// WORK IN PROGRESS
// / TBD

// 0. Relearn micro takes input and output :(

// 1. Create a send_byte(BYTE) function that sets ODR of GPIO MCU pins 
// to send a byte to a shift register or the Synth Chip, depending on hardware
// this needs to be serial or parallel -> Depending on SYNTH CHIP

// 2. Create a send_2synth(BUTE, BYTE, BYTE) function that uses send_byte to send 3 BYTES
// to the Synth chip to play a coresponding note

// 3. Create a genSound(BOOL_playNewNote, ifTrue BYTE_whatNote) function that maintains the 
// current notes being played plus adds new notes to be played specified in parametars

//     data structure maintaned by genSound: -> Linked list of currently played notes
//     allocate and free memory as needed, one note-> 1 link,

// 4. Within the Foor Loop of 12 strings, add 2 for the buttons on screen for change of octave 
// to avoid need of interrupts

// 5. Add interrupt for configuring/ setting Metronome, Mode and Looper function

// 6. LCD Code ???

// // DO NOT TOUCH vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// PA13 PA14
// PC14 PC15
// In particular, you should not change PA13, PA14, PC14, or PC15 since these 
// are used for an External Oscillator, Debugging, and Programming interfaces.
// // DO NOT TOUCH ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



// MUST READ

// CURRENT ITERATION PB 0-3 ARE INPUTS TO BUTTONS
                  // PB 4-7 ARE OUTPUTS TO LEDS
// SETN IS USED FOR READIBUILITY -> CHANGE TO HARDCODING FOR SPEED LATER

// INPUTS CURRENTLY SET TO PULL-DOWN -> Input is HIGH when button pressed
// WHEN STRINGS ARE ADDED THIS NEEDS TO BE SET TO PULL-UP AS LDR WILL PULL LOW
// AND GENERATE AN INPUT 
