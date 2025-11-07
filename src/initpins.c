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
}

void initc()
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIOC->MODER &= 0xFFFFFF00;
  GPIOC->PUPDR &= 0xFFFFFF55;
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