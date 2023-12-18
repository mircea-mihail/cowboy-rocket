#ifndef UTILITY_H
#define UTILITY_H

/////////////////////////// pins
// still got a free PWM pin 9

// player pins
#define JS_X_PIN A0
#define JS_Y_PIN A1
#define BUTTON_PIN 13
#define BACK_BUTTON_PIN A3

// LCD display pins
#define RESET A5
#define ENABLE 8
#define DATA4 7
#define DATA5 A4
#define DATA6 2
#define DATA7 4
#define LCD_CONTRAST 10
#define LCD_BRIGHTNESS 5

// bit shifter pins
#define DATA_IN_PIN 12
#define CLOCK_PIN 6
#define LOAD_PIN 3

// for reading outside brightness
#define BRIGHTNESS_PIN A2

// buzzer pin
#define BUZZER_PIN 11

// utility
#define MATRIX_ADDRESS 0
#define MATRIX_SIZE 16
#define REAL_MATRIX_SIZE 8
#define REAL_MATRIX_MIDDLE (REAL_MATRIX_SIZE / 2)
#define MATRIX_MIDDLE (MATRIX_SIZE / 2)

// generallys useful defines

// game states
#define GAME_IN_GAME 0
#define GAME_IN_MENU 1
#define GAME_IN_START_ANIMATION 2
#define GAME_LOST 3
#define GAME_WON 4
#define GAME_IN_MENU 5
#define GAME_BETWEEN_LEVELS 6

// menu states
#define MENU_IN_START_GAME 0 
#define MENU_IN_HIGH_SCORES 1
#define MENU_IN_SETTINGS 2
#define MENU_IN_HOW_TO_PLAY 3
#define MENU_IN_ABOUT 4

#define LOWEST_MENU_STATE MENU_IN_START_GAME
#define HIGHEST_MENU_STATE MENU_IN_ABOUT
// Not options but valid states
#define MENU_IN_GAME (HIGHEST_MENU_STATE + 1)

// menu time related
#define CYCLE_DELAY_MILLIS 400

// matrix occupation related
#define MAP_NUMBER_OF_ORIENTATIONS 4

#define MAP_EMPTY 0
#define MAP_WALL 1
#define MAP_PLAYER 2
#define MAP_BULLET 3
#define MAP_POWER_UP 4
#define MAP_ENEMY 5

// facing direction of player/bullet/other things
#define DIRECTION_UP 0
#define DIRECTION_LEFT 1 
#define DIRECTION_DOWN 2
#define DIRECTION_RIGHT 3

// bullet defines
#define BULLET_TICK_RATE 200
#define SHOOTING_COOLDOWN (BULLET_TICK_RATE)
#define DEFAULT_BULLET_RANGE 2
#define BULLET_BLINK_INTERVAL 30

// player defines 
#define PLAYER_MOVEMENT_MILLIS 400
#define PLAYER_BLINK_INTERVAL 200

// map elements defines
#define POWER_UP_BLINK_INTERVAL 500
#define ENEMY_BLINK_INTERVAL 100

// animation defines
#define DEFAULT_TIME_VAL 0

// sensor defines
#define MAX_BRIGHTNESS_INPUT 801
#define MIN_BRIGHTNESS_INPUT 800

#define MAX_MATRIX_BRIGHTNESS 15
#define MIN_MATRIX_BRIGHTNESS 0

// joystick 
#define JS_DEFAULT_VALUE 512
#define MIN_JS_THRESHOLD 312
#define MAX_JS_THRESHOLD 712

// icons for the menu
#define NUMBER_OF_ICONS 6
#define ICON_HAMMER 0
#define ICON_START 1
#define ICON_ABOUT 2
#define ICON_WINNERS 3
#define ICON_GAME_PAD 4
#define ICON_LVL_UP 5

// player name
#define LETTERS_IN_NAME 4

// levels
#define NUMBER_OF_LEVELS 5
#define FIRST_LEVEL 1

// difficulty
#define DIFFICULTY_EASY 1
#define DIFFICULTY_MEDIUM 2
#define DIFFICULTY_COWBOY 3

#define PLAYER_DEFAULT_LIVES 5

// reverses the direction. If it is up it makes it down. If it's right makes it left
#define reverseDirection(direction) (direction = (direction + (MAP_NUMBER_OF_ORIENTATIONS/2)) % MAP_NUMBER_OF_ORIENTATIONS) 
#define turnDirection90Deg(direction) (direction = (direction + 1) % MAP_NUMBER_OF_ORIENTATIONS)

// when a column in a matrix is encoded in a byte this functions returns the saught after bit value
#define getColValFromByte(byteValue, col) ((byteValue >> col) & 1)

// returns the absoulte value of a number
inline int absolute(int a) 
{
    if(a < 0)
    {
        return a * -1;
    }
    else 
    {
        return a;
    }
}

#endif