#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>

#include <stdio.h>
#include "fifo.h"
#include "tty.h"
#include "lcd.h"
#include "sendmidicode.h"
#include "handleinput.h"
#include "metronome.h"
#include "initpins.h"
#include "songplayer.h"
#include "support.h"
#include "globalvar.h"
#include "UI_Commands.h"

uint8_t bufferString00 = 1;
uint8_t bufferString01 = 1;
uint8_t bufferString02 = 1;
uint8_t bufferString03 = 1;
uint8_t bufferString04 = 1;
uint8_t bufferString05 = 1;
uint8_t bufferString06 = 1;
uint8_t bufferString07 = 1;
uint8_t bufferString08 = 1;
uint8_t bufferString09 = 1;
uint8_t bufferString10 = 1;
uint8_t bufferString11 = 1;

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

uint8_t octave = 60;
uint8_t octaveStep = 12;
uint8_t bufferButton0 = 1;
uint8_t bufferButton1 = 1;
uint8_t bufferButton2 = 1;
uint8_t bufferButton3 = 1;

uint8_t BPM = 60;
uint8_t Mstatus = 0;
uint8_t metroSig = 1;

uint8_t metroSigCounter = 0;

// From Justin
char octaves[7] = {'1','2','3', '4', '5', '6', '7'};
char modes[5] = {'A', 'B', 'C', 'D', 'E'};
char metros[4] = {'4', '3', '5', '7'};

uint8_t modeidx = 0;
uint8_t metroidx = 0;

// Song Player
volatile uint32_t tim7Ticks = 0;
volatile uint8_t songPlaying = 0;
uint32_t songIndex = 0;
uint32_t next_tick = 0;
uint8_t song_done = 0;

volatile uint8_t currentSong = Sweeden;

uint8_t col;

// Volume

uint8_t strings_volume = 100;
uint8_t songPlayer_volume = 50;
uint8_t metronome_volume = 100;

// From Justin
void disp_begin();
void disp_harp_Menu1();
void init_clear();
void init_lcd_spi();
//void dispUI(char octave, char metro, char mode);

void internal_clock(void);

int main(void)
{
  internal_clock();
  inita();
  initc();
  setup_serial();
  init_tim6_metronome();
  init_tim7_songDelay();
  init_tim14_keypad();

  // From Justin

  init_lcd_spi();
  LCD_Setup();
  disp_harp_Menu1();

  //char temp;
  
  changeInstrument(0,4);

// Sweeden      1 -> Index  0
// DryHands     2 -> Index  322
// Pokemon      3 -> Index  1119
// Sweet Cgild  4 -> Index  7741
// N in Paris   5 -> Index  9225
// Cat          6 -> Index  9354  
// Mario        7 -> Index  27483
// Snowman      8 -> Index  31732 
// Mocking Bird 9 -> Index  34573 
// I Wonder     0 -> index  39131
  
  currentSong = '0';
  startSong();

  // get_keypress();
  // sendMIDI(0,60,100); // Note Off test
  // while (!(USART5->ISR & USART_ISR_TXE))
  //   ; // Wait until TX buffer empty
  // USART5->TDR = 0x90 | (0 & 0x0F);

  while (1)
  {                                                                       
    strings();
  
    songPlayer();
  
    buttons();

    settings();
  }

  return 0;
}
