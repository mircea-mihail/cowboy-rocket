#include "enemy.h"


enemy::enemy(int p_xPos, int p_yPos, byte p_direction, byte p_type = EASY_180_TYPE)
{
    m_xPos = p_xPos;
    m_yPos = p_yPos;
    m_direction = p_direction;
    m_type = p_type;
    g_map.setPositionValue(m_xPos, m_yPos, MAP_EMPTY); 
}

bool enemy::isValidMove(int p_xPos, int p_yPos)
{
    if(g_map.isMapElement(MAP_WALL, p_xPos, p_yPos))
    {
        switch(m_type)
        {
            case EASY_180_TYPE:
                reverseDirection(m_direction);
                break;
            
            case EASY_90_TYPE:
                turnDirection90Deg(m_direction);
                break;

            case EASY_RANDOM_TYPE:
                // random is exclusive with the last value but just to be sure 
                m_direction = random(0, MAP_NUMBER_OF_ORIENTATIONS) % 4;
                break;
            case HARD_TYPE:
                break;

            default:
                break;
        }
        return false;
    }
    return true;
}

bool enemy::tryToGoBack(int &p_xNextPos, int &p_yNextPos)
{
    if(p_xNextPos != m_xPos)
    {
        if(p_xNextPos < m_xPos)
        {
            p_xNextPos = m_xPos;
            p_yNextPos = m_yPos;
            p_xNextPos ++;

            dealWithOutOfBounds(p_xNextPos, p_yNextPos);
            if(isValidMove(p_xNextPos, p_yNextPos))
            {
                return true;
            }
        }
        else
        {
            p_xNextPos = m_xPos;
            p_yNextPos = m_yPos;
            p_xNextPos --;

            dealWithOutOfBounds(p_xNextPos, p_yNextPos);
            if(isValidMove(p_xNextPos, p_yNextPos))
            {
                return true;
            }
        }
    }
    else
    {
        if(p_yNextPos < m_yPos)
        {
            p_xNextPos = m_xPos;
            p_yNextPos = m_yPos;
            p_yNextPos ++;

            dealWithOutOfBounds(p_xNextPos, p_yNextPos);
            if(isValidMove(p_xNextPos, p_yNextPos))
            {
                return true;
            }
        }
        else
        {
            p_xNextPos = m_xPos;
            p_yNextPos = m_yPos;
            p_yNextPos --;

            dealWithOutOfBounds(p_xNextPos, p_yNextPos);
            if(isValidMove(p_xNextPos, p_yNextPos))
            {
                return true;
            }
        }
    }
    return false;
}

bool enemy::updatePosition() 
{
    if(millis() - m_lastMoveTime < ENEMY_MOVE_INTERVAL)
    {
        return false;
    }
    
    if(m_type == HARD_TYPE && millis() - m_lastMoveTime < HARD_ENEMY_MOVE_INTERVAL)
    {
        return false;
    }

    m_lastMoveTime = millis();

    m_xNextPos = m_xPos;
    m_yNextPos = m_yPos;

    if(m_type != HARD_TYPE)
    {
        switch(m_direction)
        {
            case(DIRECTION_UP):
                m_xNextPos -= 1;
                break;
            
            case(DIRECTION_DOWN):
                m_xNextPos += 1;
                break;

            case(DIRECTION_LEFT):
                m_yNextPos -= 1;
                break;

            case(DIRECTION_RIGHT):
                m_yNextPos += 1;
                break;

            default:
                return;
                break;
        }
    }
    else
    {
        int xPlayerPos, yPlayerPos;
        g_player1.getCoordonates(xPlayerPos, yPlayerPos);
        if(absolute(m_xNextPos - xPlayerPos) > absolute(m_yNextPos - yPlayerPos))
        {
            if(m_xNextPos > xPlayerPos)
            {
                m_xNextPos --;
            }
            else
            {
                m_xNextPos ++;
            }
        }
        else
        {       
            if(m_yNextPos > yPlayerPos)
            {
                m_yNextPos --;
            }
            else
            {
                m_yNextPos ++;
            }
        }   
    }

    dealWithOutOfBounds(m_xNextPos, m_yNextPos);
    
    if(g_map.isWithinBounds(m_xNextPos, m_yNextPos))
    {
        if(!isValidMove(m_xNextPos, m_yNextPos))
        {
            if(m_type == HARD_TYPE)
            {
                if(!tryToGoBack(m_xNextPos, m_yNextPos))
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        g_map.setPositionValue(m_xNextPos, m_yNextPos, MAP_ENEMY);

        if(m_xPos != m_xNextPos || m_yPos != m_yNextPos)
        {
            g_map.setPositionValue(m_xPos, m_yPos, MAP_EMPTY); 
        }

        m_xPos = m_xNextPos;
        m_yPos = m_yNextPos;
        return true;
    }
    
    return false;
}

void enemy::damageEnemy()
{
    if(m_lives > 0)
    {
        m_lives --;
    }
}

byte enemy::getLives()
{
    return m_lives;
}

byte enemy::getType()
{
    return m_type;
}
