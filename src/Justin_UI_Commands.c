#include "stm32f0xx.h"
#include "ff.h"
#include "lcd.h"
#include "tty.h"
#include <string.h>
#include <stdio.h>

// Data structure for the mounted file system.

void lcd_init(int argc, char *argv[])
{
    LCD_Setup();
}

void clear(int x1, int y1, int x2, int y2)
{
    LCD_Clear(0000, x1, y1, x2, y2);
}

void drawline(int argc, char *argv[])
{
    if (argc != 6) {
        printf("Wrong number of arguments: line x1 y1 x2 y2 color");
        return;
    }
    int x1 = strtoul(argv[1], 0, 10);
    int y1 = strtoul(argv[2], 0, 10);
    int x2 = strtoul(argv[3], 0, 10);
    int y2 = strtoul(argv[4], 0, 10);
    int c = strtoul(argv[5], 0, 16);
    LCD_DrawLine(x1,y1,x2,y2,c);
}

void drawrect(int argc, char *argv[])
{
    if (argc != 6) {
        printf("Wrong number of arguments: drawrect x1 y1 x2 y2 color");
        return;
    }
    int x1 = strtoul(argv[1], 0, 10);
    int y1 = strtoul(argv[2], 0, 10);
    int x2 = strtoul(argv[3], 0, 10);
    int y2 = strtoul(argv[4], 0, 10);
    int c = strtoul(argv[5], 0, 16);
    LCD_DrawRectangle(x1,y1,x2,y2,c);
}

void drawfillrect(int argc, char *argv[])
{
    if (argc != 6) {
        printf("Wrong number of arguments: drawfillrect x1 y1 x2 y2 color");
        return;
    }
    int x1 = strtoul(argv[1], 0, 10);
    int y1 = strtoul(argv[2], 0, 10);
    int x2 = strtoul(argv[3], 0, 10);
    int y2 = strtoul(argv[4], 0, 10);
    int c = strtoul(argv[5], 0, 16);
    LCD_DrawFillRectangle(x1,y1,x2,y2,c);
}


char curguess[5];

void color(int i, int Reg_Correct[5], int Reg_SemiCo[5], int Reg_InCo[5]){
    if(i == 0){
        if(Reg_Correct[i] == 1){
            LCD_DrawFillRectangle(10,260,85,310,0x07E0);
        }
        else if(Reg_SemiCo[i] == 1){
            LCD_DrawFillRectangle(10,260,85,310,0xFFE0);
        }
        else if (Reg_InCo[i] == 1){
            LCD_DrawFillRectangle(10,260,85,310,0xF800);
        }
    }
    else if(i == 1){
        if(Reg_Correct[i] == 1){
            LCD_DrawFillRectangle(10,200,85,250,0x07E0);
        }
        else if(Reg_SemiCo[i] == 1){
            LCD_DrawFillRectangle(10,200,85,250,0xFFE0);
        }
        else if (Reg_InCo[i] == 1){
            LCD_DrawFillRectangle(10,200,85,250,0xF800);
        }
    }
    else if(i == 2){
        if(Reg_Correct[i] == 1){
            LCD_DrawFillRectangle(10,140,85,190,0x07E0);
        }
        else if(Reg_SemiCo[i] == 1){
            LCD_DrawFillRectangle(10,140,85,190,0xFFE0);
        }
        else if (Reg_InCo[i] == 1){
            LCD_DrawFillRectangle(10,140,85,190,0xF800);
        }
    }
    else if(i == 3){
        if(Reg_Correct[i] == 1){
            LCD_DrawFillRectangle(10,80,85,130,0x07E0);
        }
        else if(Reg_SemiCo[i] == 1){
            LCD_DrawFillRectangle(10,80,85,130,0xFFE0);
        }
        else if (Reg_InCo[i] == 1){
            LCD_DrawFillRectangle(10,80,85,130,0xF800);
        }
    }
    else{
        if(Reg_Correct[i] == 1){
            LCD_DrawFillRectangle(10,20,85,70,0x07E0);
        }
        else if(Reg_SemiCo[i] == 1){
            LCD_DrawFillRectangle(10,20,85,70,0xFFE0);
        }
        else{
            LCD_DrawFillRectangle(10,20,85,70,0xF800);
        }
    }
}

