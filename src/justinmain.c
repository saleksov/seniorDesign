// KEYPAD MAPING LOGIC FOR KEYS
//  1 -> AB     2 -> CD     3 -> EF     A -> GH
//  4 -> IJ     5 -> KL     6 -> MN     B -> OP
//  7 -> QR     8 -> ST     9 -> UV     C -> WX
//              0 -> YZ     # -> ENTER

#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "fifo.h"
#include "tty.h"
#include "commands.h"

// char dictionary[number of words][number of letters in each word] = 
//{"asdfg", "anasd"} ....

// key = adc_value % "number of words"
// for (int i in number of letters)
// tobeGuessed[i] = dictionary[key_(calculated above)][i];


#define WORD_LENGTH 5
#define MAX_GUESSES 5

uint8_t col; // the column being scanned

void internal_clock();

void drive_column(int);   // energize one of the column outputs
int  read_rows();         // read the four row inputs
void update_history(int col, int rows); // record the buttons of the driven column
char get_key_event(void); // wait for a button event (press or release)
char get_keypress(void);  // wait for only a button press event.
float getfloat(void);     // read a floating-point number from keypad
void show_keys(void);     // demonstrate get_key_event()



// TIM 7 IS USED FOR KEYPAD

// void TIM7_IRQHandler() {  // this is the interrupt
//     TIM7->SR &= ~TIM_SR_UIF;  // Turn off interrupt pending bit
//     int rows = read_rows();   // get rows

//     update_history(col, rows);  // update history

//     col = (col+1) & 3;  // wrap around to 1
    
//     drive_column(col);    // drive col
// }

// TIM 7 IS USED FOR KEYPAD

void init_tim7(void) {

    RCC -> APB1ENR |= RCC_APB1ENR_TIM7EN;
    TIM7 -> PSC = 240 -1;
    TIM7 -> ARR = 200 -1;
    TIM7 -> DIER |= TIM_DIER_UIE;
    NVIC -> ISER[0] |= 1 << TIM7_IRQn;
    TIM7 -> CR1 |= TIM_CR1_CEN;
}


//spi 1 for display
void init_spi1_slow (void){
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB -> MODER &= ~0x00000fc0;
    GPIOB -> MODER |= 0x00000a80;
    GPIOB -> AFR[0] &= ~(GPIO_AFRL_AFRL3 | GPIO_AFRL_AFRL4 | GPIO_AFRL_AFRL5);
    RCC -> APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1 -> CR1 &= ~SPI_CR1_SPE;
    SPI1->CR1 &= ~(SPI_CR1_BR);
    SPI1 -> CR1 |= SPI_CR1_MSTR;
    SPI1 -> CR2 |= SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;
    SPI1 -> CR2 &= ~SPI_CR2_DS_3;
    SPI1 -> CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    SPI1 -> CR2 |= SPI_CR2_FRXTH;
    SPI1 -> CR1 |= SPI_CR1_SPE;
}

void sdcard_io_high_speed (void){
    SPI1 -> CR1 &= ~SPI_CR1_SPE;
    SPI1->CR1 &= ~(SPI_CR1_BR);
    SPI1 -> CR1 &= ~(SPI_CR1_BR_2 | SPI_CR1_BR_1);
    SPI1 -> CR1 |= SPI_CR1_SPE;
}

void init_lcd_spi (void){
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB -> MODER &= ~0x30c30000;
    GPIOB -> MODER |= 0x10410000;
    init_spi1_slow();
    sdcard_io_high_speed();                             
}
//

// ENABLE KEYPAD IO PORTS

void enable_ports(void) {
    
    // ENABLE C - KEYPAD GPIO
    RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable C

    GPIOC -> MODER &= 0xFFFF0000; // Reset pins PC 0 - 7, set pins 0-3 as input
    GPIOC -> MODER |= 0x00005500; // Set pins PC 4-7 as outputs

    GPIOC -> OTYPER |= 0xF0;

    GPIOC -> PUPDR &= 0xFFFFFF00; // Reset Pins PC 0 - 3
    // GPIOC -> PUPDR |= 0x00000055; // Set PullHIGH mode
    GPIOC -> PUPDR |= 0x000000aa;
}


