#ifndef __JUSTIN_UI_COMMANDS_H__
#define __JUSTIN_UI_COMMANDS_H__

#include "stm32f0xx.h"
#include "ff.h"
#include "lcd.h"
#include "tty.h"
//#include "commands.h"
#include <string.h>
#include <stdio.h>

void lcd_init(int argc, char *argv[]);
void clear(int x1, int y1, int x2, int y2);
void drawline(int argc, char *argv[]);
void drawrect(int argc, char *argv[]);
void drawfillrect(int argc, char *argv[]);

void disp_begin();
void disp_entry(char letter, int index);
void init_clear ();
void disp_guess(int Reg_Correct[5], int Reg_SemiCo[5], int Reg_InCo[5]);

void disp_harp_Menu1();
void disp_keypad();
void disp_song_Set();
void disp_metronome_Set();
void disp_signature_Set();
void disp_enterNumber();
void disp_number(uint8_t number, uint8_t idx);
void disp_instrument_Set();
void disp_changeInstrument();

//void clear_harp_Menu1();

#endif