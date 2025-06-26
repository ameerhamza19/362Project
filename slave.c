#include "stm32f0xx.h"

void SPI1_Slave_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    GPIOA->MODER |= (GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER4_1);
    GPIOA->AFR[0] |= (0 << GPIO_AFRL_AFSEL5_Pos) | (0 << GPIO_AFRL_AFSEL6_Pos) | (0 << GPIO_AFRL_AFSEL4_Pos);

    SPI1->CR1 = 0;
    SPI1->CR2 = SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; 
    SPI1->CR1 |= SPI_CR1_SPE;
}

             

int main(void) {
    SPI1_Slave_Init();
    uint8_t received_data;
    while (1) {
        received_data = SPI1_Slave_Receive();
//Any processing goes here
    } 
}

uint8_t SPI1_Slave_Receive(void) {
        while (!(SPI1->SR & SPI_SR_RXNE)); 
        return SPI1->DR;    
}
