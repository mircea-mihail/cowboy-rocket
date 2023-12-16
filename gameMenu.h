#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "utility.h"
#include "inputHwControl.h"
#include "gameMap.h"
#include "memory.h"

// lcd default characteristics
#define LCD_COLS 16
#define LCD_ROWS 2

#define LCD_INCREMENT_VAL 16

// printing defines
#define FIRST_LCD_ROW 0
#define SECOND_LCD_ROW 1

#define FIRST_LCD_COL 0
#define SECOND_LCD_COL 1

// menu cycling
#define INTRO_MESSAGE_MILLIS 3000
#define END_MESSAGE_MILLIS 3000

// settings submenu states:
#define IN_MATRIX_BRIGHTNESS 0
#define IN_LCD_BRIGHTNESS 1 
#define IN_LCD_CONTRAST 2
#define IN_ENTER_NAME 3
#define IN_TOGGLE_SOUND 4
#define IN_RESET_HIGH_SCORES 5

#define RETURN_FROM_SETTINGS 6 // always the biggest of the options

#define LOWEST_SETTINGS_STATE IN_MATRIX_BRIGHTNESS
#define HIGHEST_SETTINGS_STATE IN_RESET_HIGH_SCORES

// custom lcd characters
#define LCD_CHARACTER_HEIGHT 8
#define NUMBER_OF_CUSTOM_CHARACTERS 6

#define EMPTY_CHAR 0
#define SUN_CHAR 1
#define CONTRAST_CHAR 2
#define ARROW_RIGHT_CHAR 3
#define ARROW_LEFT_CHAR 4
#define ARROW_UP_CHAR 5

// miscelanious
#define PWM_RESOLUTION 255
#define SCORES_TO_SHOW_AT_A_TIME 2

// reset scores stages
#define NOT_SURE_YET 0 
#define ARE_YOU_SURE 1
#define SURE_SURE 2
#define ABSOLUTELY_POSITIVE 3

extern gameMap g_map;
extern bool g_disableSound;

class gameMenu
{
private:
    bool m_inSubmenu = false;
    bool m_inSettingsSubmenu = false;

    int m_state = MENU_IN_START_GAME;
    int m_settingsState = RETURN_FROM_SETTINGS;
    int m_previousSettingsState = IN_MATRIX_BRIGHTNESS;
    bool m_showFirstScores = true;

    bool m_changedState = true;
    unsigned long m_lastCycleTime = 0;

    bool m_showedIntroMessage = false;
    unsigned long m_introMessageTime = 0;

    bool m_showEndMessage = false;
    unsigned long m_endMessageTime = 0;

    unsigned long m_lastContrastChange = 0;
    unsigned long m_lastBrightnessChange = 0;
    unsigned long m_lcdScrollChange = 0;
    unsigned long m_lastMatrixBrightnessChange = 0;
    unsigned long m_lastNameArrowChange = 0;

    char m_nameArray[LETTERS_IN_NAME] = {'A', 'A', 'A', 'A'};
    int m_nameArrayIdx = 0;
    int m_wallsLeftOnMap = 0;  

    // menu variables:
    bool m_showAboutText = false;
    byte m_deleteHighScores = NOT_SURE_YET;

    byte m_lcdContrast = PWM_RESOLUTION/2;
    byte m_lcdBrightness = PWM_RESOLUTION/2;
    inputHwControl m_hwCtrl;
    LiquidCrystal m_lcd = LiquidCrystal(RESET, ENABLE, DATA4, DATA5, DATA6, DATA7);

