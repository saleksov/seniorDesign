#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "fifo.h"
#include "tty.h"
#include "commands.h"
#include "sounds.h"
#include "difficulty_level.h"

void internal_clock();

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
void disp_harp();
void disp_begin();
void init_clear();
void init_lcd_spi();
void dispUI(char octave, char metro, char mode);

int idx; // please comment me
int win_loss;
int metro;
int mode;
int octaveup;
int octavedn;

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
}

