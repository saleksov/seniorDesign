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

#define STRING0  0
#define STRING1  1
#define STRING2  2
#define STRING3  3
#define STRING4  4
#define STRING5  5
#define STRING6  6
#define STRING7  7
#define STRING8  8
#define STRING9  9
#define STRING10 10
#define STRING11 11

uint8_t buffer0;
uint8_t buffer1;
uint8_t buffer2;
uint8_t buffer3;

typedef struct ActiveNotes {
    int SomeData;
    struct ActiveNotes *next;
    struct ActiveNotes *back;
} ActiveNotes;

void initb();
void strings(void);
void setup_serial(void);
void sendMIDI(uint8_t  channel, uint8_t  note, uint8_t  velocity);

void internal_clock(void);

int main(void) {
    
  internal_clock();
  initb();
  setup_serial();

  while(1) 
  {
    strings();
  }
    
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
  GPIOB -> MODER &= 0x00000000;
  
  // set 8-11 to output for Demo
  GPIOB -> MODER |= GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0 | GPIO_MODER_MODER11_0;

  // PUPDR
  // 00: No pull-up, pull-down
  // 01: Pull-up
  // 10: Pull-down
  // 11: Reserved

  // CURRENTLY SET TO PULL-DOWN FOR BUTTONS
  // WHEN STRINGS REPLACE BUTTONS SET TO PULL-UP
  GPIOB -> PUPDR &= 0xFFFFFF55;
}

// Strings
// Example of 1 strign what i should be
// if true -> Trigger note generation
// Have logic in note generation for if true -> detect 0 before allowed to be true again.

// Create linked list struct for playing notes

void strings(void) 
{
  // String 0 - Output is B11
  if ((GPIOB -> IDR) & (1 << STRING0))  buffer0 = 0;             
  else if (buffer0 == 0)                {sendMIDI(0,60,100); buffer0 = 1;}
  if ((GPIOB -> IDR) & (1 << STRING1))  buffer1 = 0;             
  else if (buffer1 == 0)                {sendMIDI(0,63,100); buffer1 = 1;}
  if ((GPIOB -> IDR) & (1 << STRING2))  buffer2 = 0;             
  else if (buffer2 == 0)                {sendMIDI(0,64,100); buffer2 = 1;}
  if ((GPIOB -> IDR) & (1 << STRING3))  buffer3 = 0;             
  else if (buffer3 == 0)                {sendMIDI(0,67,100); buffer3 = 1;}
}

void sendMIDI(uint8_t  channel, uint8_t  note, uint8_t  velocity)
{
    while (!(USART5->ISR & USART_ISR_TXE));  // Wait until TX buffer empty
    USART5->TDR = 0x90 | (channel & 0x0F); 
    while (!(USART5->ISR & USART_ISR_TXE));  // Wait until TX buffer empty
    USART5->TDR = note; 
    while (!(USART5->ISR & USART_ISR_TXE));  // Wait until TX buffer empty
    USART5->TDR = velocity; 
}

void setup_serial(void)
{
    RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOC -> MODER &= ~(GPIO_MODER_MODER12);
    GPIOC -> MODER |= GPIO_MODER_MODER12_1;

    // SET TO HIGH SPEED?
    // GPIOC -> OSPEEDR |= GPIO_OSPEEDR_OSPEEDR12_1 | GPIO_OSPEEDR_OSPEEDR12_0;

    GPIOC -> AFR[1] &= ~GPIO_AFRH_AFRH4;
    GPIOC -> AFR[1] |= 1 << (1 + GPIO_AFRH_AFRH4_Pos);

    // RCC -> AHBENR |= RCC_AHBENR_GPIODEN;     // LAB MANUAL HAS RE, DO WE NEED? RECIVER
    // GPIOD -> MODER &= ~GPIO_MODER_MODER2;
    // GPIOD -> MODER |= GPIO_MODER_MODER2_1;

    // GPIOD -> AFR[0] &= ~GPIO_AFRL_AFRL2;
    // GPIOD -> AFR[0] |= 1 << (1 + GPIO_AFRL_AFRL2_Pos);

    RCC -> APB1ENR |= RCC_APB1ENR_USART5EN;

    USART5->CR1 &= ~USART_CR1_UE; // Dissable UE, set UE = 0

    USART5->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); // Set word size of 8 bits

    USART5->CR1 &= ~USART_CR1_OVER8; // SET TO 16X OVERSAMPLING

    USART5->CR1 &= ~USART_CR1_PCE; // PARITY DISSABLED

    USART5->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1); // 1 STOP BIT

    // bAUD RATE 31250 IN BRR REGISTER
    // USART5 -> BRR = 417; THIS IS 115200 BAUD IN LAB
    USART5 -> BRR = 1536;
    
    USART5->CR1 |= USART_CR1_TE;
    // USART5->CR1 |= USART_CR1_RE; // LAB MANUAL HAS RE, DO WE NEED? rECIVER

    USART5->CR1 |= USART_CR1_UE; // Enable UE, set UE = 1

    while (!((USART5 -> ISR) & (USART_ISR_TEACK 
                          //  | USART_ISR_REACK // DO WE NEED RE?
                                                  )));
}

// QUESTIONS?
// HOW DO I SET TO 31250 BAUD?
// DO I SET PC12 TO HIGH SPEED?