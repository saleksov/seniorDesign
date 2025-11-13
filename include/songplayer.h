#ifndef __SONGPLAYER_H__
#define __SONGPLAYER_H__

#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "globalvar.h"
#include "sendmidicode.h"

void init_tim7_songDelay();
void songPlayer();
void startSong(void);
void stopSong(void);
void pauseSong(void);
void resumeSong(void);

#endif