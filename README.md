# Trivia Game Using STM32 Microcontrollers

## Project Description
This project implements a multiplayer trivia game, inspired by *Who Wants to Be a Millionaire?*, using two STM32 microcontrollers, TFT LCD displays, a keypad, and a seven-segment display. The game supports 1-4 players, each answering timed multiple-choice questions. The system tracks scores and rounds while utilizing interrupts, timers, DMA, and SPI for efficient functionality.

### Features
- **Multiple Players (1-4)**: Players answer randomly selected trivia questions.
- **TFT LCD Display**: Displays questions and answer choices.
- **Keypad Input**: Players input their answers using a 16-button alphanumeric keypad.
- **Seven-Segment Display**: Shows countdown timer for answering.
- **Dual STM32 Communication**: Two STM32 microcontrollers communicate via UART.
- **SD Card Storage**: Stores player scores and game data.
- **RGB LED Feedback**: Indicates correct and incorrect answers using PWM-controlled LEDs.

### Contributors & Responsibilities
- **Ahmed**: Implement SPI communication for MicroSD card data storage.
- **Hamza**: Enable UART communication between the two STM32 microcontrollers.
- **Osan**: Display questions and choices on TFT LCD via SPI.
- **Austin**: Implement RGB LED indication for answer correctness using PWM.

## Hardware Components
- **2x STM32F091 Microcontroller Dev Boards**
- **2x TFT LCD Screens**
- **1x MicroSD Card**
- **1x DD-14881 16-Button Alphanumeric Keypad**
- **1x TDCR1050 Seven-Segment Display**
- **RGB LEDs**

## Software & Functionality
- The game stores player data, tracks scores, and randomly selects trivia questions.
- It prompts players to input the number of players and rounds.
- Error handling ensures valid inputs and smooth operation.
- Utilizes **interrupts, timers, DMA, and SPI** to optimize response time and efficiency.

## Timeline
### **Week 8-9**
- Review datasheets and documentation for hardware components (by *3/8/2025*).
- Plan software structure and assess reusable code from past labs (by *3/9/2025*).
- Implement basic input handling and display output (by *3/13/2025*).

### **Week 10**
- Implement player setup (total score, rounds, etc.).
- Define game logic, scoring system (without hardware testing) (by *3/17/2025*).
- Develop question selection, randomization, and answer validation (by *3/23/2025*).
- Design schematic for breadboard implementation (by *3/23/2025*).

### **Week 11**
- Finalize embedded code and create test benches (by *3/28/2025*).
- Start hardware integration and breadboard setup (by *3/29/2025*).

### **Week 12**
- Connect and test displays, keypad, and seven-segment display (by *4/4/2025*).
- Implement timers and interrupts for countdowns and response timing (by *4/5/2025*).
- Debug SPI, DMA, and communication protocols (by *4/6/2025*).

### **Week 13**
- Finalize breadboard testing (by *4/10/2025*).
- Complete UART communication between STM32 microcontrollers (by *4/11/2025*).
- Ensure MicroSD card storage functionality (by *4/11/2025*).
- Conduct full-system testing with multiple players (by *4/12/2025*).
- Schedule and present project demo (by *4/13/2025*).

### **Week 14-15**
- Final project demonstration to GTA (by *4/22/2025*).

## Related Projects
1. **ChatGPT-Driven Arduino Quiz Game**: Uses an Arduino board and OLED display with ChatGPT API to generate quiz questions. Could be adapted for STM32 platforms.
   - [Video Link](https://www.youtube.com/watch?v=UAzOPjrwaa4)
2. **STM32 + OLED = Display Anything**: Demonstrates interfacing a 128x64 OLED display with STM32, useful for displaying trivia questions.
   - [Video Link](https://www.youtube.com/watch?v=97_Vyph9EzM)

---
## How to Run the Project
1. Flash the firmware onto both STM32 microcontrollers.
2. Connect the hardware as per the schematic.
3. Power up the system and follow on-screen prompts to configure the game.
4. Players input answers using the keypad while the seven-segment display tracks the timer.

### Future Improvements
- Adding wireless connectivity for remote play.
- Expanding the question database for a more diverse game experience.
- Enhancing UI elements on TFT LCD for better user interaction.

### License
This project is open-source under the **MIT License**. Feel free to modify and expand upon it!

