#ifndef MATRIX_ENTITY_H
#define MATRIX_ENTITY_H

#include "utility.h"
#include "gameMap.h"

extern gameMap g_map;

// base class for player and bullet and maybe enemy providing some basic functionality and useful members
class matrixEntity
{
protected: 
    int m_xPos = 0;
    int m_yPos = 0;

    int m_xNextPos = 0;
    int m_yNextPos = 0;

    byte m_direction;
public:
    // pure virtual class that should be implemented by all children
    virtual bool updatePosition() = 0;

    // moves the entity in a direction if the entity is controlled by a joystick
    void initialPositionUpdate(int &p_xPos, int &p_yPos, int p_xCommand, int p_yCommand);

    // if the entity if out of bounds its position is updated with the location opposite to it on the map (x == matrixSIze => x = 0)
    void dealWithOutOfBounds(int &p_xPos, int &p_yPos);

    // if both x and y axes are changed by the joystick this function only makes the entity choose one axis
    void pickAnAxes(int &p_xNextPos, int &p_yNextPos, int p_xPos, int p_yPos, int p_xCommand, int p_yCommand);
};

#endif