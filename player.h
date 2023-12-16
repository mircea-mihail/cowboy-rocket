#ifndef PLAYER_H
#define PLAYER_H

#include "matrixEntity.h"
#include "bulletList.h"
#include "gameMap.h"
#include "inputHwControl.h"

extern bulletList g_bulletList;
extern gameMap g_map;

// player class to control a player
class player : public matrixEntity
{
    inputHwControl m_hwCtrl;
    byte m_direction;
    unsigned long m_lastShot = 0;
    bool m_foundExplodingBullets = false;
    byte m_lives = 3;

private:
    unsigned long long m_lastMoved = 0; // Tracks the last time the LED moved

    // stores the next movement of the player in the next move members
    void storeMovement(int p_xCommand, int p_yCommand);
    
    // sets the direction the player is facing based on their last move
    void setDirection(int p_xNextPos, int p_yNextPos, int p_xPos, int p_yPos);

    // checks if the player shot a shot pressing the shoot button
    bool pulledTrigger();

    // checks where to place the bullet by looking at the direction the player faces
    void getBulletPlacement(int &p_xPos, int &p_yPos);

public:
    // initialises the player's position, direction and places them on the map
    player(int p_xPos, int p_yPos);
    
    // updates the player's position based on the joystick movement
    bool updatePosition() override;

    // tries to shoot if the shooting cooldown has expired and the player pressed the shoot button
    void shoot();

    // moves the player on their default position
    void goToDefaultPosition();

    // puts the current player coordonates in p_xPosPlayer and p_yPosPlayer
    void getCoordonates(int &p_xPosPlayer, int &p_yPosPlayer);

    // removes power ups from player
    void resetPowerUps();

    // returns the number of lives the player has lefts
    byte getLives();
};

#endif