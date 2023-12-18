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

    // if the entity if out of bounds its position is updated with the location opposite to it on the map (x == matrixSIze => x = 0)
    void dealWithOutOfBounds(int &p_xPos, int &p_yPos);

    // get x and y coordonates of the entity
    void getCoordonates(int &p_xPos, int &p_yPos);
};

#endif