void disp_begin(){

        LCD_Clear(0000, 10+1, 140+1, 85-1, 190);
        LCD_DrawChar(145, 15, 0xFFFF, 0xFFFF, '#', 16, 5);

        LCD_Clear(0000, 125+1, 260+1, 200-1, 310);
        LCD_DrawChar(265, 130, 0xFFFF, 0xFFFF, 'S', 16, 5);

        LCD_Clear(0000, 125+1, 200+1, 200-1, 250);
        LCD_DrawChar(205, 130, 0xFFFF, 0xFFFF, 'T', 16, 5);

        LCD_Clear(0000, 125+1, 140+1, 200-1, 190);
        LCD_DrawChar(145, 130, 0xFFFF, 0xFFFF, 'A', 16, 5);

        LCD_Clear(0000, 125+1, 80+1, 200-1, 130);
        LCD_DrawChar(85, 130, 0xFFFF, 0xFFFF, 'R', 16, 5);

        LCD_Clear(0000, 125+1, 20+1, 200-1, 70);
        LCD_DrawChar(25, 130, 0xFFFF, 0xFFFF, 'T', 16, 5);

}

void init_clear (){

        LCD_Clear(0000, 10+1, 140+1, 85-1, 190);

        LCD_Clear(0000, 125+1, 140+1, 200-1, 190);

        LCD_Clear(0000, 125+1, 260+1, 200-1, 310);

        LCD_Clear(0000, 125+1, 200+1, 200-1, 250);

        LCD_Clear(0000, 125+1, 140+1, 200-1, 190);

        LCD_Clear(0000, 125+1, 80+1, 200-1, 130);

        LCD_Clear(0000, 125+1, 20+1, 200-1, 70);
}

void disp_guess(int Reg_Correct[5], int Reg_SemiCo[5], int Reg_InCo[5]){
    for(int i = 0; i < 5; i++){
        if(i == 0){
            LCD_Clear(0000, 10+1, 260+1, 85-1, 310);
            LCD_Clear(0000, 125+1, 260+1, 200-1, 310);
            color(i, Reg_Correct, Reg_SemiCo, Reg_InCo);
            LCD_DrawChar(265, 15, 0xFFFF, 0xFFFF, curguess[0], 16, 5);
        }
        else if (i == 1){
            LCD_Clear(0000, 10+1, 200+1, 85-1, 250);
            LCD_Clear(0000, 125+1, 200+1, 200-1, 250);
            color(i, Reg_Correct, Reg_SemiCo, Reg_InCo);
            LCD_DrawChar(205, 15, 0xFFFF, 0xFFFF, curguess[1], 16, 5);
            
        }
        else if (i == 2){
            LCD_Clear(0000, 10+1, 140+1, 85-1, 190);
            LCD_Clear(0000, 125+1, 140+1, 200-1, 190);
            color(i, Reg_Correct, Reg_SemiCo, Reg_InCo);
            LCD_DrawChar(145, 15, 0xFFFF, 0xFFFF, curguess[2], 16, 5);
        }
        else if (i == 3){
            LCD_Clear(0000, 10+1, 80+1, 85-1, 130);
            LCD_Clear(0000, 125+1, 80+1, 200-1, 130);
            color(i, Reg_Correct, Reg_SemiCo, Reg_InCo);
            LCD_DrawChar(85, 15, 0xFFFF, 0xFFFF, curguess[3], 16, 5);
        }
        else{
            LCD_Clear(0000, 10+1, 20+1, 85-1, 70);
            LCD_Clear(0000, 125+1, 20+1, 200-1, 70);
            color(i, Reg_Correct, Reg_SemiCo, Reg_InCo);
            LCD_DrawChar(25, 15, 0xFFFF, 0xFFFF, curguess[4], 16, 5);
        }
    }
}

