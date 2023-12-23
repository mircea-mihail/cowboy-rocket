#include "gameMap.h"
#include "matrixEntity.h"
#include "bullet.h"
#include "bulletList.h"
#include "player.h"
#include "score.h"
#include "gameMenu.h"
#include "inputHwControl.h"
#include "mySounds.h"
#include "enemy.h"

// skip animation 
#define ANIMATION_SKIP_DELAY_MS 500
#define LVL_UP_ICON_MILLIS 1000
#define WAIT_TIME_AFTER_LEVEL_FINISH 600
#define MAX_ENEMY_COUNT 10

gameMap g_map;
player g_player1(MATRIX_MIDDLE, MATRIX_MIDDLE);
bulletList g_bulletList;
score g_score;
inputHwControl g_hwCtrl;
gameMenu g_menu;
enemy *g_enemyArray[MAX_ENEMY_COUNT];

byte g_gameState;
byte g_menuState;

// frame display
bool g_finishedAnimiation = false;
unsigned long g_timeForLastFrame = 0;
unsigned long g_timeForBulletUpdate = 0;
bool g_disableSound = false;
bool g_shownLvlUpIcon = false;
unsigned long g_timeForLvlUpIcon = 0;
unsigned long g_lastNoteTime = 0; 

enemySpawnCoordonates easyEnemySpawn[NUMBER_OF_EASY_SPAWNS];
enemySpawnCoordonates hardEnemySpawn[NUMBER_OF_HARD_SPAWNS];

void playEnemyDeathNote()
{
    if(!g_disableSound)
    {
        tone(BUZZER_PIN, FREQ_ENEMY_DEATH, ENEMY_DEATH_SOUND_DURATION);
    }
}

void initEnemySpawns()
{
    easyEnemySpawn[0].m_xPos = FIRST_EASY_ENEMY_X;
    easyEnemySpawn[0].m_yPos = FIRST_EASY_ENEMY_Y;
    easyEnemySpawn[1].m_xPos = SECOND_EASY_ENEMY_X;
    easyEnemySpawn[1].m_yPos = SECOND_EASY_ENEMY_Y;
    easyEnemySpawn[2].m_xPos = THIRD_EASY_ENEMY_X;
    easyEnemySpawn[2].m_yPos = THIRD_EASY_ENEMY_Y;
    easyEnemySpawn[3].m_xPos = FOURTH_EASY_ENEMY_X;
    easyEnemySpawn[3].m_yPos = FOURTH_EASY_ENEMY_Y;
    easyEnemySpawn[4].m_xPos = FIFTH_EASY_ENEMY_X;
    easyEnemySpawn[4].m_yPos = FIFTH_EASY_ENEMY_Y;
    easyEnemySpawn[5].m_xPos = SIXTH_EASY_ENEMY_X;
    easyEnemySpawn[5].m_yPos = SIXTH_EASY_ENEMY_Y;

    hardEnemySpawn[0].m_xPos = FIRST_HARD_ENEMY_X;
    hardEnemySpawn[0].m_yPos = FIRST_HARD_ENEMY_Y;
    hardEnemySpawn[1].m_xPos = SECOND_HARD_ENEMY_X;
    hardEnemySpawn[1].m_yPos = SECOND_HARD_ENEMY_Y;
    hardEnemySpawn[2].m_xPos = THIRD_HARD_ENEMY_X;
    hardEnemySpawn[2].m_yPos = THIRD_HARD_ENEMY_Y;
    hardEnemySpawn[3].m_xPos = FOURTH_HARD_ENEMY_X;
    hardEnemySpawn[3].m_yPos = FOURTH_HARD_ENEMY_Y;
}

