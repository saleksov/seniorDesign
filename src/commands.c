#include "stm32f0xx.h"
#include "ff.h"
#include "lcd.h"
#include "tty.h"
#include "commands.h"
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

void disp_harp(){

    LCD_Clear(0000, 0, 0, 0, 0);

    LCD_DrawChar(265, 35, 0xFFFF, 0xFFFF, 'M', 16, 2);

    LCD_DrawChar(245, 35, 0xFFFF, 0xFFFF, 'E', 16, 2);

    LCD_DrawChar(225, 35, 0xFFFF, 0xFFFF, 'T', 16, 2);

    LCD_DrawChar(205, 35, 0xFFFF, 0xFFFF, 'R', 16, 2);

    LCD_DrawChar(185, 35, 0xFFFF, 0xFFFF, 'O', 16, 2);

    LCD_DrawChar(165, 35, 0xFFFF, 0xFFFF, 'N', 16, 2);

    LCD_DrawChar(145, 35, 0xFFFF, 0xFFFF, 'O', 16, 2);

    LCD_DrawChar(125, 35, 0xFFFF, 0xFFFF, 'M', 16, 2);

    LCD_DrawChar(105, 35, 0xFFFF, 0xFFFF, 'E', 16, 2);

    LCD_DrawChar(65, 35, 0xFFFF, 0xFFFF, ':', 16, 2);

    LCD_DrawChar(265, 115, 0xFFFF, 0xFFFF, 'O', 16, 2);

    LCD_DrawChar(245, 115, 0xFFFF, 0xFFFF, 'C', 16, 2);

    LCD_DrawChar(225, 115, 0xFFFF, 0xFFFF, 'T', 16, 2);

    LCD_DrawChar(205, 115, 0xFFFF, 0xFFFF, 'A', 16, 2);

    LCD_DrawChar(185, 115, 0xFFFF, 0xFFFF, 'V', 16, 2);

    LCD_DrawChar(165, 115, 0xFFFF, 0xFFFF, 'E', 16, 2);

    LCD_DrawChar(125, 115, 0xFFFF, 0xFFFF, ':', 16, 2);

    LCD_DrawChar(265, 195, 0xFFFF, 0xFFFF, 'M', 16, 2);

    LCD_DrawChar(245, 195, 0xFFFF, 0xFFFF, 'O', 16, 2);

    LCD_DrawChar(225, 195, 0xFFFF, 0xFFFF, 'D', 16, 2);

    LCD_DrawChar(205, 195, 0xFFFF, 0xFFFF, 'E', 16, 2);

    LCD_DrawChar(165, 195, 0xFFFF, 0xFFFF, ':', 16, 2);

}

void dispUI (char octave, char metro, char mode){

    disp_harp();

    LCD_DrawChar(25, 35, 0xFFFF, 0xFFFF, metro, 16, 2);

    LCD_DrawChar(85, 115, 0xFFFF, 0xFFFF, octave, 16, 2);

    LCD_DrawChar(125, 195, 0xFFFF, 0xFFFF, mode, 16, 2);
}