void disp_entry(char letter, int index){

    if(letter == '!'){
        return;
    }

    if(index == 0){
        curguess[0] = letter;
        LCD_Clear(0000, 125+1, 260+1, 200-1, 310);
        LCD_DrawChar(265, 130, 0xFFFF, 0xFFFF, letter, 16, 5);
    }
    else if(index == 1){
        curguess[1] = letter;
        LCD_Clear(0000, 125+1, 200+1, 200-1, 250);
        LCD_DrawChar(205, 130, 0xFFFF, 0xFFFF, letter, 16, 5);
    }
    else if (index == 2){
        curguess[2] = letter;
        LCD_Clear(0000, 125+1, 140+1, 200-1, 190);
        LCD_DrawChar(145, 130, 0xFFFF, 0xFFFF, letter, 16, 5);
    }
    else if(index == 3){
        curguess[3] = letter;
        LCD_Clear(0000, 125+1, 80+1, 200-1, 130);
        LCD_DrawChar(85, 130, 0xFFFF, 0xFFFF, letter, 16, 5);
    }
    else if(index == 4){
        curguess[4] = letter;
        LCD_Clear(0000, 125+1, 20+1, 200-1, 70);
        LCD_DrawChar(25, 130, 0xFFFF, 0xFFFF, letter, 16, 5);
    }

}

void loser(char tobeguessed[5]){
    for(int i = 0; i < 5; i++){
        switch(i){
            case 0: 
                LCD_Clear(0000, 125+1, 260+1, 200-1, 310);
                LCD_DrawChar(265, 130, 0xFFFF, 0xFFFF, tobeguessed[0], 16, 5);
                break;
            case 1: 
                LCD_Clear(0000, 125+1, 200+1, 200-1, 250);
                LCD_DrawChar(205, 130, 0xFFFF, 0xFFFF, tobeguessed[1], 16, 5);
                break;
            case 2: 
                LCD_Clear(0000, 125+1, 140+1, 200-1, 190);
                LCD_DrawChar(145, 130, 0xFFFF, 0xFFFF, tobeguessed[2], 16, 5);
                break;
            case 3: 
                LCD_Clear(0000, 125+1, 80+1, 200-1, 130);
                LCD_DrawChar(85, 130, 0xFFFF, 0xFFFF, tobeguessed[3], 16, 5);
                break;
            case 4: 
                LCD_Clear(0000, 125+1, 20+1, 200-1, 70);
                LCD_DrawChar(25, 130, 0xFFFF, 0xFFFF, tobeguessed[4], 16, 5);
                break;
            default: break;
        }

    }
}

void disp_wordle(){

    LCD_Setup();
        
    LCD_Clear(0000, 0, 0, 0, 0);


    //top row
    LCD_DrawFillRectangle(90,20,95,70,0xffff);
    LCD_DrawFillRectangle(90,80,95,130,0xffff);
    LCD_DrawFillRectangle(90,140,95,190,0xffff);
    LCD_DrawFillRectangle(90,200,95,250,0xffff);
    LCD_DrawFillRectangle(90,260,95,310,0xffff);
    LCD_DrawRectangle(10,20,85,70,0xffff);
    LCD_DrawRectangle(10,80,85,130,0xffff);
    LCD_DrawRectangle(10,140,85,190,0xffff);
    LCD_DrawRectangle(10,200,85,250,0xffff);
    LCD_DrawRectangle(10,260,85,310,0xffff);

    //bottom row
    LCD_DrawFillRectangle(205,20,210,70,0xffff);
    LCD_DrawFillRectangle(205,80,210,130,0xffff);
    LCD_DrawFillRectangle(205,140,210,190,0xffff);
    LCD_DrawFillRectangle(205,200,210,250,0xffff);
    LCD_DrawFillRectangle(205,260,210,310,0xffff);
    LCD_DrawRectangle(125,20,200,70,0xffff);
    LCD_DrawRectangle(125,80,200,130,0xffff);
    LCD_DrawRectangle(125,140,200,190,0xffff);
    LCD_DrawRectangle(125,200,200,250,0xffff);
    LCD_DrawRectangle(125,260,200,310,0xffff);

}

uint8_t MetroWord[] = {'M', 'E', 'T', 'R', 'O', 'N', 'O', 'M', 'E'};
uint8_t OctoWord[] = {'O', 'C', 'T', 'A', 'V', 'E'};
//uint8_t Team8Word[] = {'T', 'E', 'A', 'M', '8'};
uint8_t StringlessWord[] = {'S', 'T', 'R', 'I', 'N', 'G', 'L', 'E', 'S', 'S'};
uint8_t HarpWord[] = {'H', 'A', 'R', 'P'};
uint8_t PressWord[] = {'P', 'R', 'E', 'S', 'S', ' ', 'F', 'R', 'O', 'N', 'T'};
uint8_t PBsettingsWord[] = {'P', 'B', ' ','4', ' ', 'S', 'E', 'T', 'T', 'I', 'N', 'G', 'S'};


