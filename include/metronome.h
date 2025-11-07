#ifndef __METRONOME_H__
#define __METRONOME_H__

#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "sendmidicode.h"
#include "globalvar.h"
#include "commands.h"

void init_tim6_metronome(void);
void TIM6_DAC_IRQHandler(void);
void set_bpm(void);
void toggleMetronome(void);
void metroSignature(void);

#endif