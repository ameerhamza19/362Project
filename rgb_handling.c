#include "stm32f0xx.h"
#include <stdio.h>
#include <stdint.h>

#define QUESTION_TIME 30

void internal_clock();
void init_rgb(void);
void rgb_green(void);
void rgb_red(void);
void init_question_tim(void);
void seg7_init_bb(void);
void seg7_write_bit(int val);
void seg7_write_halfword(int halfword);
void drive_7seg(void);
char num_to_7seg(int input_7seg);
void toggle_question_tim(void);
void rgb_off();

volatile uint16_t seg7_msg[4] = {0x0000, 0x0100, 0x0200, 0x0300};
volatile int8_t time_remaining = 	0;
//Uses GPIOA and TIM1 to configure PWM for RGB light on PA2 | PA8-PA10.
//RGB ENABLED VIA PA2
void init_rgb(){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    GPIOA->MODER &= ~(0x3f << (8 * 2));
	GPIOA->MODER |= (0x2a << (8 * 2));
	GPIOA->MODER &= ~(0x3 << 4);
	GPIOA->MODER |= (1 << 4);
	GPIOA->AFR[1] |= 0x222;
	TIM1->BDTR |= (1 << 15);
	TIM1->PSC = 1 - 1;
	TIM1->ARR = 48000 - 1;
    TIM1->CCMR1 |= 0x6060;
	TIM1->CCMR2 |= 0x60;
	TIM1->CCER |= 0x111;
	TIM1->CR1 = TIM_CR1_CEN;
}

void rgb_green(){ //enables rgb timer with green output
	TIM1->CCR1 = 48000 - 1; 
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 48000 - 1;
	GPIOA->ODR |= (1 << 2);
}

void rgb_red(){ //enables rgb timer with red output
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 48000 - 1;
	TIM1->CCR3 = 48000 - 1;
    TIM1->CR1 = TIM_CR1_CEN;
	GPIOA->ODR |= (1<<2);
}

void rgb_off(){
	GPIOA->ODR &= ~(1<<2);
}

//creates a timer that updates every second
//will update time remaining value on 7 segment

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
	time_remaining--;
	int8_t temp_time = time_remaining;
	rgb_off();	//rgb should always be off when question is starting
	//reset bits and enter new values for bit banging
	seg7_msg[2] &= ~0xFF;
	seg7_msg[3] &= ~0xFF;
	seg7_msg[2] |= num_to_7seg(temp_time / 10);
	seg7_msg[3] |= num_to_7seg(temp_time % 10);
	if(time_remaining <= 0){	//should be changed to account for actual answers
		rgb_red();
		toggle_question_tim();
	}
}

void toggle_question_tim(){
    TIM14->CR1 ^= TIM_CR1_CEN;
	time_remaining = QUESTION_TIME;
}
///////////////////////////////////////////////////
//BITBANGING FOR 7SEG
///////////////////////////////////////////////////

//setup (PB12 | PB13 | PB15) as (CS | SCK | SDI)
void seg7_init_bb(){
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(0xff << (12*2));
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
	while(time_remaining != 0){
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

int main(void){
    internal_clock();
	seg7_init_bb();
    init_question_tim();
	toggle_question_tim();
	drive_7seg();
    init_rgb();

}
/*
NOTES FOR GAME LOGIC:
Start of question must toggle question timer
Answer checking needs to include time_remaining
timer should turn off when answer is given
*/