#include "enemy.h"


enemy::enemy(int p_xPos, int p_yPos, byte p_direction)
{
    m_xPos = p_xPos;
    m_yPos = p_yPos;
    m_direction = p_direction;
}

bool enemy::isValidMove(int p_xPos, int p_yPos)
{
    if(g_map.isMapElement(MAP_WALL, p_xPos, p_yPos))
    {
        reverseDirection(m_direction);
        return false;
    }
    return true;
}

bool enemy::updatePosition() 
{
    if(millis() - m_lastMoveTime < ENEMY_MOVE_INTERVAL)
    {
        return false;
    }
    
    m_lastMoveTime = millis();

    m_xNextPos = m_xPos;
    m_yNextPos = m_yPos;

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

    dealWithOutOfBounds(m_xNextPos, m_yNextPos);
    
    if(g_map.isWithinBounds(m_xNextPos, m_yNextPos))
    {
        if(!isValidMove(m_xNextPos, m_yNextPos))
        {
            return false;
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