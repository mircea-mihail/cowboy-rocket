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

// i'm keeping the score by timing the time it takes the player to finish a level
class score
{
private:
    unsigned long m_timerBeginning = DEFAULT_TIME_VAL;
    unsigned long m_score = 0;

    // writes new scores to memory
    void updateMemoryScores(unsigned long scores[NUMBER_OF_SCORES_KEPT]);

    // starts the memory writing process by reading the current scores and including the new score if necessary
    void writeScoreToMemory(unsigned long p_scoreToWrite);

public:
    // starts the score timer, call this when the level begins
    void startCounting();

    // stops the score timer, call this when the level ends. It also writes to memory the new score if the case by calling the private functions
    unsigned long stopCounting();

    // writes 0xFFFFFFFF to all score memory places (the largest unsigned long value and the worst score)
    void clearScores();

    // prints the high scores recorded in memory
    void printHighScores();
};

#endif