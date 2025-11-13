#include "sendmidicode.h"

void debugSendNote(void)
{
  sendMIDI(0,60,100);
  sendMIDI(0,66,100);
  sendMIDI(0,69,100);
}

void allNotesOff(uint8_t channel)
{
    while (!(USART5->ISR & USART_ISR_TXE));
    USART5->TDR = 0xB0 | (channel & 0x0F);  // Control Change on channel
    while (!(USART5->ISR & USART_ISR_TXE));
    USART5->TDR = 0x7B;                     // Controller 123 = All Notes Off
    while (!(USART5->ISR & USART_ISR_TXE));
    USART5->TDR = 0x00;                     // Value = 0 (ignored)
}

void changeInstrument(uint8_t channel, uint8_t instrument)
{
  // Channel
  while (!(USART5->ISR & USART_ISR_TXE))
     ; // Wait until TX buffer empty

   USART5->TDR = 0xC0 | (channel & 0xF);

   // instrument
   while (!(USART5->ISR & USART_ISR_TXE))
     ; // Wait until TX buffer empty
   USART5->TDR = (instrument) & 0x7F;
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

void setOctave(void)
{
  note00 = octave + 0;
  note01 = octave + 1; 
  note02 = octave + 2;
  note03 = octave + 3;
  note04 = octave + 4;
  note05 = octave + 5;
  note06 = octave + 6;
  note07 = octave + 7;
  note08 = octave + 8;
  note09 = octave + 9;
  note10 = octave + 10;
  note11 = octave + 11;
}