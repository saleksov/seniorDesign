// // DO NOT TOUCH vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// PA13 PA14
// PC14 PC15
// In particular, you should not change PA13, PA14, PC14, or PC15 since these
// are used for an External Oscillator, Debugging, and Programming interfaces.
// // DO NOT TOUCH ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>

// From Justin
#include <stdio.h>
#include "fifo.h"
#include "tty.h"
#include "commands.h"
#include "lcd.h"
#include "sendmidicode.h"
#include "handleinput.h"
#include "metronome.h"
#include "initpins.h"
#include "songplayer.h"

uint8_t bufferString00 = 0;
uint8_t bufferString01 = 0;
uint8_t bufferString02 = 0;
uint8_t bufferString03 = 0;
uint8_t bufferString04 = 0;
uint8_t bufferString05 = 0;
uint8_t bufferString06 = 0;
uint8_t bufferString07 = 0;
uint8_t bufferString08 = 0;
uint8_t bufferString09 = 0;
uint8_t bufferString10 = 0;
uint8_t bufferString11 = 0;

uint8_t note00 = 60;
uint8_t note01 = 61;
uint8_t note02 = 62;
uint8_t note03 = 63;
uint8_t note04 = 64;
uint8_t note05 = 65;
uint8_t note06 = 66;
uint8_t note07 = 67;
uint8_t note08 = 68;
uint8_t note09 = 69;
uint8_t note10 = 70;
uint8_t note11 = 71;

uint8_t octave = 3;
uint8_t bufferOctave0 = 0;
uint8_t bufferOctave1 = 0;
uint8_t bufferSettings0 = 0;
uint8_t bufferSettings1 = 0;

uint8_t BPM = 120;
uint8_t Mstatus = 0;
uint8_t metroSig = 1;

uint8_t metroSigCounter = 0;
uint8_t Pedal = 1;

// From Justin
char octaves[7] = {'1','2','3', '4', '5', '6', '7'};
char modes[5] = {'A', 'B', 'C', 'D', 'E'};
char metros[4] = {'4', '3', '5', '7'};

uint8_t modeidx = 0;
uint8_t metroidx = 0;

// From Justin
void disp_begin();
void disp_harp();
void init_clear();
void init_lcd_spi();
void dispUI(char octave, char metro, char mode);

void internal_clock(void);

int main(void)
{

  internal_clock();
  inita();
  initc();
  setup_serial();
  init_tim6_metronome();

  // From Justin

  //init_lcd_spi();
  //LCD_Setup();
  //disp_harp();

  //toggleMetronome();

  //  while (!(USART5->ISR & USART_ISR_TXE))
  //    ; // Wait until TX buffer empty
  //  USART5->TDR = 0xC0;
  //  while (!(USART5->ISR & USART_ISR_TXE))
  //    ; // Wait until TX buffer empty
  //  USART5->TDR = 128-1;

  while (1)
  {
    strings();
  
    songPlayer();
    
  //   //buttons_octaves();
  //   //if ((GPIOC->IDR) & (1))
  //   //{
  //    // buttons_settings();
  //   //}
  }

  return 0;
}
