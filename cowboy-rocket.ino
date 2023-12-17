#include "gameMap.h"
#include "matrixEntity.h"
#include "bullet.h"
#include "bulletList.h"
#include "player.h"
#include "score.h"
#include "gameMenu.h"
#include "inputHwControl.h"

//to do in order:
// display the end stats at the end
// make levels 

// check tasks

// do enemies 

// skip animation 
#define ANIMATION_SKIP_DELAY_MS 500

gameMap g_map;
player g_player1(MATRIX_MIDDLE, MATRIX_MIDDLE);
bulletList g_bulletList;
score g_score;
inputHwControl g_hwCtrl;
gameMenu g_menu;

byte g_gameState;
byte g_menuState;

// frame display
bool g_finishedAnimiation = false;
unsigned long g_timeForLastFrame = 0;
unsigned long g_timeForBulletUpdate = 0;
bool g_disableSound = false;

void initAllHw()
{
    // button and js
    pinMode(JS_X_PIN, INPUT);
    pinMode(JS_Y_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);   
    pinMode(BACK_BUTTON_PIN, INPUT_PULLUP);

    // matrix
    pinMode(DATA_IN_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LOAD_PIN, OUTPUT);

    // LCD
    pinMode(RESET, OUTPUT);
    pinMode(ENABLE, OUTPUT);
    pinMode(DATA4, OUTPUT);
    pinMode(DATA5, OUTPUT);
    pinMode(DATA6, OUTPUT);
    pinMode(DATA7, OUTPUT);
    pinMode(LCD_CONTRAST, OUTPUT);

    // brightness sensor
    pinMode(BRIGHTNESS_PIN, INPUT);    
}

// call this when starting a new level
void startLevelSequence()
{
    g_gameState = GAME_IN_GAME;    

    g_map.refreshAnimationValues();               
    g_map.generateMap();

    char playerName[LETTERS_IN_NAME];
    g_menu.getPlayerName(playerName);
    g_menu.resetRunSpecificVariables();
    g_menu.resetLevel();

    g_score.startCounting(playerName);
    g_score.printHighScores();
    
    g_player1.resetValues();
    g_player1.goToDefaultPosition();

    // debug
    g_map.printEmptyMatrix();
    // g_score.clearScores();
}

void goToNextLevelSequence()
{
    g_map.refreshAnimationValues();               
    g_map.generateMap();

    g_menu.resetRunSpecificVariables();
    g_menu.goToNextLevel();
    
    g_player1.resetValues();
    g_player1.goToDefaultPosition();

    // debug
    g_map.printEmptyMatrix();
}

// adjust brightness using the sensor
void adjustBrightness()
{
    int outsideBrightness = g_hwCtrl.getBrightness();
    if(outsideBrightness < MIN_BRIGHTNESS_INPUT)
    {
        g_map.setMatrixBrightness(MIN_MATRIX_BRIGHTNESS);
    }  
    else if(outsideBrightness > MAX_BRIGHTNESS_INPUT)
    {
        g_map.setMatrixBrightness(MAX_MATRIX_BRIGHTNESS);
    } 
    else
    {
        int newBrightness = map(outsideBrightness, MIN_BRIGHTNESS_INPUT, MAX_BRIGHTNESS_INPUT, MIN_MATRIX_BRIGHTNESS, MAX_MATRIX_BRIGHTNESS);
        g_map.setMatrixBrightness(newBrightness);
    } 
}   

