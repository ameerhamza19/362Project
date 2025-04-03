#include "stm32f0xx.h"

void SPI1_Master_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    GPIOA->MODER |= (GPIO_MODER_MODER5_1 | GPIO_MODER_MODER7_1 | GPIO_MODER_MODER4_1);
    GPIOA->AFR[0] |= (0 << GPIO_AFRL_AFSEL5_Pos) | (0 << GPIO_AFRL_AFSEL7_Pos) | (0 << GPIO_AFRL_AFSEL4_Pos);

    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_1 | SPI_CR1_SSI | SPI_CR1_SSM;
    SPI1->CR2 = SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;

    SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI1_Master_Transmit(uint8_t data) {
    while (!(SPI1->SR & SPI_SR_TXE));  
    SPI1->DR = data;                   
    while (SPI1->SR & SPI_SR_BSY);     
}

int main(void) {
    SPI1_Master_Init();
    //example use ig
    while (1) {
        SPI1_Master_Transmit(0x55); 
        for (volatile int i = 100000; i > 0; i--); 
    }
}
