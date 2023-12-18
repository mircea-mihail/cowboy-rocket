#ifndef ENEMY_H
#define ENEMY_H

#include "matrixEntity.h"
#include "player.h"

// move patterns
#define ENEMY_MOVE_INTERVAL 300
#define HARD_ENEMY_MOVE_INTERVAL 600

// enemy points
#define EASY_ENEMY_POINTS 10
#define HARD_ENEMY_POINTS 20

// enemy types
#define EASY_180_TYPE 0
#define EASY_90_TYPE 1
#define EASY_RANDOM_TYPE 2
#define HARD_TYPE 3

#define NUMBER_OF_EASY_TYPES 3

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

// enemies on levels
#define EASY_ENEMIES_ON_FIRST_LVL 2
#define EASY_ENEMIES_ON_SECOND_LVL 4 
#define EASY_ENEMIES_ON_THIRD_LVL 4
#define EASY_ENEMIES_ON_FOURTH_LVL 6
#define EASY_ENEMIES_ON_FIFTH_LVL 6

#define HARD_ENEMIES_ON_FIRST_LVL 0
#define HARD_ENEMIES_ON_SECOND_LVL 0 
#define HARD_ENEMIES_ON_THIRD_LVL 1
#define HARD_ENEMIES_ON_FOURTH_LVL 2
#define HARD_ENEMIES_ON_FIFTH_LVL 4

extern player g_player1;

struct enemySpawnCoordonates
{
    int m_xPos;
    int m_yPos;
};

class enemy: public matrixEntity
{
private:
    byte m_lives = 1;
    byte m_type = EASY_180_TYPE;
    unsigned long m_lastMoveTime = 0;

    // checks if the next position is valid and if not modifies the enemy direction
    bool isValidMove(int p_xPos, int p_yPos);

    // for hard enemies. When hitting a wall try to go around instead of going after the player
    bool tryToGoBack(int &p_xNextPos, int &p_yNextPos);

public:
    enemy(int p_xPos, int p_yPos, byte p_direction, byte p_type = EASY_180_TYPE);

    // updates the enemy position in the direction he is going
    bool updatePosition() override;   

    // decreases enemy lives to 0 if they are not 0 already
    void damageEnemy();

    // returns the enemy life left
    byte getLives();

    // returns the enemy type
    byte getType();
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