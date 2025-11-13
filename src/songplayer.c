#include "songplayer.h"

const unsigned char sweeden [] = {
// Grand Piano (Classic)
0x90,55, 0x91,47, 0x92,52, 5,83, 0x90,69, 0x91,54, 0x82, 2,170, 0x90,71, 2,170, 0x90,45, 0x91,62, 0x92,57, 0x93,66, 
5,83, 0x90,47, 0x81, 0x82, 0x83, 2,170, 0x91,62, 1,85, 0x91,64, 1,85, 0x90,57, 0x91,61, 0x92,66, 5,84, 0x90,55, 
0x81, 0x82, 2,169, 0x90,54, 1,85, 0x90,57, 1,85, 0x90,57, 0x91,61, 0x92,64, 0x93,45, 10,168, 0x90,55, 0x91,64, 
0x92,67, 0x93,52, 2,169, 0x94,74, 2,170, 0x94,71, 2,170, 0x94,69, 2,170, 0x90,43, 0x91,57, 0x92,66, 0x93,62, 
0x84, 5,83, 0x90,47, 2,170, 0x91,62, 0x82, 0x83, 1,85, 0x91,64, 1,85, 0x90,57, 0x91,61, 0x92,66, 5,84, 0x90,54, 
0x81, 0x82, 2,170, 0x91,69, 1,84, 0x91,66, 1,85, 0x90,45, 0x91,57, 0x92,61, 0x93,64, 10,168, 0x90,59, 0x91,64, 
0x92,67, 0x93,52, 5,83, 0x90,69, 0x91,54, 0x82, 0x83, 2,170, 0x90,71, 2,170, 0x90,66, 0x91,62, 0x92,43, 0x93,57, 
0x94,74, 5,84, 0x90,59, 0x81, 0x82, 0x83, 0x84, 2,169, 0x90,62, 0x91,78, 1,85, 0x90,64, 0x91,76, 1,85, 0x90,69, 
0x91,61, 0x92,57, 0x93,73, 5,84, 0x90,55, 0x81, 0x82, 0x83, 2,170, 0x91,74, 0x92,66, 1,85, 0x91,73, 1,84, 0x90,69, 
0x81, 0x82, 10,168, 0x90,59, 0x91,67, 0x92,64, 5,83, 0x90,71, 0x91,66, 0x82, 2,170, 0x90,69, 2,170, 0x90,57, 0x91,74, 
0x92,69, 0x93,78, 5,84, 0x90,59, 0x81, 0x82, 0x83, 2,169, 0x91,74, 1,85, 0x91,76, 1,85, 0x90,69, 0x91,73, 0x92,78, 
5,84, 0x90,67, 0x81, 0x82, 2,170, 0x90,66, 1,85, 0x90,69, 1,85, 0x90,61, 0x91,57, 0x92,64, 10,167, 0x80, 0x81, 
0x82, 0xF0};
const unsigned int sweedenLen = 320;

void songPlayer(void)
{
    if (!songPlaying || song_done) return;

    while (songIndex < sweedenLen && tim7Ticks >= next_tick)
    {
        uint8_t b = sweeden[songIndex++];

        if (b & 0x80)
        {
            uint8_t cmd = b & 0xF0;

            switch (cmd)
            {
                case 0x90: {  // Note On
                    uint8_t note = sweeden[songIndex++];
                    sendMIDI(0x90, note, 100);
                    break;
                }
                case 0x80: {  // Note Off
                    // aaaa do nothing
                    break;
                }
                case 0xF0:    // End of song
                    song_done = 1;
                    songPlaying = 0;
                    allNotesOff(0); // stop all notes on all channels
                    return;

                default:
                    break;
            }
        }
        else
        {
            if (songIndex >= sweedenLen) break;
            uint16_t delay = ((b & 0x7F) << 8) | sweeden[songIndex++];
            next_tick = tim7Ticks + delay;
            break;
        }
    }
}

void startSong(void)
{
    tim7Ticks = 0;
    songIndex = 0;
    next_tick = 0;
    song_done = 0;
    songPlaying = 1;

    TIM7->SR &= ~TIM_SR_UIF;
    TIM7->CR1 |= TIM_CR1_CEN;
}

void stopSong(void)
{
    TIM7->CR1 &= ~TIM_CR1_CEN;

    for (uint8_t ch = 0; ch < 16; ch++) {
        allNotesOff(ch);
    }

    songPlaying = 0;
    song_done = 1;
}

void pauseSong(void)
{
    TIM7->CR1 &= ~TIM_CR1_CEN;
    songPlaying = 0;
}

void resumeSong(void)
{
    TIM7->SR &= ~TIM_SR_UIF;
    TIM7->CR1 |= TIM_CR1_CEN;
    songPlaying = 1;
}