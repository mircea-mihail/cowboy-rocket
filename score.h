#ifndef SCORE_H
#define SCORE_H

#include <EEPROM.h>
#include <Arduino.h>
#include "utility.h"
#include "memory.h"


// scoring related
#define WORST_SCORE 0xFFFFFFFF

#define FIRST_SCORE_IDX 0
#define SECOND_SCORE_IDX 1
#define THIRD_SCORE_IDX 2

#define PLAYER_FIRST_RANK 1

#define DECREASE_TIME_MILLIS 3000

// i'm keeping the score by timing the time it takes the player to finish a level
class score
{
private:
    unsigned long m_lastScoreDecrease = 0;
    long m_score = 0;
    char m_winnerName[LETTERS_IN_NAME] = {'A', 'A', 'A', 'A'};

    // writes new scores to memory along with the winners
    void updateMemoryScores(long p_scores[NUMBER_OF_SCORES_KEPT], char p_scoresNames[NUMBER_OF_SCORES_KEPT][LETTERS_IN_NAME]);

    // starts the memory writing process by reading the current scores and including the new score if necessary
    void writeScoreToMemory(long p_scoreToWrite, char p_nameToWrite[LETTERS_IN_NAME]);

public:
    // starts the score timer, call this when the level begins
    void startCounting(char p_nameOfPlayer[LETTERS_IN_NAME]);

    // stops the score timer, call this when the level ends. It also writes to memory the new score if the case by calling the private functions
    long stopCounting();

    // prints the high scores recorded in memory
    void printHighScores();

    // updates the current score 
    void updateScore(int p_wallsDestroyed, int p_enemiesKilled = 0);

    // periodic decrease of score depending on the time it took the player
    void periodicScoreDecrease();
};

/*
1 wall = 1p
1 weak enemy = 10p
1 hard enemy = 20p

avg experience = 250p 
1 min -> -20 p?  
*/

#endif