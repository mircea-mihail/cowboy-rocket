#include "gameMap.h"

void gameMap::displayElement(int realRow, int realCol, int p_row, int p_col)
{
    switch(matrix[p_row][p_col])
    {
        case(MAP_PLAYER):
            m_ledControl.setLed(MATRIX_ADDRESS, realRow, realCol, m_blinkPlayer);

            if(millis() - m_lastPlayerBlink > PLAYER_BLINK_INTERVAL)
            {
                m_lastPlayerBlink = millis();
                m_blinkPlayer = !m_blinkPlayer;
            }
            break;

        case(MAP_WALL):
            m_ledControl.setLed(MATRIX_ADDRESS, realRow, realCol, HIGH);
            break;

        case(MAP_EMPTY):
            m_ledControl.setLed(MATRIX_ADDRESS, realRow, realCol, LOW);
            break;

        case MAP_BULLET:
            m_ledControl.setLed(MATRIX_ADDRESS, realRow, realCol, m_blinkBullet);

            if(millis() - m_lastBulletBlink > BULLET_BLINK_INTERVAL)
            {
                m_lastBulletBlink = millis();
                m_blinkBullet = !m_blinkBullet;
            }
            break;

        case MAP_POWER_UP:
            m_ledControl.setLed(MATRIX_ADDRESS, realRow, realCol, m_blinkPowerUp);

            if(millis() - m_lastPowerUpBlink > POWER_UP_BLINK_INTERVAL)
            {
                m_lastPowerUpBlink = millis();
                m_blinkPowerUp = !m_blinkPowerUp;
            }
            break;

        case MAP_ENEMY:
            m_ledControl.setLed(MATRIX_ADDRESS, realRow, realCol, m_blinkEnemy);

            if(millis() - m_lastEnemyBlink > ENEMY_BLINK_INTERVAL)
            {
                m_lastEnemyBlink = millis();
                m_blinkEnemy = !m_blinkEnemy;
            }

            break;

        default:
            break;
    }
}

void gameMap::initMatrix()
{
    m_matrixBrightness = EEPROM.read(EEPROM_MATRIX_BRIGHTNESS_ADDRESS);

    m_ledControl.shutdown(MATRIX_ADDRESS, false); // turn off power saving, enables display
    m_ledControl.setIntensity(MATRIX_ADDRESS, m_matrixBrightness); // sets brightness (0~15 possible values)
    m_ledControl.clearDisplay(MATRIX_ADDRESS); // Clear the matrix display
}

void gameMap::setPositionValue(int p_xPos, int p_yPos, int p_newValue)
{
    matrix[p_xPos][p_yPos] = p_newValue;
}

void gameMap::updateDisplay(int p_xPosPlayer, int p_yPosPlayer) 
{
    int minCol = p_yPosPlayer - REAL_MATRIX_MIDDLE;
    int minRow = p_xPosPlayer - REAL_MATRIX_MIDDLE;
    
    int maxCol = minCol + REAL_MATRIX_SIZE;
    int maxRow = minRow + REAL_MATRIX_SIZE;

    if(minCol < 0)
    {
        minCol = 0;
        maxCol = REAL_MATRIX_SIZE;
    }
    if(minRow < 0)
    {
        minRow = 0;
        maxRow = REAL_MATRIX_SIZE;
    }
    if(maxCol > MATRIX_SIZE)
    {
        maxCol = MATRIX_SIZE;
        minCol = MATRIX_SIZE - REAL_MATRIX_SIZE;
    }
    if(maxRow > MATRIX_SIZE)
    {
        maxRow = MATRIX_SIZE;
        minRow = MATRIX_SIZE - REAL_MATRIX_SIZE;
    }

    for(int row = minRow; row < maxRow; row++) 
    {
        for(int col = minCol; col < maxCol; col++) 
        {
            int realRow = row - minRow;
            int realCol = col - minCol;

            displayElement(realRow, realCol, row, col);
        }
    }
}

bool gameMap::isWithinBounds(int p_xPos, int p_yPos)
{
    if(p_xPos >= 0 && p_xPos < MATRIX_SIZE && p_yPos >= 0 && p_yPos < MATRIX_SIZE)
    {
        return true;
    }
    return false;
}

bool gameMap::isMapElement(const byte p_mapElement, int p_xPos, int p_yPos)
{
    if(matrix[p_xPos][p_yPos] == p_mapElement)
    {
        return true;
    }
    return false;
}

void gameMap::generateMap(byte p_currentLevel)
{
    randomSeed(analogRead(BUTTON_PIN));
    // randomly generate walls
    for(int row = 0; row < MATRIX_SIZE; row++)
    {
        for(int col = 0; col < MATRIX_SIZE; col++)
        {
            long chance = random(0, GRANULARITY_OF_CHANCE);
            // the bigger the level the higher the chance
            if(chance < CHANCE_OF_WALL - NUMBER_OF_LEVELS + p_currentLevel)
            {
                matrix[row][col] = MAP_WALL; 
            }
            else
            {
                matrix[row][col] = MAP_EMPTY;
            }
        }       
    }

    // clear area around player
    for(int row = MATRIX_MIDDLE - 1; row <= MATRIX_MIDDLE + 1; row++)
    {
        for(int col = MATRIX_MIDDLE - 1; col <= MATRIX_MIDDLE + 1; col++)
        {
            matrix[row][col] = MAP_EMPTY; 
        }
    }

    // plant power up
    int powerUpXPos = random(0, MATRIX_SIZE);
    int powerUpYPos = random(0, MATRIX_SIZE);

    if(powerUpXPos == MATRIX_MIDDLE)
    {
        powerUpXPos ++;
    }

    matrix[powerUpXPos][powerUpYPos] = MAP_POWER_UP;
}

