#ifndef __SENDMIDICODE_H__
#define __SENDMIDICODE_H__

#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "globalvar.h"

void sendMIDI(uint8_t channel, uint8_t note, uint8_t velocity);
void sendMIDIstrings(uint8_t channel, uint8_t note, uint8_t velocity);
void setOctave(void); 

void changeInstrument(uint8_t channel, uint8_t instrument);
void allNotesOff(uint8_t channel);
void debugSendNote(void);

#endif