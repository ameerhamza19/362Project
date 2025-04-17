#include "stm32f0xx.h"

// Keypad configuration: 4 rows, 4 columns on PC1-PC8
#define ROW1_PORT GPIOC
#define ROW1_PIN  GPIO_PIN_1
#define ROW2_PORT GPIOC
#define ROW2_PIN  GPIO_PIN_2
#define ROW3_PORT GPIOC
#define ROW3_PIN  GPIO_PIN_3
#define ROW4_PORT GPIOC
#define ROW4_PIN  GPIO_PIN_4
#define COL1_PORT GPIOC
#define COL1_PIN  GPIO_PIN_5
#define COL2_PORT GPIOC
#define COL2_PIN  GPIO_PIN_6
#define COL3_PORT GPIOC
#define COL3_PIN  GPIO_PIN_7
#define COL4_PORT GPIOC
#define COL4_PIN  GPIO_PIN_8

// Keypad layout (only A, B, C, D used)
char keypad[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void SystemClock_Config(void);
void GPIO_Init(void);
void UART2_Init(void);
char Keypad_Scan(void);
void UART2_Transmit(char c);
void Delay_ms(uint32_t ms);

int main(void) {
    SystemClock_Config();
    GPIO_Init();
    UART2_Init();

    char key;
    while (1) {
        key = Keypad_Scan();
        if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
            UART2_Transmit(key);
            Delay_ms(200); // Debounce delay
        }
    }
}

char Keypad_Scan(void) {
    // Set columns as outputs
    ROW1_PORT->MODER &= ~(3UL << (2 * 1)); // Rows input
    ROW2_PORT->MODER &= ~(3UL << (2 * 2));
    ROW3_PORT->MODER &= ~(3UL << (2 * 3));
    ROW4_PORT->MODER &= ~(3UL << (2 * 4));
    ROW1_PORT->PUPDR |= (1UL << (2 * 1));  // Pull-up
    ROW2_PORT->PUPDR |= (1UL << (2 * 2));
    ROW3_PORT->PUPDR |= (1UL << (2 * 3));
    ROW4_PORT->PUPDR |= (1UL << (2 * 4));

    COL1_PORT->MODER |= (1UL << (2 * 5));  // Columns output
    COL2_PORT->MODER |= (1UL << (2 * 6));
    COL3_PORT->MODER |= (1UL << (2 * 7));
    COL4_PORT->MODER |= (1UL << (2 * 8));

    // Scan columns
    for (int col = 0; col < 4; col++) {
        // Set all columns high
        COL1_PORT->ODR |= COL1_PIN;
        COL2_PORT->ODR |= COL2_PIN;
        COL3_PORT->ODR |= COL3_PIN;
        COL4_PORT->ODR |= COL4_PIN;

        // Set current column low
        if (col == 0) COL1_PORT->ODR &= ~COL1_PIN;
        else if (col == 1) COL2_PORT->ODR &= ~COL2_PIN;
        else if (col == 2) COL3_PORT->ODR &= ~COL3_PIN;
        else COL4_PORT->ODR &= ~COL4_PIN;

        // Check rows
        if (!(ROW1_PORT->IDR & ROW1_PIN)) return keypad[0][col];
        if (!(ROW2_PORT->IDR & ROW2_PIN)) return keypad[1][col];
        if (!(ROW3_PORT->IDR & ROW3_PIN)) return keypad[2][col];
        if (!(ROW4_PORT->IDR & ROW4_PIN)) return keypad[3][col];
    }
    return '\0'; // No key pressed
}

void UART2_Transmit(char c) {
    while (!(USART2->ISR & USART_ISR_TXE)); // TX empty
    USART2->TDR = c; // Send 
    while (!(USART2->ISR & USART_ISR_TC)); // Wait for transmission 
}

void SystemClock_Config(void) {
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));
    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE);
}

void GPIO_Init(void) {
    // Enable GPIOC clock for keypad
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    //  GPIOA clock for UART2 
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    //  PA2 as UART2 TX (AF1)
    GPIOA->MODER |= (2UL << (2 * 2)); // Af
    GPIOA->AFR[0] |= (1UL << (4 * 2)); // AF1 for PA2
}

void UART2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    USART2->BRR = 8000000 / 9600;
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE; // Enable TX, UART
}

void Delay_ms(uint32_t ms) {
    // Simple delay 
    for (uint32_t i = 0; i < ms * 8000 / 4; i++) {
        __NOP();
    }
}