bool gameMap::checkWinningCondition()
{
    for(int row = 0; row < MATRIX_SIZE; row++)
    {
        for(int col = 0; col < MATRIX_SIZE; col++)
        {
            if( matrix[row][col] != MAP_BULLET && matrix[row][col] != MAP_PLAYER 
                && matrix[row][col] != MAP_EMPTY && matrix[row][col] != MAP_ENEMY)
            {
                return false;
            }
        }       
    }
    return true;
}

int gameMap::getWallsLeft()
{
    int wallsLeft = 0;
    for(int row = 0; row < MATRIX_SIZE; row++)
    {
        for(int col = 0; col < MATRIX_SIZE; col++)
        {
            if(matrix[row][col] == MAP_WALL)
            {
                wallsLeft ++;
            }
        }       
    }
    return wallsLeft;   
}

void gameMap::printMatrixFrame(const byte p_matrixVector[][REAL_MATRIX_SIZE],  byte p_frameToPrint)
{
    for(int row = 0; row < REAL_MATRIX_SIZE; row++)
    {
        for(int col = 0; col < REAL_MATRIX_SIZE; col++)
        {   
            m_ledControl.setLed(MATRIX_ADDRESS, row, col, getColValFromByte(p_matrixVector[p_frameToPrint][row], col));
        }
    }
}

bool gameMap::printWinningMatrixAnimation()
{
    if(millis() - m_lastFrameTime > WINNING_FRAME_DISPLAY_TIME)
    {
        m_lastFrameTime = millis();
        printMatrixFrame(m_winningMatrixVector, m_currentFrameIndex);

        m_currentFrameIndex += 1;

        if(m_currentFrameIndex == WINNING_FRAME_NUMBER)
        {
            m_currentFrameIndex = 0;
            return true;  
        }
    }
    return false;
}

bool gameMap::printStartGameMatrixAnimation()
{
    if(millis() - m_lastFrameTime > GAME_START_FRAME_DISPLAY_TIME)
    {
        m_lastFrameTime = millis();
        printMatrixFrame(m_gameStartMatrixVector, m_currentFrameIndex);

        m_currentFrameIndex += 1;

        if(m_currentFrameIndex == GAME_START_FRAME_NUMBER)
        {
            refreshAnimationValues();
            return true;  
        }
    }
    return false;
}

void gameMap::refreshAnimationValues()
{
    m_currentFrameIndex = DEFAULT_FRAME_INDEX_VALUE;
}

void gameMap::setMatrixBrightness(byte p_newBrightness)
{
    m_matrixBrightness = p_newBrightness;
    m_ledControl.setIntensity(MATRIX_ADDRESS, m_matrixBrightness);
}

byte gameMap::getMatrixBrightness()
{
    return m_matrixBrightness;
}

byte gameMap::incrementMatrixBrightness()
{
    m_matrixBrightness ++;
    if(m_matrixBrightness > MAX_MATRIX_BRIGHTNESS)
    {
        m_matrixBrightness = MIN_MATRIX_BRIGHTNESS;
    }

    m_ledControl.setIntensity(MATRIX_ADDRESS, m_matrixBrightness);
    return m_matrixBrightness;
}

byte gameMap::decrementMatrixBrightness()
{
    if(m_matrixBrightness == MIN_MATRIX_BRIGHTNESS)
    {
        m_matrixBrightness = MAX_MATRIX_BRIGHTNESS;
    }
    else
    {
        m_matrixBrightness --;  
    }
    m_ledControl.setIntensity(MATRIX_ADDRESS, m_matrixBrightness);
    return m_matrixBrightness;
}

void gameMap::displayIcon(const byte p_iconIndex)
{
    for(int row = 0; row < REAL_MATRIX_SIZE; row++)
    {
        for(int col = 0; col < REAL_MATRIX_SIZE; col++)
        {   
            m_ledControl.setLed(MATRIX_ADDRESS, row, col, getColValFromByte(m_gameIcons[p_iconIndex][row], col));
        }
    }   
}

// for debug
void gameMap::printEmptyMatrix()
{
    for(int row = 0; row < MATRIX_SIZE; row++)
    {
        for(int col = 0; col < MATRIX_SIZE; col++)
        {   
            matrix[row][col] = MAP_EMPTY;
        }
    }
    
    matrix[MATRIX_MIDDLE - 1][MATRIX_MIDDLE] = MAP_POWER_UP;

    matrix[MATRIX_MIDDLE - 2][MATRIX_MIDDLE] = MAP_WALL;
    matrix[MATRIX_MIDDLE - 2][MATRIX_MIDDLE - 1] = MAP_WALL;
    matrix[MATRIX_MIDDLE - 2][MATRIX_MIDDLE + 1] = MAP_WALL;
    matrix[MATRIX_MIDDLE - 2][MATRIX_MIDDLE - 2] = MAP_WALL;

    matrix[MATRIX_MIDDLE + 3][MATRIX_MIDDLE] = MAP_WALL;
    matrix[MATRIX_MIDDLE + 3][MATRIX_MIDDLE - 1] = MAP_WALL;
    matrix[MATRIX_MIDDLE + 3][MATRIX_MIDDLE + 1] = MAP_WALL;
    matrix[MATRIX_MIDDLE + 3][MATRIX_MIDDLE - 2] = MAP_WALL;
}