// between letters 20 
// line to line start at 35, increase by 70

uint16_t row[] = {10, 50, 90, 130, 170, 210, 250, 310};
uint16_t col[] = {260, 240, 220, 200, 180, 160, 140, 120, 100, 80, 60, 40, 20};

void disp_harp_Menu1(){

    LCD_Clear(0000, 0, 0, 0, 0);

    // for (int i = 0; i < 5; i++){
    //     LCD_DrawChar(col[i+3], row[0], 0xFFFF, 0xFFFF, Team8Word[i], 16, 2);
    // }

    for (int i = 0; i < 10; i++){
        LCD_DrawChar(col[i], row[1], 0xFFFF, 0xFFFF, StringlessWord[i], 16, 2);
    }

    for (int i = 0; i < 4; i++){
        LCD_DrawChar(col[i+4] + 10, row[2], 0xFFFF, 0xFFFF, HarpWord[i], 16, 2);
    }

    for(int i = 0; i < 11; i++){
        LCD_DrawChar(col[i+1], row[4]-10, 0xFFFF, 0xFFFF, PressWord[i], 16, 2);
    }

    for (int i = 0; i < 13; i++){
        LCD_DrawChar(col[i], row[5]-10, 0xFFFF, 0xFFFF, PBsettingsWord[i], 16, 2);
    }
}

uint8_t SongPlayerWord[] =  {'M', 'P', '3', ' ', 'P', 'L', 'A', 'Y', 'E', 'R', ' ', 'A'}; 
uint8_t MetronomeWord[] =   {'M', 'E', 'T', 'R', 'O', 'N', 'O', 'M', 'E', ' ', ' ', 'B'};
uint8_t InstrumentWord[] =  {'I', 'N', 'S', 'T', 'R', 'U', 'M', 'E', 'N', 'T', ' ', 'C'};
//uint8_t VolumeWord[] =      {'V', 'O', 'L', 'U', 'M', 'E', ' ', ' ', ' ', ' ', ' ', 'D'};
uint8_t GoBackWord[] =      {'G', 'O', ' ', 'B', 'A', 'C', 'K', ' ', ' ', ' ', ' ', '#'};

void disp_keypad(){

    LCD_Clear(0000, 0, 0, 0, 0);

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[1], 0xFFFF, 0xFFFF, SongPlayerWord[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[2], 0xFFFF, 0xFFFF, MetronomeWord[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[3], 0xFFFF, 0xFFFF, InstrumentWord[i], 16, 2);
    }

    // for (int i = 0; i < 12; i++){
    //     LCD_DrawChar(col[i], row[4], 0xFFFF, 0xFFFF, VolumeWord[i], 16, 2);
    // }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[5], 0xFFFF, 0xFFFF, GoBackWord[i], 16, 2);
    }
}

uint8_t StartStopWord[] =   {'S', 'T', 'A', 'R', 'T', '/', 'S', 'T', 'O', 'P', ' ', 'A'};
uint8_t PauseResumeWord[] = {'P', 'A', 'U', 'S', 'E', '/', 'P', 'L', 'A', 'Y', ' ', 'B'};
uint8_t skipsongWord[] =    {'S', 'K', 'I', 'P', ' ', 'S', 'O', 'N', 'G', ' ', ' ', 'C'};
uint8_t SellectSongWithNumWord[] = {'S', 'E', 'L', 'E', 'C', 'T', ' ', 'S', 'O', 'N', 'G'};
uint8_t Nums09Word[] =      {'W', 'I', 'T', 'H', ' ', 'N', 'U', 'M', 'S', ' ', '0', '-', '9'};