    // custom LCD characters:
    byte m_customCharArray[NUMBER_OF_CUSTOM_CHARACTERS][LCD_CHARACTER_HEIGHT] = {
        {
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            B00000
        },
        {
            B00100,
            B10001,
            B01110,
            B01110,
            B01110,
            B10001,
            B00100,
            B00000
        },
        {
            B00000,
            B11111,
            B10001,
            B10011,
            B10111,
            B11111,
            B00000,
            B00000
        },
        {
            B00000,
            B00100,
            B00110,
            B11111,
            B11111,
            B00110,
            B00100,
            B00000
        },
        {
            B00000,
            B00100,
            B01100,
            B11111,
            B11111,
            B01100,
            B00100,
            B00000
        },
        {
            B00100,
            B01110,
            B11111,
            B01110,
            B01110,
            B01110,
            B00000,
            B00000
        }
    };

    /////////////////////////////////// INTRO OUTRO MESSAGES

    // ensures the display of the intro message (when, how, if and for how long)
    bool doIntroMessageSequence();

    // ensures the display of the end game message (when, how, if and for how long)
    bool doEndMessageSequence();

    // only does the printing of the acutal intro words
    void displayStartMessage();
    
    // only does the printing of the acutal end game words
    void displayEndMessage();

    /////////////////////////////////// GENERAL MENU UTILITY FUNCTIONS

    // updates the state and does other changes to prepare the object for the state change
    void changeState(int &p_state, int p_newState);

    // changes the current menu option by increasing/decreasing the menu state according to joystick input 
    void goToNextMenuOption(int &p_currentState, const int p_lowerBound, const int p_upperBound);

    // makes sure the given value is within its bounds and if not, modifies it to be
    void keepInBounds(int &p_value, const int p_lowerBound, const int p_upperBound);

    // used to refresh menu variables when going from a state to another
    void refreshMenuVariables();

    /////////////////////////////////// SUBMENU UTILITY FUNCTIONS

    // used to print percent hashes showing LCD related info in a graphical manner
    void printHashesLCD(int p_fillAmount);
    
    // used to print percent hashes showing Matrix related info in a graphical manner
    void printHashesForMatrix(int p_fillAmount);

    // updates the matrix brightness and does the logic to display it on the lcd
    void updateMatrixBrightness();

    // updates the lcd contrast and does the logic to display it on the lcd
    void updateLcdContrast();

    // updates the lcd brightness and does the logic to display it on the lcd
    void updateLcdBrightness();
    
    // displays up arrow under the appropriate selected name
    void displayNameCharArrow(int p_initialSpaceOffset, int p_spaceOffset, int p_numberOfOffsets);

    // gets from memory the winners' names and therir scores
    void getWinnersAndScores(char p_namesOfWinners[NUMBER_OF_SCORES_KEPT][SIZE_OF_NAME_IN_EEPROM], unsigned long p_winnerScores[NUMBER_OF_SCORES_KEPT]);

    // dispays once two winners offset from the first winner by offset
    void displayWinnersOnce(char p_namesOfWinners[NUMBER_OF_SCORES_KEPT][SIZE_OF_NAME_IN_EEPROM], unsigned long p_winnerScores[NUMBER_OF_SCORES_KEPT], const byte p_offset = 0);

    /////////////////////////////////// FUNCTIONS TO ENTER SUBMENUS    

    // the settings menu logic that orchestrates the changes between settings-menu options
    void goToSettingsMenu();

    // call this to go to change name submenu of the settings submenu
    void goToChangeName();

    /////////////////////////////////// MAIN MENU UTILITY FUNCTIONS
    
    // deals with the display of the about in main menu and in submenu
    void displayAboutMenu();

    // goes through the about submenu and exits/enters it depending on input
    void doAboutMenuLogic();

    // does quadruple checking before deleting high scores ( some people might be mad about the decision )
    void doResetScoresLogic();


public:
    // constructor, does the initializations and setups for the lcd
    gameMenu();

    // the main menu logic called in the main .ino file that ensures the changes between menu options
    int menuSequence();

    // initiates the printing of the main message by setting a variable to true
    void printEndMessage();
    
    // used to reset the menu state to begin printing all the settings options after finishing a game
    void setStateToDefault();
};

#endif