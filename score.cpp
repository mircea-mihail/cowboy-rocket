#include "score.h"

void score::updateMemoryScores(long scores[NUMBER_OF_SCORES_KEPT])
{
    byte scoreIdx = FIRST_SCORE_IDX;
    for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(long))
    {
        EEPROM.put(addr, scores[scoreIdx ++]);
    }
}

void score::writeScoreToMemory(long p_scoreToWrite)
{
    long scores[NUMBER_OF_SCORES_KEPT];
    byte scoreIdx = FIRST_SCORE_IDX;
    Serial.println("current winner scores");
    for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(long))
    {
        EEPROM.get(addr, scores[scoreIdx ++]);
        Serial.println(scores[scoreIdx - 1]);
    }
    Serial.println(p_scoreToWrite);
    if(p_scoreToWrite > scores[FIRST_SCORE_IDX])
    {
        scores[THIRD_SCORE_IDX] = scores[SECOND_SCORE_IDX];
        scores[SECOND_SCORE_IDX] = scores[FIRST_SCORE_IDX];
        scores[FIRST_SCORE_IDX] = p_scoreToWrite;
    }
    else if(p_scoreToWrite > scores[SECOND_SCORE_IDX])
    {
        scores[THIRD_SCORE_IDX] = scores[SECOND_SCORE_IDX];
        scores[SECOND_SCORE_IDX] = p_scoreToWrite;
    }
    else if(p_scoreToWrite > scores[THIRD_SCORE_IDX])
    {
        scores[THIRD_SCORE_IDX] = p_scoreToWrite;
    }

    updateMemoryScores(scores);
}

void score::startCounting()
{
    m_score = 0;
}

long score::stopCounting()
{
    writeScoreToMemory(m_score);        

    return m_score;
}

void score::printHighScores()
{
    int playerRank = PLAYER_FIRST_RANK;
    for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(long))
    {
        long score;
        EEPROM.get(addr, score);
        Serial.print(F("no. "));
        Serial.print(playerRank);
        Serial.print(F(": "));
        Serial.println(score);
        playerRank ++;
    }
}

void score::updateScore(int p_wallsDestroyed, int p_enemiesKilled = 0)
{   
    m_score += p_wallsDestroyed;
    Serial.print("score: ");
    Serial.println(m_score);
}

void score::periodicScoreDecrease()
{
    if(millis() - m_lastScoreDecrease > DECREASE_TIME_MILLIS)
    {
        m_lastScoreDecrease = millis();
        m_score --;
    }
}
