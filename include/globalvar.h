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
extern volatile uint8_t currentSong;

// keypad
extern uint8_t col;

// Volume

extern uint8_t strings_volume;
extern uint8_t songPlayer_volume;
extern uint8_t metronome_volume;
 
#define Sweeden '1'
#define Dry '2'
#define Poke '3'
#define Sweet '4'
#define Nnn '5'
#define Cat '6'
#define Mario '7'
#define Snowman '8'
#define Mocking '9'
#define Wonder '0'
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
#define SweedenIdx 0
#define DryIdx 322
#define PokeIdx 1119
#define SweetIdx 7741
#define NnnIdx 9225
#define CatIdx 9354
#define MarioIdx 27483
#define SnowmanIdx 31732
#define MockingIdx 34573
#define WonderIdx 39131

#define STRINGS_CHANNEL 10
#define METRONOME_CHANNEL 11
#define SONGPLAYER_CHANNEL 0
#define DEBUG_CHANNEL 9

#endif