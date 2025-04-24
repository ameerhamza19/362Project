#include "stm32f0xx.h"

void init_rgb(void);
void rgb_green(void);
void rgb_red(void);
void init_question_tim(void);
void seg7_init_bb(void);
void seg7_write_bit(int val);
void seg7_write_halfword(int halfword);
void drive_7seg(void);
char num_to_7seg(int input_7seg);

//Uses GPIOA and TIM1 to configure PWM for RGB light on PA8-PA10.
//DOES NOT ENABLE TIMER - other functions do so for better color control
void init_rgb(){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    GPIOA->MODER &= ~(0x3f << (8 * 2));
	GPIOA->MODER |= (0x2a << (8 * 2));
	GPIOA->AFR[1] |= 0x222;
	TIM1->BDTR |= (1 << 15);
	TIM1->PSC = 1 - 1;
	TIM1->ARR = 48000 - 1;
    TIM1->CCMR1 |= 0x6060;
	TIM1->CCMR2 |= 0x60;
	TIM1->CCER |= 0x111;
}

void rgb_green(){ //enables rgb timer with green output
	TIM1->CCR1 = 48000 - 1; 
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 48000 - 1;
    TIM1->CR1 = TIM_CR1_CEN;
}

void rgb_red(){ //enables rgb timer with red output
	TIM1->CCR1 = 48000 - 1;
	TIM1->CCR2 = 48000 - 1;
	TIM1->CCR3 = 0;
    TIM1->CR1 = TIM_CR1_CEN;
}

//creates a timer that updates every second
//will update time remaining value on 7 segment
uint8_t time_remaining = 	30;
//Initialize Timer to trigger every second (represents remaining time)
void init_question_tim(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
	TIM14->PSC = 4800 - 1;
	TIM14->ARR = 10000 - 1;
	TIM14->DIER |= TIM_DIER_UIE;
	NVIC->ISER[0] = 1 << TIM14_IRQn;
	//TIM14->CR1 |= TIM_CR1_CEN;
}

//IRQ Handler for that timer
void TIM14_IRQHandler(){
	TIM14->SR &= ~TIM_SR_UIF;
	time_remaining -= 1;
	//reset bits and enter new values for bit banging
	seg7_msg[3] &= ~0xFF;
	seg7_msg[4] &= ~0xFF;
	seg7_msg[3] |= num_to_7seg(time_remaining / 10);
	seg7_msg[4] |= num_to_7seg(time_remaining % 10);
	if(time_remaining == 0){
		//Handling for incorrect answer (Coordinate w/ Osan)
		TIM14->CR1 &= ~TIM_CR1_CEN;
		rgb_red();
	}
}

///////////////////////////////////////////////////
//BITBANGING FOR 7SEG
///////////////////////////////////////////////////

//assuming this is a way to identify 7seg index...
//Modified from lab6 to only use 1 display for 4 slots
uint16_t seg7_msg[4] = {0x0000, 0x0100, 0x0200, 0x0300};

//setup (PB12 | PB13 | PB15) as (CS | SCK | SDI)
void seg7_init_bb(){
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~0xff << (12*2);
    GPIOB->MODER |= 0x45000000;

    GPIOB->BSRR &= ~0xffff;
    GPIOB->BSRR |= 0x1000;
}

void seg7_write_bit(int val){
	// CS (PB12)
    // SCK (PB13)
    // SDI (PB15)
    GPIOB->ODR &= ~(1 << 15);
    GPIOB->ODR |= val << 15;
    nano_wait(50000);
    GPIOB->ODR |= 1 << 13;
    nano_wait(50000);
    GPIOB->ODR &= ~(1 << 13);
}

void seg7_write_halfword(int halfword){
	GPIOB->ODR &= ~(1 << 12);
    for(int i = 15; i >= 0; i--){
        seg7_write_bit((halfword >> i) & 1);
    }
    GPIOB->ODR |= 1 << 12;
}

void drive_7seg(void){
	while(time_remaining > 0){
		for(int i = 0; i<4; i++){
			seg7_write_halfword(seg7_msg[i]);
			nano_wait(1000000);
		}
	}
}

//Change input value to hex code. No need to copy over font[], we only need digits here
char num_to_7seg(int input_7seg){
	char seg_out[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};
	return seg_out[input_7seg];
}
