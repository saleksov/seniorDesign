// // DO NOT TOUCH vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// PA13 PA14
// PC14 PC15
// In particular, you should not change PA13, PA14, PC14, or PC15 since these
// are used for an External Oscillator, Debugging, and Programming interfaces.
// // DO NOT TOUCH ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#include "stm32f0xx.h"
#include <stdint.h>

// From Justin
#include <stdio.h>
#include "fifo.h"
#include "tty.h"
#include "commands.h"
#include "lcd.h"

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
uint8_t bufferSettings0 = 0;
uint8_t bufferSettings1 = 0;

uint8_t note0 = 60;
uint8_t note1 = 62;
uint8_t note2 = 64;
uint8_t note3 = 65;
uint8_t note4 = 67;
uint8_t note5 = 69;
uint8_t note6 = 71;
uint8_t note7 = 72;

uint8_t BPM = 60;
uint8_t Mstatus = 0;

// From Justin
char octaves[7] = {'1','2','3', '4', '5', '6', '7'};
char modes[5] = {'A', 'B', 'C', 'D', 'E'};
char metros[4] = {'4', '3', '5', '7'};

uint8_t modeidx = 0;
uint8_t metroidx = 0;

void inita();
void initc();
void strings(void);
void buttons_octaves(void);

void setOctave(void);
void buttons_settings(void);
void setup_serial(void);
void sendMIDI(uint8_t channel, uint8_t note, uint8_t velocity);

// Metronome
void init_tim6_metronome(void);
void TIM6_DAC_IRQHandler(void);
void set_bpm(void);
void toggleMetronome(void);

// From Justin
void disp_begin();
void disp_harp();
void init_clear();
void init_lcd_spi();
void dispUI(char octave, char metro, char mode);

void init_spi1_slow (void);
void init_lcd_spi (void);

void internal_clock(void);

int main(void)
{

  internal_clock();
  inita();
  initc();
  setup_serial();
  init_tim6_metronome();

  // From Justin

  init_lcd_spi();
  LCD_Setup();
  disp_harp();

  toggleMetronome();

  while (1)
  {
    strings();
    buttons_octaves();
    if ((GPIOC->IDR) & (1))
    {
      buttons_settings();
    }
  }

  return 0;
}

void buttons_settings(void)
{
  bufferSettings0 = 1;
  while ((GPIOC->IDR) & (1))
    bufferSettings0 = 1;

  int loop = 1;
  while (loop)
  {
    if ((GPIOC->IDR) & (1)){
      bufferSettings0 = 0;
    }
    else if (bufferSettings0 == 0)
    {
      bufferSettings0 = 1;
      loop = 0;
    }

    if ((GPIOC->IDR) & (1 << 2))
      bufferOctave0 = 0;
    else if (bufferOctave0 == 0)
    {
      bufferOctave0 = 1;
      BPM += 10;
      if ((BPM != 0) && (Mstatus == 0))
      {
        Mstatus = 1;
        toggleMetronome();
      }
      if (BPM > 220)
        BPM = 220; 
      set_bpm();
    }

    if ((GPIOC->IDR) & (1 << 3))
      bufferOctave1 = 0;
    else if (bufferOctave1 == 0)
    {
      bufferOctave1 = 1;
      if (BPM != 0)
        BPM -= 10;
      if (BPM == 0)
      {
        Mstatus = 0;
        toggleMetronome();
      }
      set_bpm();
    }
  }

  bufferOctave0 = 0;
  bufferOctave1 = 0;
}

// METRONOME

void toggleMetronome(void)
{
  if (Mstatus)
  {
    TIM6->CNT = 0;              // reset counter
    TIM6->SR &= ~TIM_SR_UIF;    // clear flag
    TIM6->CR1 |= TIM_CR1_CEN;   // enable counter
    dispUI(octaves[octave],'Y', modes[modeidx]);
  }
  else{
    TIM6->CR1 &= ~TIM_CR1_CEN;
    dispUI(octaves[octave],'N', modes[modeidx]);
  }
}