void initEnemies()
{
    for(int enemyIdx = 0; enemyIdx < MAX_ENEMY_COUNT; enemyIdx++)
    {
        if(g_enemyArray[enemyIdx] != nullptr)
        {
            delete(g_enemyArray[enemyIdx]);
            g_enemyArray[enemyIdx] = nullptr;
        }
    }
    byte currentLevel = g_menu.getLevel();
    //spawn them depending on the level
    
    // 6 max ez enemies 
    // 4 max hard enemies 
    // 1 -> 2 ez
    // 2 -> 4 ez 
    // 3 -> 4 ez 1 hard
    // 4 -> 6 ez 2 hard
    // 5 -> 6 ez 4 hard
    int numberOfEasyEnemies;
    int numberOfHardEnemies;

    switch(currentLevel)
    {
        case FIRST_LEVEL:
            numberOfEasyEnemies = EASY_ENEMIES_ON_FIRST_LVL;
            numberOfHardEnemies = HARD_ENEMIES_ON_FIRST_LVL;
            break;

        case SECOND_LEVEL:
            numberOfEasyEnemies = EASY_ENEMIES_ON_SECOND_LVL;
            numberOfHardEnemies = HARD_ENEMIES_ON_SECOND_LVL;
            break;

        case THIRD_LEVEL:
            numberOfEasyEnemies = EASY_ENEMIES_ON_THIRD_LVL;
            numberOfHardEnemies = HARD_ENEMIES_ON_THIRD_LVL;
            break;

        case FOURTH_LEVEL:
            numberOfEasyEnemies = EASY_ENEMIES_ON_FOURTH_LVL;
            numberOfHardEnemies = HARD_ENEMIES_ON_FOURTH_LVL;
            break;

        case FIFTH_LEVEL:
            numberOfEasyEnemies = EASY_ENEMIES_ON_FIFTH_LVL;
            numberOfHardEnemies = HARD_ENEMIES_ON_FIFTH_LVL;
            break;
    }

    // numberOfEasyEnemies = 0;
    // numberOfHardEnemies = 4;
    // g_map.clearMapAround(5, 5);
    // g_enemyArray[0] = new enemy(5, 5, DIRECTION_DOWN, HARD_TYPE);

    for(int enemyIdx = 0; enemyIdx < numberOfEasyEnemies; enemyIdx++)
    {
        int ezPosIdx = random(0, NUMBER_OF_EASY_SPAWNS);
        int xEnemyPos = easyEnemySpawn[ezPosIdx].m_xPos;
        int yEnemyPos = easyEnemySpawn[ezPosIdx].m_yPos;
        g_map.clearMapAround(xEnemyPos, yEnemyPos);

        byte direction = random(0, MAP_NUMBER_OF_ORIENTATIONS);
        byte easyType = random(0, NUMBER_OF_EASY_TYPES);

        g_enemyArray[enemyIdx] = new enemy(xEnemyPos, yEnemyPos, direction, easyType);
    }

    if(currentLevel == FIFTH_LEVEL)
    {
        // on the last level make sure that all the hard enemies are in their designated spots
        for(int enemyIdx = numberOfEasyEnemies; enemyIdx < numberOfEasyEnemies + numberOfHardEnemies; enemyIdx++)
        {
            int hardPosIdx = enemyIdx - numberOfEasyEnemies;
            int xEnemyPos = hardEnemySpawn[hardPosIdx].m_xPos;
            int yEnemyPos = hardEnemySpawn[hardPosIdx].m_yPos;

            g_map.clearMapAround(xEnemyPos, yEnemyPos);

            byte direction = random(0, MAP_NUMBER_OF_ORIENTATIONS);

            g_enemyArray[enemyIdx] = new enemy(xEnemyPos, yEnemyPos, direction, HARD_TYPE);
        }    
    }
    else
    {
        for(int enemyIdx = numberOfEasyEnemies; enemyIdx < numberOfEasyEnemies +  numberOfHardEnemies; enemyIdx++)
        {
            int hardPosIdx = random(0, NUMBER_OF_HARD_SPAWNS);
            int xEnemyPos = hardEnemySpawn[hardPosIdx].m_xPos;
            int yEnemyPos = hardEnemySpawn[hardPosIdx].m_yPos;

            g_map.clearMapAround(xEnemyPos, yEnemyPos);

            byte direction = random(0, MAP_NUMBER_OF_ORIENTATIONS);

            g_enemyArray[enemyIdx] = new enemy(xEnemyPos, yEnemyPos, direction, HARD_TYPE);
        }
    }
}

