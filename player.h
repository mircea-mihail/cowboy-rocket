#ifndef PLAYER_H
#define PLAYER_H

#include "matrixEntity.h"
#include "gameMap.h"
#include "inputHwControl.h"

extern gameMap g_map;
extern bool g_disableSound;

// player class to control a player
class player : public matrixEntity
{
    inputHwControl m_hwCtrl;
    byte m_direction;
    unsigned long m_lastShot = 0;
    bool m_foundExplodingBullets = false;

    byte m_lives = 3;

private:
    unsigned long m_lastMoved = 0; // Tracks the last time the LED moved

    // stores the next movement of the player in the next move members
    void storeMovement(int p_xCommand, int p_yCommand);
    
    // sets the direction the player is facing based on their last move
    void setDirection(int p_xNextPos, int p_yNextPos, int p_xPos, int p_yPos);

    // checks if the player shot a shot pressing the shoot button
    bool pulledTrigger();

    // checks where to place the bullet by looking at the direction the player faces
    void getBulletPlacement(int &p_xPos, int &p_yPos);

    // plays a high pitched sound when picking up a power up
    void playPowerUpPickupSound();

    // plays a low note when taking damage
    void playTakeDamageSound();

public:
    // initialises the player's position, direction and places them on the map
    player(int p_xPos, int p_yPos);
    
    // updates the player's position based on the joystick movement
    bool updatePosition() override;

    // returns true if the shooting cooldown has expired and the player pressed the shoot button. Also gives through its parrameters 
    // sufficient information for the creation of a new bullet by the bulletList class
    bool shoot(int &playerXPos, int &playerYPos, byte &facingDirection, bool &isExplodingType);

    // moves the player on their default position
    void goToDefaultPosition();

    // puts the current player coordonates in p_xPosPlayer and p_yPosPlayer
    void getCoordonates(int &p_xPosPlayer, int &p_yPosPlayer);

    // removes power ups and gives back lives to player
    void resetValues();

    // returns the number of lives the player has lefts
    byte getLives();

    // decreases the player's life
    void takeDamage();

    // sets player's lives depending on the difficulty
    void setLives(byte p_difficulty);
};

#endif