void init_tim6_metronome(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

  TIM6->PSC = 48000 - 1;
  TIM6->ARR = (60000 / BPM) - 1;

  TIM6->DIER |= TIM_DIER_UIE;
  TIM6->CR1  |= TIM_CR1_CEN;

  NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

// METRONOME
void TIM6_DAC_IRQHandler(void) {
  TIM6->SR &= ~TIM_SR_UIF; // clear interrupt flag
  sendMIDI(0x90, 84, 100); // Note On      
}

// Metronome
void set_bpm(void) {
    if (Mstatus == 1)
    {
      Mstatus = 0;
      toggleMetronome();
      TIM6->ARR = (60000 / BPM) - 1;
      Mstatus = 1;
      toggleMetronome();
    }
      
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
  GPIOC->MODER &= 0xFFFFFF00;
  GPIOC->PUPDR &= 0xFFFFFF55;
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

void buttons_octaves()
{
  if ((GPIOC->IDR) & (1 << 2))
    bufferOctave0 = 0;
  else if (bufferOctave0 == 0)
  {
    bufferOctave0 = 1;
    if (octave < 7) // max is 7
    {
      octave++;
      setOctave();
      dispUI(octaves[octave], metros[metroidx], modes[modeidx]);
    }
  }
  if ((GPIOC->IDR) & (1 << 3))
    bufferOctave1 = 0;
  else if (bufferOctave1 == 0)
  {
    bufferOctave1 = 1;
    if (octave > 1) // min is 1
    {
      octave--;
      setOctave();
      dispUI(octaves[octave], metros[metroidx], modes[modeidx]);
    }
  }
}

void setOctave(void)
{
  switch (octave)
  {
  case 1:
    note0 = 24;
    note1 = 26;
    note2 = 28;
    note3 = 29;
    note4 = 31;
    note5 = 33;
    note6 = 35;
    note7 = 36;
  break;
  case 2:
    note0 = 36;
    note1 = 38;
    note2 = 40;
    note3 = 41;
    note4 = 43;
    note5 = 45;
    note6 = 47;
    note7 = 48;
  break;
  case 3:
    note0 = 48;
    note1 = 50;
    note2 = 52;
    note3 = 53;
    note4 = 55;
    note5 = 57;
    note6 = 59;
    note7 = 60;
  break;
  case 4:
    note0 = 60;
    note1 = 62;
    note2 = 64;
    note3 = 65;
    note4 = 67;
    note5 = 69;
    note6 = 71;
    note7 = 72;
  break;
  case 5:
    note0 = 72;
    note1 = 74;
    note2 = 76;
    note3 = 77;
    note4 = 79;
    note5 = 81;
    note6 = 83;
    note7 = 84;
  break;
  case 6:
    note0 = 84; 
    note1 = 86;
    note2 = 88;
    note3 = 89;
    note4 = 91;
    note5 = 93;
    note6 = 95;
    note7 = 96;
  break;
  case 7:
    note0 = 96;
    note1 = 98;
    note2 = 100;
    note3 = 101;
    note4 = 103;
    note5 = 105;
    note6 = 107;
    note7 = 108;
  break;
  
// Default:
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

//spi 1 for display
void init_spi1_slow (void){
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB -> MODER &= ~0x00000fc0;
    GPIOB -> MODER |= 0x00000a80;
    GPIOB -> AFR[0] &= ~(GPIO_AFRL_AFRL3 | GPIO_AFRL_AFRL4 | GPIO_AFRL_AFRL5);
    RCC -> APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1 -> CR1 &= ~SPI_CR1_SPE;
    SPI1->CR1 &= ~(SPI_CR1_BR);
    SPI1 -> CR1 |= SPI_CR1_MSTR;
    SPI1 -> CR2 |= SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;
    SPI1 -> CR2 &= ~SPI_CR2_DS_3;
    SPI1 -> CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    SPI1 -> CR2 |= SPI_CR2_FRXTH;
    SPI1 -> CR1 |= SPI_CR1_SPE;
}

void init_lcd_spi (void){
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB -> MODER &= ~0x30c30000;
    GPIOB -> MODER |= 0x10410000;
    init_spi1_slow();
                                
}