void doEnemyRoutine()
{
    int ezEnemiesNr = 0;
    int hardEnemiesNr = 0;

    for(int enemyIdx = 0; enemyIdx < MAX_ENEMY_COUNT; enemyIdx++)
    {
        if(g_enemyArray[enemyIdx] != nullptr)
        {
            if(g_enemyArray[enemyIdx]->getType() == HARD_TYPE)
            {
                hardEnemiesNr ++;
            }
            else
            {
                ezEnemiesNr ++;
            }

            int xEnemyPos, yEnemyPos;
            int xPlayerPos, yPlayerPos;

            g_player1.getCoordonates(xPlayerPos, yPlayerPos);
            g_enemyArray[enemyIdx]->getCoordonates(xEnemyPos, yEnemyPos);

            if(xPlayerPos == xEnemyPos && yPlayerPos == yEnemyPos)
            {
                g_menu.setBlinkDisplayTimer();
                g_player1.takeDamage();
                g_enemyArray[enemyIdx]->damageEnemy();
            }

            if(g_bulletList.checkBulletOnPos(xEnemyPos, yEnemyPos))
            {
                g_enemyArray[enemyIdx]->damageEnemy();
            }

            if(g_enemyArray[enemyIdx]->getLives() == 0)
            {
                playEnemyDeathNote();
                int wallsDestroyed = 0;
                int enemiesKilled = 1;
                g_score.updateScore(wallsDestroyed, g_enemyArray[enemyIdx]->getType(), enemiesKilled);
                
                delete(g_enemyArray[enemyIdx]);
                g_enemyArray[enemyIdx] = nullptr;
            }
            else
            {
                g_enemyArray[enemyIdx]->updatePosition();
            }
        }
    }

}

void playMelody()
{
    if(g_disableSound)
    {
        return;
    }

    if(millis() - g_lastNoteTime > TIME_BETWEEN_NOTES * NUMBER_OF_END_NOTES)
    {
        g_lastNoteTime = millis();
    }
    else if(millis() - g_lastNoteTime > TIME_BETWEEN_NOTES * SECOND_NOTE_IDX)
    {
        tone(BUZZER_PIN, FREQ_END_THIRD_NOTE, TIME_OF_NOTE);
    }
    else if(millis() - g_lastNoteTime > TIME_BETWEEN_NOTES)
    {
        tone(BUZZER_PIN, FREQ_END_SECOND_NOTE, TIME_OF_NOTE);
    }
    else
    {
        tone(BUZZER_PIN, FREQ_END_FIRST_NOTE, TIME_OF_NOTE);
    }
}

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

    // buzzer
    pinMode(BUZZER_PIN, OUTPUT);
}

// call this when starting a new level
void startLevelSequence()
{
    g_gameState = GAME_IN_GAME;    

    char playerName[LETTERS_IN_NAME];
    g_menu.getPlayerName(playerName);
    g_menu.resetRunSpecificVariables();
    g_menu.resetLevel();

    g_map.refreshAnimationValues();               
    g_map.generateMap(g_menu.getLevel());

    g_score.startCounting(playerName);
    
    g_player1.resetValues();
    g_player1.goToDefaultPosition();
    g_player1.setLives(PLAYER_DEFAULT_LIVES - g_menu.getDifficulty());

    // debug
    // g_map.printEmptyMatrix();
    // g_score.clearScores();
    initEnemies();

}

void goToNextLevelSequence()
{
    playMelody();

    if(!g_shownLvlUpIcon)
    {
        g_shownLvlUpIcon = true;
        g_timeForLvlUpIcon = millis();
        g_map.displayIcon(ICON_LVL_UP); 
        g_gameState = GAME_BETWEEN_LEVELS;
        g_menu.displayLvlUp();
        g_menu.setInAnimationVar(true);
    }
    else
    {
        if(millis() - g_timeForLvlUpIcon > LVL_UP_ICON_MILLIS)
        {
            g_shownLvlUpIcon = false;
            g_gameState = GAME_IN_GAME;
            
            g_map.refreshAnimationValues();               
            g_map.generateMap(g_menu.getLevel());

            g_menu.resetRunSpecificVariables();
            g_menu.goToNextLevel();
            g_menu.setInAnimationVar(false);
            
            g_player1.resetValues();
            g_player1.goToDefaultPosition();
            g_player1.setLives(PLAYER_DEFAULT_LIVES - g_menu.getDifficulty());
            // debug
            // g_map.printEmptyMatrix();
            initEnemies();
        }
    }
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
    doEnemyRoutine();

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
        if(millis() - g_timeForBulletUpdate > WAIT_TIME_AFTER_LEVEL_FINISH)
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
        if(millis() - g_timeForBulletUpdate > WAIT_TIME_AFTER_LEVEL_FINISH)
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
    playMelody();

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
    // Serial.begin(115200);
    initAllHw();
    initEnemySpawns();

    g_gameState = GAME_IN_MENU;
    g_map.initMatrix();
}


// unsigned long time = 0;
void loop() 
{
    //adjustBrightness();
    // Serial.println(millis() - time);
    // time = millis();

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
        
        case GAME_BETWEEN_LEVELS:
            goToNextLevelSequence();
            break;

        default:
            break;
    }
} 