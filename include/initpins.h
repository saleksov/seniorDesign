#ifndef __INITPINS_H__
#define __INITPINS_H__

#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "globalvar.h"

void inita();
void initc();
void setup_serial(void);

void init_spi1_slow (void);
void init_lcd_spi (void);

#endif