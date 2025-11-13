#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h> // for memmove()
#include "globalvar.h"

// Wait for roughly n cycles (inline assembly)
void nano_wait(unsigned int n);

// Display functions
void set_digit_segments(int digit, char val);
void print(const char str[]);
void printfloat(float f);
void append_segments(char val);
void clear_display(void);
void dot(void);

// Keypad / button functions
void push_queue(int n);
char pop_queue(void);
void update_history(int c, int rows);
void drive_column(int c);
int read_rows(void);
char get_key_event(void);
char get_keypress(void);
void show_keys(void);

// Input functions
float getfloat(void);
int getrgb(void);

// Dialer / frequency function
void set_freq(int chan, float freq);
void dialer(void);

#endif