// FUNCTION DEFINITIONS

void getGuess(char a[]);    // GETS A GUESS

void disp_harp();
void wordle();              // RUNS WORDLE
void updateReg(char a[], char b[]);    // INTERPRETS THE INPUTTED GUESS AND UPDATES GAME REGISTERS
void clearReg();            // RESETS REGISTERS
void disp_wordle();
void loser(char tobeguessed[5]);
void disp_begin();
void init_clear();
void init_lcd_spi();
void disp_entry();
void disp_guess();
char get_letter();          // GETS A LETTER FROM KEYPAD
char convert_Let(char event, int num);    // CONVERTS A KEYPAD BTTON TO A LETTER
void dispUI(char octave, char metro, char mode);

// void printWord(char a[]);   // DEBUG FUNC
// void displayReg();          // DEBUG FUNC

// EVERYWHERE IN CODE AND DISPLAY THEY ARE ORDERED LIKE THIS
int Reg_Correct[WORD_LENGTH];   // TOP - CORRECT LETTER IN CORRECT SPOT
int Reg_SemiCo[WORD_LENGTH];    // MIDDLE - CORRECT LETTER IN INCORRECT SPOT
int Reg_InCo[WORD_LENGTH];      // BOTTOM - LETTER NOT IN WORD
int idx; // please comment me
int win_loss;
int metro;
int mode;
int octaveup;
int octavedn;

#define DICTIONARY_SIZE 80
char dictionary[DICTIONARY_SIZE][WORD_LENGTH] = {"SIGMA", "DONUT", "FELON", "GHOST", "NINJA", "CRANE", "OUIJA", "FARTS", "BLANK", "CIGAR", "COBRA", "DUMMY", "GNOME", "WACKY", "JELLY", "QUIRK", "CHAOS", "FIGHT", "PRANK", "BLOOM", "SNARL", "WHISK", "LIVID", "CLOAK", "BLINK", "ZESTY", "TRUMP", "BIDEN", "OBAMA", "BRISK", "QUACK", "JOLLY", "MERRY", "MONEY", "LEMUR", "BANJO", "WITTY", "SQUIB", "SWAMP", "FUNGI", "GRAVY", "PIZZA", "DRONE", "SPOON", "SCARY", "WACKY", "DWEEB", "TWERP", "YIKES", "GUMMY", "BONGO", "SPORK", "BLIMP", "PLANE", "HELLO", "APPLE", "MANGO", "QUILT", "DREAM", "TORCH", "SPIKE", "ANGEL", "TOWER", "TRAIN", "ZEBRA", "OCEAN", "MUSIC", "MAGIC", "CHOMP", "CHARM", "SWIRL", "DANDY", "FORGE", "STUMP", "LEDGE", "QUEST", "CRAVE", "BETAS", "ALPHA", "MARIO"};

char toBeGuessed[WORD_LENGTH] = {'H','E','L','L','O'};  // DEFAULT

void setTarget(int key)
{
    // KEY IS VALUE READ FROM ADC AFTER PRESSING # AFTER PROGRAM START
    // CHANGE LINE BELLO TO BE IN THE RANGE OF 0 - Dictionary size -1
    // CONVERT KEY FROM ADC VALUE TO AN INDEX TO ARRAY DICTIONARRY
    // SO IT IS TIED TO THE NUMBER OF WORDS
    // IT MIGHT BE SELF SUFFICIENT, BUT IT ALSO MIGHT BURN THE MICRO.
    key = (int)((float)key / 4095 * 100 / (100 / DICTIONARY_SIZE + 1)) % DICTIONARY_SIZE;
    for (int i = 0; i < WORD_LENGTH; i++)
    {
        toBeGuessed[i] = dictionary[key][i];
    }
}

void TIM7_IRQHandler() {  // this is the interrupt
    TIM7->SR &= ~TIM_SR_UIF;  // Turn off interrupt pending bit

    metro = (GPIOC->IDR & (1 << 0)) ? 1 : 0;

    mode = (GPIOC->IDR & (1 << 1)) ? 1 : 0;

    octaveup = (GPIOC->IDR & (1 << 2)) ? 1 : 0;

    octavedn = (GPIOC->IDR & (1 << 3)) ? 1 : 0;

}


