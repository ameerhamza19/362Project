#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUESTIONS 10
#define MAX_CHOICES 4
#define MAX_PLAYERS 4

// Simulated output/input functions (to be replaced by actual hardware drivers)
void LCD_DisplayString(char *str);
char Keypad_GetChar();
void UART_Send(char c);
void init_all_peripherals();

typedef struct {
    char question[100];
    char choices[MAX_CHOICES][30];
    char correct_answer; // 'A', 'B', 'C', or 'D'
} TriviaQuestion;

TriviaQuestion questions[MAX_QUESTIONS] = {
    {
        "What is the capital of France?",
        {"A. Berlin", "B. Madrid", "C. Paris", "D. Rome"},
        'C'
    },
    {
        "Which planet is the Red Planet?",
        {"A. Earth", "B. Mars", "C. Venus", "D. Jupiter"},
        'B'
    },
    {
        "What is the largest ocean?",
        {"A. Indian", "B. Atlantic", "C. Pacific", "D. Arctic"},
        'C'
    },
    {
        "Which gas do plants absorb?",
        {"A. Oxygen", "B. Nitrogen", "C. Hydrogen", "D. CO2"},
        'D'
    },
    {
        "Who wrote 'Macbeth'?",
        {"A. Twain", "B. Dickens", "C. Shakespeare", "D. Poe"},
        'C'
    },
    {
        "What is H2O?",
        {"A. Oxygen", "B. Hydrogen", "C. Water", "D. Acid"},
        'C'
    },
    {
        "Fastest land animal?",
        {"A. Cheetah", "B. Horse", "C. Lion", "D. Gazelle"},
        'A'
    },
    {
        "Boiling point of water (°C)?",
        {"A. 90", "B. 100", "C. 110", "D. 120"},
        'B'
    },
    {
        "2 + 2 * 2 = ?",
        {"A. 6", "B. 8", "C. 4", "D. 10"},
        'A'
    },
    {
        "What year did WW2 end?",
        {"A. 1945", "B. 1940", "C. 1939", "D. 1942"},
        'A'
    }
};

int player_scores[MAX_PLAYERS] = {0};

void display_question(int q_index) {
    LCD_DisplayString(questions[q_index].question);
    for (int i = 0; i < MAX_CHOICES; i++) {
        LCD_DisplayString(questions[q_index].choices[i]);
    }
}

char get_player_answer() {
    char input = Keypad_GetChar();
    while (input < 'A' || input > 'D') {
        LCD_DisplayString("Invalid! A-D only.");
        input = Keypad_GetChar();
    }
    return input;
}

void play_round(int q_index, int player_num) {
    display_question(q_index);
    char answer = get_player_answer();

    if (answer == questions[q_index].correct_answer) {
        LCD_DisplayString("Correct!");
        player_scores[player_num]++;
    } else {
        LCD_DisplayString("Wrong!");
    }
}

void display_scores(int num_players) {
    for (int i = 0; i < num_players; i++) {
        char score_msg[30];
        sprintf(score_msg, "P%d Score: %d", i + 1, player_scores[i]);
        LCD_DisplayString(score_msg);
    }
}

int main(void) {
    init_all_peripherals();
    LCD_DisplayString("Enter # players (1-4):");
    char num_players = Keypad_GetChar();
    int players = num_players - '0';

    LCD_DisplayString("Enter # rounds (1-9):");
    char num_rounds = Keypad_GetChar();
    int rounds = num_rounds - '0';

    for (int r = 0; r < rounds; r++) {
        for (int p = 0; p < players; p++) {
            char msg[30];
            sprintf(msg, "Player %d's turn", p + 1);
            LCD_DisplayString(msg);

            int q_index = rand() % MAX_QUESTIONS;
            play_round(q_index, p);
        }
    }

    LCD_DisplayString("Game Over!");
    display_scores(players);

    while (1);
}
