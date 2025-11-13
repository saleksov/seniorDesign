#include "initpins.h"

void inita()
{
  // MODER
  // 00: Input (reset state)
  // 01: General purpose output mode
  // 10: Alternate function mode
  // 11: Analog mode

  // Enable clock for GPIOB
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

  GPIOA->MODER &= 0xFF000000;

  // PUPDR
  // 00: No pull-up, pull-down
  // 01: Pull-up
  // 10: Pull-down
  // 11: Reserved

  // SET TO PULL-UP
  GPIOA->PUPDR &= 0xFF555555;
  GPIOA->PUPDR |= 0x00555555;
}

void initc()
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

  // 00: Input (reset state)
  // 01: General purpose output mode

  GPIOC->MODER &= 0xFF000000;
  GPIOC->MODER |= 0x00550000;

  GPIOC->OTYPER &= ~0xfff;
  GPIOC->OTYPER |= 0xf00;

  // 01: Pull-up
  // 10: Pull-down

  GPIOC->PUPDR &= 0xFFFF0000;
  GPIOC->PUPDR |= 0x00005555;
}

void setup_serial(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

  GPIOC->MODER &= ~(GPIO_MODER_MODER12);
  GPIOC->MODER |= GPIO_MODER_MODER12_1;

  GPIOC->AFR[1] &= ~GPIO_AFRH_AFRH4;
  GPIOC->AFR[1] |= 1 << (1 + GPIO_AFRH_AFRH4_Pos);

  RCC->APB1ENR |= RCC_APB1ENR_USART5EN;

  USART5->CR1 &= ~USART_CR1_UE; // Dissable UE, set UE = 0

  USART5->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); // Set word size of 8 bits

  USART5->CR1 &= ~USART_CR1_OVER8; // SET TO 16X OVERSAMPLING

  USART5->CR1 &= ~USART_CR1_PCE; // PARITY DISSABLED

  USART5->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1); // 1 STOP BIT

  USART5->BRR = 1536;

  USART5->CR1 |= USART_CR1_TE;

  USART5->CR1 |= USART_CR1_UE; // Enable UE, set UE = 1

  while (!((USART5->ISR) & (USART_ISR_TEACK
                            //  | USART_ISR_REACK // DO WE NEED RE?
                            )))
    ;
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

void init_lcd_spi (void){
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB -> MODER &= ~0x30c30000;
    GPIOB -> MODER |= 0x10410000;
    init_spi1_slow();
                                
}

// Used as 1ms timer for playing songs
void init_tim7_songDelay(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;

  TIM7->PSC = 240 - 1;
  TIM7->ARR = 200 - 1;

  TIM7->DIER |= TIM_DIER_UIE;
  TIM7->CR1  |= TIM_CR1_CEN;

  NVIC_EnableIRQ(TIM7_IRQn);
}

void TIM7_IRQHandler(void)
{
    TIM7->SR &= ~TIM_SR_UIF;

    tim7Ticks++;
}

// Keypad
void init_tim14_keypad(void)
{
  RCC -> APB1ENR |= RCC_APB1ENR_TIM14EN;
  TIM14 -> PSC = 240 -1;
  TIM14 -> ARR = 200 -1;
  TIM14 -> DIER |= TIM_DIER_UIE;
  NVIC -> ISER[0] |= 1 << TIM14_IRQn;
  TIM14 -> CR1 |= TIM_CR1_CEN;
}

void TIM14_IRQHandler() {  // this is the interrupt
    TIM14->SR &= ~TIM_SR_UIF;  // Turn off interrupt pending bit
    int rows = read_rows();   // get rows

    update_history(col, rows);  // update history

    col = (col+1) & 3;  // wrap around to 1
    
    drive_column(col);    // drive col
}