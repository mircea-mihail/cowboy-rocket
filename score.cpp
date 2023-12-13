#include "score.h"

void score::updateMemoryScores(unsigned long scores[NUMBER_OF_SCORES_KEPT])
{
    byte scoreIdx = FIRST_SCORE_IDX;
    for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(unsigned long))
    {
        EEPROM.put(addr, scores[scoreIdx ++]);
    }
}

void score::writeScoreToMemory(unsigned long p_scoreToWrite)
{
    unsigned long scores[NUMBER_OF_SCORES_KEPT];
    byte scoreIdx = FIRST_SCORE_IDX;
    for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(unsigned long))
    {
        EEPROM.get(addr, scores[scoreIdx ++]);
    }

    if(p_scoreToWrite < scores[FIRST_SCORE_IDX])
    {
        scores[THIRD_SCORE_IDX] = scores[SECOND_SCORE_IDX];
        scores[SECOND_SCORE_IDX] = scores[FIRST_SCORE_IDX];
        scores[FIRST_SCORE_IDX] = p_scoreToWrite;
    }
    else if(p_scoreToWrite < scores[SECOND_SCORE_IDX])
    {
        scores[THIRD_SCORE_IDX] = scores[SECOND_SCORE_IDX];
        scores[SECOND_SCORE_IDX] = p_scoreToWrite;
    }
    else if(p_scoreToWrite < scores[THIRD_SCORE_IDX])
    {
        scores[THIRD_SCORE_IDX] = p_scoreToWrite;
    }

    updateMemoryScores(scores);
}

void score::startCounting()
{
    m_timerBeginning = millis();
}

unsigned long score::stopCounting()
{
    m_score = millis() - m_timerBeginning;

    writeScoreToMemory(m_score);        

    return m_score;
}

void score::clearScores()
{
    for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(unsigned long))
    {
        EEPROM.put(addr, WORST_SCORE);
    }
}

void score::printHighScores()
{
    int playerRank = PLAYER_FIRST_RANK;
    for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(unsigned long))
    {
        unsigned long score;
        EEPROM.get(addr, score);
        Serial.print(F("no. "));
        Serial.print(playerRank);
        Serial.print(F(": "));
        Serial.println(score);
        playerRank ++;
    }
}