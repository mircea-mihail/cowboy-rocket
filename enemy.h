#ifndef ENEMY_H
#define ENEMY_H

#include "matrixEntity.h"

#define ENEMY_MOVE_INTERVAL 200

// enemy types
#define EASY_TURN_TYPE 0
#define EASY_90_TYPE 1
#define HARD_TYPE 2

#define NUMBER_OF_POSSIBLE_SPAWNS 10
#define NUMBER_OF_EASY_SPAWNS 6
#define NUMBER_OF_HARD_SPAWNS 4

// enemy spawn defines

#define FIRST_EASY_ENEMY_X 3
#define FIRST_EASY_ENEMY_Y 3
#define SECOND_EASY_ENEMY_X 3
#define SECOND_EASY_ENEMY_Y 12
#define THIRD_EASY_ENEMY_X 12
#define THIRD_EASY_ENEMY_Y 3
#define FOURTH_EASY_ENEMY_X 12
#define FOURTH_EASY_ENEMY_Y 12
#define FIFTH_EASY_ENEMY_X 0
#define FIFTH_EASY_ENEMY_Y 7
#define SIXTH_EASY_ENEMY_X 9 
#define SIXTH_EASY_ENEMY_Y 15

#define FIRST_HARD_ENEMY_X 1
#define FIRST_HARD_ENEMY_Y 1
#define SECOND_HARD_ENEMY_X 1
#define SECOND_HARD_ENEMY_Y 14
#define THIRD_HARD_ENEMY_X 14
#define THIRD_HARD_ENEMY_Y 1
#define FOURTH_HARD_ENEMY_X 14
#define FOURTH_HARD_ENEMY_Y 14

struct enemySpawnCoordonates
{
    int m_xPos;
    int m_yPos;
};

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
};

// enemy spawn spots
/*
P = player
E = easy enemy -> dumb moves, only spins around or turns 90 deg
H = hard enemy -> tries to go after the player (hunts the player)

hard enemy possible spots: (x y) 
    (1 1) (1 14) (14 1) (14 14)

easy enemy possible spots: (x y) 
    (3 3) (3 12) (12 3) (12 12) (0 7) (9 15)

  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
0 _ _ _ _ . . . . _ _ _ _ . . . . 
1 . H . . _ _ _ _ . . . . _ _ H _ 
2 _ _ _ _ . . . . _ _ _ _ . . . . 
3 . . . E _ _ _ _ . . . . E _ _ _ 
4 _ _ _ _ . . . . _ _ _ _ . . . . 
5 . . . . _ _ _ _ . . . . _ _ _ _ 
6 _ _ _ _ . . . . _ _ _ _ . . . . 
7 E . . . _ _ _       . . _ _ _ _ 
8 _ _ _ _ . . .   P   _ _ . . . . 
9 . . . . _ _ _       . . _ _ _ E 
0 _ _ _ _ . . . . _ _ _ _ . . . . 
1 . . . . _ _ _ _ . . . . _ _ _ _ 
2 _ _ _ E . . . . _ _ _ _ E . . . 
3 . . . . _ _ _ _ . . . . _ _ _ _ 
4 _ H _ _ . . . . _ _ _ _ . . H . 
5 . . . . _ _ _ _ . . . . _ _ _ _ 



*/

#endif