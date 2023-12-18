#include "score.h"

void score::updateMemoryScores(long p_scores[NUMBER_OF_SCORES_KEPT], char p_scoresNames[NUMBER_OF_SCORES_KEPT][LETTERS_IN_NAME])
{
    byte scoreIdx = FIRST_SCORE_IDX;
    for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(long))
    {
        EEPROM.put(addr, p_scores[scoreIdx ++]);
    }

    scoreIdx = FIRST_SCORE_IDX;
    for(int nameIdx = 0; nameIdx < NUMBER_OF_SCORES_KEPT; nameIdx ++)
    {
        for(int charIdx = 0; charIdx < LETTERS_IN_NAME; charIdx ++)
        {
            const int memoryIdx = SCORE_NAMES_ADDRESS + nameIdx * SIZE_OF_NAME_IN_EEPROM + charIdx;
            EEPROM.update(memoryIdx, p_scoresNames[nameIdx][charIdx]);
        }
    }
    
}

void score::writeScoreToMemory(long p_scoreToWrite, char p_nameToWrite[LETTERS_IN_NAME])
{
    long scores[NUMBER_OF_SCORES_KEPT];
    char scoresNames[NUMBER_OF_SCORES_KEPT][LETTERS_IN_NAME];

    byte scoreIdx = FIRST_SCORE_IDX;
    for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(long))
    {
        EEPROM.get(addr, scores[scoreIdx ++]);
    }
    for(int nameIdx = 0; nameIdx < NUMBER_OF_SCORES_KEPT; nameIdx ++)
    {
        for(int charIdx = 0; charIdx < LETTERS_IN_NAME; charIdx ++)
        {
            const int memoryIdx = SCORE_NAMES_ADDRESS + nameIdx * SIZE_OF_NAME_IN_EEPROM + charIdx;
            scoresNames[nameIdx][charIdx] = EEPROM.read(memoryIdx);
        }
    }
    
    m_scoreRank = PLAYER_NO_RANK; 
    
    if(p_scoreToWrite > scores[FIRST_SCORE_IDX])
    {
        m_scoreRank = PLAYER_FIRST_RANK; 

        scores[THIRD_SCORE_IDX] = scores[SECOND_SCORE_IDX];
        memcpy(scoresNames[THIRD_SCORE_IDX], scoresNames[SECOND_SCORE_IDX], LETTERS_IN_NAME);

        scores[SECOND_SCORE_IDX] = scores[FIRST_SCORE_IDX];
        memcpy(scoresNames[SECOND_SCORE_IDX], scoresNames[FIRST_SCORE_IDX], LETTERS_IN_NAME);

        scores[FIRST_SCORE_IDX] = p_scoreToWrite;
        memcpy(scoresNames[FIRST_SCORE_IDX], p_nameToWrite, LETTERS_IN_NAME);
    }
    else if(p_scoreToWrite > scores[SECOND_SCORE_IDX])
    {
        m_scoreRank = PLAYER_SECOND_RANK; 

        scores[THIRD_SCORE_IDX] = scores[SECOND_SCORE_IDX];
        memcpy(scoresNames[THIRD_SCORE_IDX], scoresNames[SECOND_SCORE_IDX], LETTERS_IN_NAME);
     
        scores[SECOND_SCORE_IDX] = p_scoreToWrite;
        memcpy(scoresNames[SECOND_SCORE_IDX], p_nameToWrite, LETTERS_IN_NAME);
    }
    else if(p_scoreToWrite > scores[THIRD_SCORE_IDX])
    {
        m_scoreRank = PLAYER_THIRD_RANK; 

        scores[THIRD_SCORE_IDX] = p_scoreToWrite;
        memcpy(scoresNames[THIRD_SCORE_IDX], p_nameToWrite, LETTERS_IN_NAME);
    }

    updateMemoryScores(scores, scoresNames);
}

void score::startCounting(char p_nameOfPlayer[LETTERS_IN_NAME])
{
    memcpy(m_winnerName, p_nameOfPlayer, LETTERS_IN_NAME);
    m_score = 0;
}

long score::stopCounting()
{
    writeScoreToMemory(m_score, m_winnerName);        

    return m_score;
}

void score::updateScore(int p_wallsDestroyed, byte p_currentDifficulty, int p_enemiesKilled = 0, byte p_enemyType = EASY_180_TYPE)
{   
    m_score += p_wallsDestroyed * p_currentDifficulty;
    m_score += (p_enemiesKilled * ((p_enemyType != HARD_TYPE) ? EASY_ENEMY_POINTS : HARD_ENEMY_POINTS)); 
    
    if(g_player1.foundExplodingBullets())
    {
        m_score ++;
    }
}

void score::periodicScoreDecrease()
{
    if(millis() - m_lastScoreDecrease > DECREASE_TIME_MILLIS)
    {
        m_lastScoreDecrease = millis();
        m_score --;
    }
}

bool score::changedScore()
{
    if(m_score != m_lastScoreChecked)
    {
        m_lastScoreChecked = m_score;
        return true;
    }
    return false;
}

long score::getScore()
{
    return m_score;
}

byte score::isHighScore()
{
    return m_scoreRank;
}
