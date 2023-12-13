#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <EEPROM.h>
#include "LedControl.h" // Include LedControl library for controlling the LED matrix
#include "utility.h"
#include "memory.h"

// bit shifter 
#define DATA_IN_PIN 12
#define CLOCK_PIN 11
#define LOAD_PIN 10
#define DRIVER_INDEX 1

// matrix generation
#define GRANULARITY_OF_CHANCE 10
#define CHANCE_OF_WALL 6 // 60%

// matrix animations
#define WINNING_FRAME_DISPLAY_TIME 300
#define WINNING_FRAME_NUMBER 5
#define GAME_START_FRAME_DISPLAY_TIME 500
#define GAME_START_FRAME_NUMBER 4
#define DEFAULT_FRAME_INDEX_VALUE 0

// deals with the map interaction and animation displaying
class gameMap
{   
private:
    LedControl m_ledControl = LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN, DRIVER_INDEX);
    byte m_matrixBrightness = 0;
    
    unsigned long m_lastPlayerBlink = 0;
    unsigned long m_lastBulletBlink = 0;
    unsigned long m_lastPowerUpBlink = 0;
    
    bool m_blinkPlayer = false;
    bool m_blinkBullet = false;
    bool m_blinkPowerUp = false;

    // for matrix display
    unsigned long m_lastFrameTime = 0;
    byte m_currentFrameIndex = 0;

    unsigned long m_lastBulletMove = 0;

    // logic matrix (the map of the game)
    byte matrix[MATRIX_SIZE][MATRIX_SIZE] = 
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    // animation frames displayed when winning
    const byte m_winningMatrixVector[WINNING_FRAME_NUMBER][REAL_MATRIX_SIZE] = {
        {
            0b00011000,
            0b01111110,
            0b00011000,
            0b00001000,
            0b00111100,
            0b01011010,
            0b01011010,
            0b00100100
        },
        {
            0b00011000,
            0b01111110,
            0b00011000,
            0b00001000,
            0b01111110,
            0b10011001,
            0b00011000,
            0b00100100
        },
        {
            0b00011000,
            0b01111110,
            0b00011000,
            0b10001001,
            0b01111110,
            0b00011000,
            0b00011000,
            0b00100100
        },
        {
            0b00011000,
            0b01111110,
            0b10011001,
            0b01001010,
            0b00111100,
            0b00011000,
            0b00011000,
            0b00100100
        },
        {
            0b00011000,
            0b01111110,
            0b10011001,
            0b01001010,
            0b00111100,
            0b00011000,
            0b00011000,
            0b00100100
        }
    };

    // animation frames displayed at the start of the game
    const byte m_gameStartMatrixVector[GAME_START_FRAME_NUMBER][REAL_MATRIX_SIZE] = 
    {
        {
            0b00011000,
            0b01111110,
            0b00011000,
            0b00010011,
            0b01111110,
            0b01011000,
            0b00011000,
            0b00100100
        },
        {
            0b00011000,
            0b01111110,
            0b00011000,
            0b11001000,
            0b01111110,
            0b00011010,
            0b00011000,
            0b00100100
        }, 
        {
            0b00011000,
            0b01111110,
            0b00011000,
            0b00010011,
            0b01111110,
            0b01011000,
            0b00011000,
            0b00100100
        },
        {
            0b00011000,
            0b01111110,
            0b00011000,
            0b11001000,
            0b01111110,
            0b00011010,
            0b00011000,
            0b00100100
        }
    };

    const uint8_t m_gameIcons[NUMBER_OF_ICONS][REAL_MATRIX_SIZE] = {
        {
            0b00111100,
            0b01110000,
            0b11111000,
            0b11001100,
            0b00000110,
            0b11110011,
            0b01110001,
            0b01100000
        },
        {
            0b00011000,
            0b01111110,
            0b00011000,
            0b00001000,
            0b00111100,
            0b01011010,
            0b01011010,
            0b00100100
        },
        {
            0b00111100,
            0b01100110,
            0b01100110,
            0b00110000,
            0b00011000,
            0b00011000,
            0b00000000,
            0b00011000
        }
    };

    // function that checks what is in the logic matrix of the game on realRow and realCol
    // and displays it as a player, bullet or anything else on the real 8x8 led matrix 
    void displayElement(int realRow, int realCol, int p_row, int p_col);

        // prints on the logical matrix starting at 0, 0 the frame stored in the matrix vector
    void printMatrixFrame(const byte p_matrixVector[][REAL_MATRIX_SIZE], byte p_frameToPrint);

public:
    // commands that set up the bit shifter to display the matrix as we want 
    // (brightness, turns off power saving and clears the display)
    void initMatrix();

    // changes a byte in the logic matrix at the position of xPos and yPos with the newValue
    void setPositionValue(int p_xPos, int p_yPos, int p_newValue);

    // goes through the steps to display the piece on the logic matrix that the player is in
    // on the real led matrix
    void updateDisplay(int p_xPosPlayer, int p_yPosPlayer);

    // checks if the x and y coordonates are within the limints of the logical matrix
    bool isWithinBounds(int p_xPos, int p_yPos);

    // checks if the byte at the xPos and yPos on the logical matrix is the given mapElement
    // (wall or player or bullet or whatever else)
    bool isMapElement(const byte p_mapElement, int p_xPos, int p_yPos);

    // generates a new level, placing walls power ups and clearing the area around the player
    void generateMap();

    // goes through the whole matrix and checks if there are any walls left    
    bool checkWinningCondition();

    // returns the number of walls left on the map
    int getWallsLeft();

    // sets the matrix brightness updating the m_newBrightness with the new value in the parameter
    void setMatrixBrightness(byte p_newBrightness);

    // returns the current matrix brightness value
    byte getMatrixBrightness();

    // increments the current matrix brightness value and assigns it
    byte incrementMatrixBrightness();

    // decrements the current matrix brightness value and assigns it
    byte decrementMatrixBrightness();

    ////////////////////////// animation and graphics related

    // deals with printing all of the frames in order of the winning animation
    bool printWinningMatrixAnimation();

    // deals with printing all of the frames in order of the animation 
    // played at the start of the game
    bool printStartGameMatrixAnimation();

    // sets all of the animation values at their default value
    void refreshAnimationValues();

    // displays the settings wheel on the matrix
    void displayIcon(const byte p_iconIndex);

    ///////////////////////////// debug purposes

    // for debug -> generates a map with a single wall near the player
    void printEmptyMatrix();
};

#endif