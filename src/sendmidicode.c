#include "sendmidicode.h"

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

void sendMIDIstrings(uint8_t channel, uint8_t note, uint8_t velocity)
{
  sendMIDI(channel, note, velocity);

  if (Pedal == FLANGER)
  {
    uint32_t i = 0;
    while(i < FLANGERDELAY)
    i++;
    sendMIDI(channel+10, note, velocity);
  }
}


void setOctave(void)
{
  void; 

  // switch (octave)
  // {
  // case 1:
  //   note0 = 24;
  //   note1 = 26;
  //   note2 = 28;
  //   note3 = 29;
  //   note4 = 31;
  //   note5 = 33;
  //   note6 = 35;
  //   note7 = 36;
  // break;
  // case 2:
  //   note0 = 36;
  //   note1 = 38;
  //   note2 = 40;
  //   note3 = 41;
  //   note4 = 43;
  //   note5 = 45;
  //   note6 = 47;
  //   note7 = 48;
  // break;
  // case 3:
  //   note0 = 48;
  //   note1 = 50;
  //   note2 = 52;
  //   note3 = 53;
  //   note4 = 55;
  //   note5 = 57;
  //   note6 = 59;
  //   note7 = 60;
  // break;
  // case 4:
  //   note0 = 60;
  //   note1 = 62;
  //   note2 = 64;
  //   note3 = 65;
  //   note4 = 67;
  //   note5 = 69;
  //   note6 = 71;
  //   note7 = 72;
  // break;
  // case 5:
  //   note0 = 72;
  //   note1 = 74;
  //   note2 = 76;
  //   note3 = 77;
  //   note4 = 79;
  //   note5 = 81;
  //   note6 = 83;
  //   note7 = 84;
  // break;
  // case 6:
  //   note0 = 84; 
  //   note1 = 86;
  //   note2 = 88;
  //   note3 = 89;
  //   note4 = 91;
  //   note5 = 93;
  //   note6 = 95;
  //   note7 = 96;
  // break;
  // case 7:
  //   note0 = 96;
  //   note1 = 98;
  //   note2 = 100;
  //   note3 = 101;
  //   note4 = 103;
  //   note5 = 105;
  //   note6 = 107;
  //   note7 = 108;
  // break;
  
// Default:
  // }
}