int main(void){
    
    internal_clock();
    init_lcd_spi();
    init_tim7();
    enable_ports();
    setup_adc();
    init_tim2();
    init_tim1();
    //disp_wordle();

    LCD_Setup();
    disp_harp();
    char octaves[3] = {'3','4','5'};
    char modes[5] = {'A', 'B', 'C', 'D', 'E'};
    char metros[4] = {'4', '3', '5', '7'};
    int octidx = 0;
    int modeidx = 0;
    int metroidx = 0;

    dispUI(octaves[octidx], metros[metroidx], modes[modeidx]);

    while (1) {
        if(metro || mode || octaveup || octavedn){ 
            if(octavedn){
                if(octidx != 0){
                     octidx--;
                }
            }
            if(octaveup){
                if(octidx != 2){
                    octidx++;
                }
            }
            if(mode){
                if(modeidx != 4){
                    modeidx++;
                }
                else {
                    modeidx = 0;
                }
            }
            if(metro){
                if(metroidx != 3){
                    metroidx++;
                }
                else {
                    metroidx = 0;
                }
            }
            dispUI(octaves[octidx], metros[metroidx], modes[modeidx]);
        }
    }
    
    //wordle();
    //sounds(win_loss); // PLAY WINNING OR LOSING SOUND AT THE CONCLUSION OF THE GAME
}

// WORDLE MAIN

void wordle(){


    int sum_temp = 0;
    
    // CALL SET TARGET
    // PLACEHOLDER
    disp_begin();
    char event = get_keypress();
    while (event != '#')
    {
        event = get_keypress();
    }

    init_clear();
    
    char currGuess[WORD_LENGTH];
    int ADC_value = (ADC1 -> DR);
    setTarget(ADC_value);

    int currAttempt = 1;
    idx = 0;

    //SET RGB LED TO RED INITIALLY MUST WIN TO TURN GREEN
    // setrgb(0x000099);


    while(currAttempt <= MAX_GUESSES)
    {
        
        // INPUT LOGIC
        // printf("ENTER GUESS #%d\n", currAttempt);
        getGuess(currGuess);

        // ECHO BACK THE INPUT
        // printWord(currGuess);

        // GET RESULTS FRON INPUT
        updateReg(toBeGuessed, currGuess);
        disp_guess(Reg_Correct, Reg_SemiCo, Reg_InCo);
        
        // CHECK IF WIN
        sum_temp = 0;
        for (int i = 0; i < WORD_LENGTH; i++) {
            sum_temp += Reg_Correct[i];
        }

        // IF STATEMENT FOR IF WIN AND END GAME, ADD FUNC CALLS HERE FOR CASE OF WIN
        if (sum_temp == WORD_LENGTH){
            // THIS IS LOGIC FOR IF YOU WIN
            win_loss = 1;
            setrgb(0x009900);
            //nano_wait(5000000000);
            // printf("\n YOU WIN; \n\n");
            break;
        }

        currAttempt++;  // LOOP INCREMENT // DONT REMOVE

    }

    // LOSS LOGIC
    // IF STATEMENT FOR CASE OF LOSS, ADD FUNC CALLS IN THIS IF
    if (sum_temp != WORD_LENGTH) {

        // LOSS LOGIC
        setrgb(0x000099);
        win_loss = 0;
        loser(toBeGuessed);
        //printf("\n YOU LOSE; \n\n");
    }

}

void updateReg(char toBeGuessed[], char currGuess[])
{
    clearReg(); // RESET EVERYTHING TO 0
    
    int target_used[WORD_LENGTH] = {0};
    // REGISTER 1 LOGIC
    for (int i = 0; i < WORD_LENGTH; i++)
    {
        if (toBeGuessed[i] == currGuess[i])
        Reg_Correct[i] = 1;
    }

    // REGISTER 2 LOGIC
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (Reg_Correct[i] == 1) {
            target_used[i] = 1; // Mark this position in target as used
            continue;
        }

        for (int j = 0; j < WORD_LENGTH; j++) {
            // Skip if already used or in the correct position
            if (i != j && currGuess[i] == toBeGuessed[j] && !Reg_Correct[j] && !target_used[j]) {
                Reg_SemiCo[i] = 1;
                target_used[j] = 1; // Mark as used in target
                break;
            }
        }
    }

    // REGISTER 3 LOGIC
    for (int i = 0; i < WORD_LENGTH; i++)
    {
        if (Reg_Correct[i] == 0 && Reg_SemiCo[i] == 0)
        Reg_InCo[i] = 1;
    }
}

