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

extern uint8_t octave ;
extern uint8_t bufferOctave0;
extern uint8_t bufferOctave1;
extern uint8_t bufferSettings0;
extern uint8_t bufferSettings1;

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

#define NOPEDAL 0
#define FLANGER 0
#define FLANGERDELAY 100000

#endif