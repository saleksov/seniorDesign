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


#include "stm32f0xx.h"
#include <stdint.h>

void initb();
void setn(int32_t pin_num, int32_t val);
int32_t readpin(int32_t pin_num);
void buttons(void);

void internal_clock(void);

int main(void) {
    
  internal_clock();
  initb();

  while(1) 
  {
    buttons();
  }

    for(;;);
    
    return 0;
}

// inita() // NEVER TOUCH // PA13 PA14
// initc() // NEVER TOUCH // PC14 PC15

void initb() 

{
  // MODER
  // 00: Input (reset state)
  // 01: General purpose output mode
  // 10: Alternate function mode
  // 11: Analog mode

  // Enable clock for GPIOB
  RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;

  // SET 0-3 to INPUT and RESET 4-7
  GPIOB -> MODER &= 0xFFFF0000;
  
  // set 4-7 to output
  GPIOB -> MODER |= 0x00005500;

  // PUPDR
  // 00: No pull-up, pull-down
  // 01: Pull-up
  // 10: Pull-down
  // 11: Reserved

  // CURRENTLY SET TO PULL-DOWN FOR BUTTONS
  // WHEN STRINGS REPLACE BUTTONS SET TO PULL-UP
  GPIOB -> PUPDR &= 0xFFFFFFAA;

}

// TEMP FUNCTION FOR READABILITY AND TESTING INPUTS
// REPLACE LATER WITH HARDCODING FOR EACH STRING
void setn(int32_t pin_num, int32_t val) 
{  
  if (val)
    GPIOB -> BSRR |= 1<<pin_num;
  else
    GPIOB -> BRR |= 1<<pin_num;
}

// READING AN INPUT
int32_t readpin(int32_t pin_num) 
{
  if ((GPIOB -> IDR) & (1 << pin_num))
    return 1;
  return 0;
}

void buttons(void) 
{
  // set value of pin 4 to the current input of pin 0 and so on 
  setn(4, readpin(0));
  setn(5, readpin(1));
  setn(6, readpin(2));
  setn(7, readpin(3));
}