void disp_song_Set()
{
    LCD_Clear(0000, 0, 0, 0, 0);

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[1], 0xFFFF, 0xFFFF, StartStopWord[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[2], 0xFFFF, 0xFFFF, PauseResumeWord[i], 16, 2);
    }   

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[3], 0xFFFF, 0xFFFF, skipsongWord[i], 16, 2);
    }

    for (int i = 0; i < 11; i++){
        LCD_DrawChar(col[i], row[4], 0xFFFF, 0xFFFF, SellectSongWithNumWord[i], 16, 2);
    }

    for (int i = 0; i < 13; i++){
        LCD_DrawChar(col[i], row[5], 0xFFFF, 0xFFFF, Nums09Word[i], 16, 2);
    }
}

uint8_t SetBPMWord[] =      {'S', 'E', 'T', ' ', 'B', 'P', 'M', ' ', ' ', ' ', ' ', 'B'};
uint8_t SignatureWord[] =   {'S', 'I', 'G', 'N', 'A', 'T', 'U', 'R', 'E', ' ', ' ', 'C'};

void disp_metronome_Set()
{
    LCD_Clear(0000, 0, 0, 0, 0);

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[1], 0xFFFF, 0xFFFF, StartStopWord[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[2], 0xFFFF, 0xFFFF, SetBPMWord[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[3], 0xFFFF, 0xFFFF, SignatureWord[i], 16, 2);
    }
    // Add code to display metronome settings
}

uint8_t Word44[] = {'4', '/', '4', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A'};
uint8_t Word34[] = {'3', '/', '4', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'B'};
uint8_t Word54[] = {'5', '/', '4', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'C'};
uint8_t Word78[] = {'7', '/', '8', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'D'};

void disp_signature_Set()
{
    LCD_Clear(0000, 0, 0, 0, 0);

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[1], 0xFFFF, 0xFFFF, Word44[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[2], 0xFFFF, 0xFFFF, Word34[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[3], 0xFFFF, 0xFFFF, Word54[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[4], 0xFFFF, 0xFFFF, Word78[i], 16, 2);
    }
}

uint8_t EnterNumber[] = {'E', 'N', 'T', 'E', 'R', ' ', 'N', 'U', 'M', 'B', 'E', 'R', ':'};
uint8_t confirmWord[] = {'C', 'O', 'N', 'F', 'I', 'R', 'M', ' ', ' ', ' ', ' ', '#'};

void disp_enterNumber()
{
    LCD_Clear(0000, 0, 0, 0, 0);
    for (int i = 0; i < 13; i++){
        LCD_DrawChar(col[i], row[2], 0xFFFF, 0xFFFF, EnterNumber[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[4], 0xFFFF, 0xFFFF, confirmWord[i], 16, 2);
    }
}


void disp_number(uint8_t number, uint8_t idx)
{
    char numChar = number + '0'; // Convert to character
    LCD_DrawChar(col[idx+3], row[3], 0xFFFF, 0xFFFF, numChar, 16, 2);
}

uint8_t InstrumentWord[] =  {'I', 'N', 'S', 'T', 'R', 'U', 'M', 'E', 'N', 'T', ' ', 'A'};
uint8_t VolumeWord[] =      {'V', 'O', 'L', 'U', 'M', 'E', ' ', ' ', ' ', ' ', ' ', 'B'};
uint8_t OctaveInc [] =      {'O', 'C', 'T', 'A', 'V', 'E', ' ', 'I', 'N', 'C', ' ', 'C'};

void disp_instrument_Set()
{
    LCD_Clear(0000, 0, 0, 0, 0);

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[1], 0xFFFF, 0xFFFF, InstrumentWord[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[2], 0xFFFF, 0xFFFF, VolumeWord[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[3], 0xFFFF, 0xFFFF, OctaveInc[i], 16, 2);
    }
}

uint8_t StringsWord[] =      {'S', 'T', 'R', 'I', 'N', 'G', 'S', ' ', ' ', ' ', ' ', 'C'};

void disp_changeInstrument()
{
    LCD_Clear(0000, 0, 0, 0, 0);

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[1], 0xFFFF, 0xFFFF, SongPlayerWord[i], 16, 2);
    }
    
    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[2], 0xFFFF, 0xFFFF, MetronomeWord[i], 16, 2);
    }

    for (int i = 0; i < 12; i++){
        LCD_DrawChar(col[i], row[3], 0xFFFF, 0xFFFF, StringsWord[i], 16, 2);
    }
}

