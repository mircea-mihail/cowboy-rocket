#ifndef ENEMY_H
#define ENEMY_H

#include "matrixEntity.h"

#define ENEMY_MOVE_INTERVAL 200

class enemy: public matrixEntity
{
private:
    byte m_lives = 1;

    unsigned long m_lastMoveTime = 0;

    // checks if the next position is valid and if not modifies the enemy direction
    bool isValidMove(int p_xPos, int p_yPos);

public:
    enemy(int p_xPos, int p_yPos, byte p_direction);

    // updates the enemy position in the direction he is going
    bool updatePosition() override;   

    void damageEnemy();

    byte getLives();

    void getCoordonates(int &p_xPos, int &p_yPos);
};

#endif