void doInGameRoutine()
{
    g_bulletList.updateBullets();
    g_player1.updatePosition();

    int xPosNewBullet, yPosNewBullet;
    byte bulletDirection;
    bool isExplodingType;

    if(g_player1.shoot(xPosNewBullet, yPosNewBullet, bulletDirection, isExplodingType))
    {
        g_bulletList.setLastBulletUpdate();
        g_bulletList.addBulletNode(new bullet(xPosNewBullet, yPosNewBullet, bulletDirection, isExplodingType));
        
    }

    int xPosPlayer, yPosPlayer;
    g_player1.getCoordonates(xPosPlayer, yPosPlayer);

    g_map.updateDisplay(xPosPlayer, yPosPlayer);

    // logic for getting to next level
    if((g_map.checkWinningCondition() && g_menu.getLevel() != NUMBER_OF_LEVELS))
    {
        if(g_timeForBulletUpdate == DEFAULT_TIME_VAL)
        {
            g_timeForBulletUpdate = millis();
        }
        if(millis() - g_timeForBulletUpdate > WINNING_FRAME_DISPLAY_TIME)
        {
            g_menu.resetRunSpecificVariables();
            g_timeForBulletUpdate = DEFAULT_TIME_VAL;

            goToNextLevelSequence();
        }
    }        

    // logic for the end of the run
    if((g_map.checkWinningCondition() && g_menu.getLevel() == NUMBER_OF_LEVELS) || g_player1.getLives() == 0)
    {
        if(g_timeForBulletUpdate == DEFAULT_TIME_VAL)
        {
            g_timeForBulletUpdate = millis();
        }
        if(millis() - g_timeForBulletUpdate > WINNING_FRAME_DISPLAY_TIME)
        {
            g_menu.resetRunSpecificVariables();
            g_gameState = GAME_WON;
            g_timeForBulletUpdate = DEFAULT_TIME_VAL;

            g_score.stopCounting();
        }
    }
}

void doInStartAnimationRoutine()
{
    // skip animation 
    g_menu.displayEmptyStatsOnce();

    if(g_hwCtrl.pressedBackButton())
    {
        g_finishedAnimiation = true;
    }
    // places the player on 0 0 to display what is printed on the "map" during animation
    if(!g_finishedAnimiation && g_map.printStartGameMatrixAnimation())
    {
        if(g_timeForLastFrame == DEFAULT_TIME_VAL)
        {
            g_timeForLastFrame = millis();
        }
        g_finishedAnimiation = true;   
    }

    if(millis() - g_timeForLastFrame > GAME_START_FRAME_DISPLAY_TIME && g_finishedAnimiation)
    {
        g_timeForLastFrame = DEFAULT_TIME_VAL;
        g_finishedAnimiation = false;

        g_gameState = GAME_IN_GAME;
        
        startLevelSequence();
    }
}

void doInWinningStateRoutine()
{
    g_menu.setInAnimationVar(true);
    // skip animation 
    if(g_hwCtrl.pressedBackButton())
    {
        g_finishedAnimiation = true;
    }

    // winning animation
    if(!g_finishedAnimiation && g_map.printWinningMatrixAnimation())
    {
        if(g_timeForLastFrame == DEFAULT_TIME_VAL)
        {
            g_timeForLastFrame = millis();
        }
        g_finishedAnimiation = true;
    }

    // if animation is finished go to start animation
    if(millis() - g_timeForLastFrame > WINNING_FRAME_DISPLAY_TIME && g_finishedAnimiation)
    {
        g_timeForLastFrame = DEFAULT_TIME_VAL;
        g_finishedAnimiation = false;

        g_gameState = GAME_IN_MENU;
        g_menu.setStateToDefault();
        g_menu.printEndMessage();
        g_menu.setInAnimationVar(false);
    }
}


void setup()
{
    Serial.begin(115200);
    initAllHw();

    g_gameState = GAME_IN_MENU;
    g_map.initMatrix();
}

void loop() 
{
    //adjustBrightness();

    g_menuState = g_menu.menuSequence();

    switch(g_gameState)
    {
        case GAME_IN_START_ANIMATION:
            doInStartAnimationRoutine();
            break;
        
        case GAME_IN_GAME:
            doInGameRoutine();
            break;

        case GAME_WON:
            doInWinningStateRoutine();
            break;            

        case GAME_IN_MENU:
            if(g_menuState == MENU_IN_GAME)
            {
                g_gameState = GAME_IN_START_ANIMATION;
                g_map.refreshAnimationValues();
            }
            break;

        default:
            break;
    }
} 