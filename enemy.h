#ifndef ENEMY_H
#define ENEMY_H

#include "matrixEntity.h"

#define ENEMY_MOVE_INTERVAL 200

class enemy: public matrixEntity
{
private:
    unsigned long m_lastMoveTime = 0;

    bool isValidMove(int p_xPos, int p_yPos);

public:
    enemy(int p_xPos, int p_yPos, byte p_direction);

    bool updatePosition() override;    
};

#endif