#ifndef __GLOBALVAR__H__
#define __GLOBALVAR__H__

#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>

uint32_t ms_counter; 

extern uint8_t bufferString00;
extern uint8_t bufferString01;
extern uint8_t bufferString02;
extern uint8_t bufferString03;
extern uint8_t bufferString04;
extern uint8_t bufferString05;
extern uint8_t bufferString06;
extern uint8_t bufferString07;
extern uint8_t bufferString08;
extern uint8_t bufferString09;
extern uint8_t bufferString10;
extern uint8_t bufferString11;

extern uint8_t note00;
extern uint8_t note01;
extern uint8_t note02;
extern uint8_t note03;
extern uint8_t note04;
extern uint8_t note05;
extern uint8_t note06;
extern uint8_t note07;
extern uint8_t note08;
extern uint8_t note09;
extern uint8_t note10;
extern uint8_t note11;

extern uint8_t octave;
extern uint8_t octaveStep;

extern uint8_t bufferButton0; // Octave down
extern uint8_t bufferButton1; // Octave Up
extern uint8_t bufferButton2; // Clear all notes 
extern uint8_t bufferButton3; // Enter / Exit Settings

extern uint8_t BPM;
extern uint8_t Mstatus;
extern uint8_t metroSig;
extern uint8_t metroSigCounter;
extern uint8_t Pedal;

extern char octaves[];
extern char modes[];
extern char metros[];

extern uint8_t modeidx;
extern uint8_t metroidx;

// Song Player
extern volatile uint32_t tim7Ticks;
extern volatile uint8_t songPlaying;
extern uint32_t songIndex;
extern uint32_t next_tick;
extern uint8_t song_done;

// keypad
extern uint8_t col;

#endif