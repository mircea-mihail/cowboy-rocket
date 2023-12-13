#include "player.h"

//////////////// private
void player::storeMovement(int p_xCommand, int p_yCommand)
{
    m_xNextPos = m_xPos;
    m_yNextPos = m_yPos;

    initialPositionUpdate(m_xNextPos, m_yNextPos, p_xCommand, p_yCommand);
    pickAnAxes(m_xNextPos, m_yNextPos, m_xPos, m_yPos, p_xCommand, p_yCommand);
}

void player::setDirection(int p_xNextPos, int p_yNextPos, int p_xPos, int p_yPos)
{
    if(p_xNextPos != p_xPos && p_yNextPos == p_yPos)
    {
        m_direction = ((p_xNextPos - p_xPos > 0) ? DIRECTION_DOWN : DIRECTION_UP);
        if(absolute(p_xNextPos - p_xPos) > 1)
        {
            reverseDirection(m_direction);
        }    
    }
    
    if(p_xNextPos == p_xPos && p_yNextPos != p_yPos)
    {
        m_direction = ((p_yNextPos - p_yPos > 0) ? DIRECTION_RIGHT : DIRECTION_LEFT);
        
        if(absolute(p_yNextPos - p_yPos) > 1)
        {
            reverseDirection(m_direction);
        }    
    }
}

bool player::pulledTrigger()
{
    return m_hwCtrl.pressedButton();
}

void player::getBulletPlacement(int &p_xPos, int &p_yPos)
{
    switch (m_direction)
    {
        case DIRECTION_UP:
            p_xPos -= 1;
            break;

        case DIRECTION_DOWN:
            p_xPos += 1;
            break;

        case DIRECTION_LEFT:
            p_yPos -= 1;
            break;

        case DIRECTION_RIGHT:
            p_yPos += 1;
            break;

        default:
            break;
    }
}

//////////////// public

player::player(int p_xPos, int p_yPos)
{
    m_direction = DIRECTION_RIGHT;
    m_xPos = p_xPos;
    m_yPos = p_yPos;
    g_map.setPositionValue(m_xPos, m_yPos, MAP_PLAYER); 
}

bool player::updatePosition()  
{
    g_map.setPositionValue(m_xPos, m_yPos, MAP_PLAYER);

    int xCommand, yCommand;
    if(m_hwCtrl.joystickDetected(xCommand, yCommand))
    {
        storeMovement(xCommand, yCommand);
        
        if(millis() - m_lastMoved > PLAYER_MOVEMENT_MILLIS)
        {
            dealWithOutOfBounds(m_xNextPos, m_yNextPos);
            setDirection(m_xNextPos, m_yNextPos, m_xPos, m_yPos);

            bool moved = false;
            if(!g_map.isMapElement(MAP_WALL, m_xNextPos, m_yNextPos) && !g_map.isMapElement(MAP_BULLET, m_xNextPos, m_yNextPos))
            {
                if(g_map.isMapElement(MAP_POWER_UP, m_xNextPos, m_yNextPos))
                {
                    m_foundExplodingBullets = true;
                }
                g_map.setPositionValue(m_xNextPos, m_yNextPos, MAP_PLAYER);

                // set old position to zero 
                if(m_xPos != m_xNextPos || m_yPos != m_yNextPos)
                {
                    g_map.setPositionValue(m_xPos, m_yPos, MAP_EMPTY); 
                    m_xPos = m_xNextPos;
                    m_yPos = m_yNextPos;
                    moved = true;
                }
            }
            m_lastMoved = millis();
            
            if(moved)
            {
                return true;
            }
        }
    }
    return false;
}

void player::shoot()
{
    if(millis() - m_lastShot > SHOOTING_COOLDOWN && pulledTrigger())
    {
        int xPos = m_xPos, yPos = m_yPos;

        getBulletPlacement(xPos, yPos);
        dealWithOutOfBounds(xPos, yPos);
        
        g_bulletList.setLastBulletUpdate();
        g_bulletList.addBulletNode(new bullet(xPos, yPos, m_direction, m_foundExplodingBullets));

        m_lastShot = millis();        
    }
}

void player::goToDefaultPosition()
{
    m_xPos = MATRIX_MIDDLE;
    m_yPos = MATRIX_MIDDLE;
}

void player::getCoordonates(int &p_xPosPlayer, int &p_yPosPlayer)
{
    p_xPosPlayer = m_xPos;
    p_yPosPlayer = m_yPos;
}

void player::resetPowerUps()
{
    m_foundExplodingBullets = false;
}
