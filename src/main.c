// // DO NOT TOUCH vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// PA13 PA14
// PC14 PC15
// In particular, you should not change PA13, PA14, PC14, or PC15 since these
// are used for an External Oscillator, Debugging, and Programming interfaces.
// // DO NOT TOUCH ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#include "stm32f0xx.h"
#include <stdint.h>

uint8_t buffer0 = 0;
uint8_t buffer1 = 0;
uint8_t buffer2 = 0;
uint8_t buffer3 = 0;
uint8_t buffer4 = 0;
uint8_t buffer5 = 0;
uint8_t buffer6 = 0;
uint8_t buffer7 = 0;

uint8_t octave = 3;
uint8_t bufferOctave0 = 0;
uint8_t bufferOctave1 = 0;

uint8_t note0 = 60;
uint8_t note1 = 62;
uint8_t note2 = 64;
uint8_t note3 = 65;
uint8_t note4 = 67;
uint8_t note5 = 69;
uint8_t note6 = 71;
uint8_t note7 = 72;

void inita();
void initc();
void strings(void);
void buttons(void);
void setup_serial(void);
void sendMIDI(uint8_t channel, uint8_t note, uint8_t velocity);

void internal_clock(void);

int main(void)
{

  internal_clock();
  inita();
  initc();
  setup_serial();

  while (1)
  {
    strings();
    buttons();
  }

  return 0;
}

void inita()

{
  // MODER
  // 00: Input (reset state)
  // 01: General purpose output mode
  // 10: Alternate function mode
  // 11: Analog mode

  // Enable clock for GPIOB
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

  GPIOA->MODER &= 0xFFFF0000;

  // PUPDR
  // 00: No pull-up, pull-down
  // 01: Pull-up
  // 10: Pull-down
  // 11: Reserved

  // SET TO PULL-UP
  GPIOA->PUPDR &= 0xFFFF5555;
}

void initc()
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIOC->MODER &= 0xFFFFFF0F;
  GPIOC->PUPDR &= 0xFFFFFF5F;
}

void strings(void)
{
  // String 0 - Output is B11
  if ((GPIOA->IDR) & (1 << 0))
    buffer0 = 0;
  else if (buffer0 == 0)
  {
    sendMIDI(0, note0, 100);
    buffer0 = 1;
  }

  if ((GPIOA->IDR) & (1 << 1))
    buffer1 = 0;
  else if (buffer1 == 0)
  {
    sendMIDI(0, note1, 100);
    buffer1 = 1;
  }

  if ((GPIOA->IDR) & (1 << 2))
    buffer2 = 0;
  else if (buffer2 == 0)
  {
    sendMIDI(0, note2, 100);
    buffer2 = 1;
  }

  if ((GPIOA->IDR) & (1 << 3))
    buffer3 = 0;
  else if (buffer3 == 0)
  {
    sendMIDI(0, note3, 100);
    buffer3 = 1;
  }

  if ((GPIOA->IDR) & (1 << 4))
    buffer4 = 0;
  else if (buffer4 == 0)
  {
    sendMIDI(0, note4, 100);
    buffer4 = 1;
  }

  if ((GPIOA->IDR) & (1 << 5))
    buffer5 = 0;
  else if (buffer5 == 0)
  {
    sendMIDI(0, note5, 100);
    buffer5 = 1;
  }

  if ((GPIOA->IDR) & (1 << 6))
    buffer6 = 0;
  else if (buffer6 == 0)
  {
    sendMIDI(0, note6, 100);
    buffer6 = 1;
  }

  if ((GPIOA->IDR) & (1 << 7))
    buffer7 = 0;
  else if (buffer7 == 0)
  {
    sendMIDI(0, note7, 100);
    buffer7 = 1;
  }
}

void buttons()
{
  if ((GPIOC->IDR) & (1 << 2))
    bufferOctave0 = 0;
  else if (bufferOctave0 == 0)
  {
    bufferOctave0 = 1;
    if (octave < 7)
    {
      octave++;
      note0 = (octave + 2) * 12;
      note1 = (octave + 2) * 12 + 2;
      note2 = (octave + 2) * 12 + 4;
      note3 = (octave + 2) * 12 + 5;
      note4 = (octave + 2) * 12 + 7;
      note5 = (octave + 2) * 12 + 9;
      note6 = (octave + 2) * 12 + 11;
      note7 = (octave + 2) * 12 + 12;
    }
  }
  if ((GPIOC->IDR) & (1 << 3))
    bufferOctave1 = 0;
  else if (bufferOctave1 == 0)
  {
    bufferOctave1 = 1;
    if (octave > 0)
    {
      octave--;
      note0 = (octave + 2) * 12;
      note1 = (octave + 2) * 12 + 2;
      note2 = (octave + 2) * 12 + 4;
      note3 = (octave + 2) * 12 + 5;
      note4 = (octave + 2) * 12 + 6;
      note5 = (octave + 2) * 12 + 7;
      note6 = (octave + 2) * 12 + 9;
      note7 = (octave + 2) * 12 + 11;
    }
  }
}

void sendMIDI(uint8_t channel, uint8_t note, uint8_t velocity)
{
  while (!(USART5->ISR & USART_ISR_TXE))
    ; // Wait until TX buffer empty
  USART5->TDR = 0x90 | (channel & 0x0F);
  while (!(USART5->ISR & USART_ISR_TXE))
    ; // Wait until TX buffer empty
  USART5->TDR = note;
  while (!(USART5->ISR & USART_ISR_TXE))
    ; // Wait until TX buffer empty
  USART5->TDR = velocity;
}

void setup_serial(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

  GPIOC->MODER &= ~(GPIO_MODER_MODER12);
  GPIOC->MODER |= GPIO_MODER_MODER12_1;

  GPIOC->AFR[1] &= ~GPIO_AFRH_AFRH4;
  GPIOC->AFR[1] |= 1 << (1 + GPIO_AFRH_AFRH4_Pos);

  RCC->APB1ENR |= RCC_APB1ENR_USART5EN;

  USART5->CR1 &= ~USART_CR1_UE; // Dissable UE, set UE = 0

  USART5->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); // Set word size of 8 bits

  USART5->CR1 &= ~USART_CR1_OVER8; // SET TO 16X OVERSAMPLING

  USART5->CR1 &= ~USART_CR1_PCE; // PARITY DISSABLED

  USART5->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1); // 1 STOP BIT

  USART5->BRR = 1536;

  USART5->CR1 |= USART_CR1_TE;

  USART5->CR1 |= USART_CR1_UE; // Enable UE, set UE = 1

  while (!((USART5->ISR) & (USART_ISR_TEACK
                            //  | USART_ISR_REACK // DO WE NEED RE?
                            )))
    ;
}