void clearReg(){
    for (int i = 0; i < WORD_LENGTH; i++){
        Reg_Correct[i] = 0;
        Reg_SemiCo[i] = 0;
        Reg_InCo[i] = 0;
    }
}


// WORK IN PROGRESS     //      //      //      //      //      //      //
void getGuess(char currGuess[])
{
    for (int i = 0; i < WORD_LENGTH; i++) {
            currGuess[i] = get_letter();
            // currGuess[i] = getchar();

        }
        // getchar();  // Input
}
// WORK IN PROGRESS     //      //      //      //      //      //      //


// KEYPAD LOGIC FOR KEYS
//  1 -> AB     2 -> CD     3 -> EF     A -> GH
//  4 -> IJ     5 -> KL     6 -> MN     B -> OP
//  7 -> QR     8 -> ST     9 -> UV     C -> WX
//              0 -> YZ     # -> ENTER
char get_letter()
{
    char letter = '!';

    // // DEBUG
    // printf("\n CURRENT LETTER IS %c \n", letter);
    // // END DEBUG

    char event = '@';
    char PrevEvent = ')';
    int intLastConverion = 0;

    do
    {
        event = get_keypress();
        
        // First Pass
        if (event != '#')
        {
            if (event != PrevEvent)
            {
                letter = convert_Let(event, 0);
                intLastConverion = 1;
            }
            else
            {
                letter = convert_Let(event, intLastConverion);
                intLastConverion++;
            }
        }
        PrevEvent = event;

        // // DEBUG
        // printf("\n CURRENT LETTER IS %c \n", letter);
        // // DEBUG

// JUSTIN HERE
        disp_entry(letter, idx);
        // GIVE JUSTIN ACCESS TO VARIABLE LETTER ON THIS LINE
        // IF (LETTER != '!')   //      //      //
    
    } while ( event != '#' || letter == '!');
    if(idx == 4){
        //disp_guess(Reg_Correct, Reg_SemiCo, Reg_InCo);
        idx = 0;
    }
    else{
        idx++;
    }
    return letter;
}

// KEYPAD LOGIC FOR KEYS
//  1 -> AB     2 -> CD     3 -> EF     A -> GH
//  4 -> IJ     5 -> KL     6 -> MN     B -> OP
//  7 -> QR     8 -> ST     9 -> UV     C -> WX
//              0 -> YZ     # -> ENTER
char convert_Let(char event, int num)
{
    char letter;
    num = num % 2;
    if (num == 0)
    {
    switch(event) {
case '1': letter = 'A'; break;
case '2': letter = 'C'; break;
case '3': letter = 'E'; break;
case '4': letter = 'I'; break;
case '5': letter = 'K'; break;
case '6': letter = 'M'; break;
case '7': letter = 'Q'; break;
case '8': letter = 'S'; break;
case '9': letter = 'U'; break;
case '0': letter = 'Y'; break;
case 'A': letter = 'G'; break;
case 'B': letter = 'O'; break;
case 'C': letter = 'W'; break;
default: letter = '!'; break;
    }}

    else
    {
    switch(event) {
case '1': letter = 'B'; break;
case '2': letter = 'D'; break;
case '3': letter = 'F'; break;
case '4': letter = 'J'; break;
case '5': letter = 'L'; break;
case '6': letter = 'N'; break;
case '7': letter = 'R'; break;
case '8': letter = 'T'; break;
case '9': letter = 'V'; break;
case '0': letter = 'Z'; break;
case 'A': letter = 'H'; break;
case 'B': letter = 'P'; break;
case 'C': letter = 'X'; break;
default: letter = '!'; break;
    }    
    }

    return letter;
}
