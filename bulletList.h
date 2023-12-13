#ifndef BULLET_LIST_H
#define BULLET_LIST_H

#include "bullet.h"

// node in the bullet holding data structure
struct bulletNode
{
    bullet *m_bullet;
    bulletNode *m_nextBulletNode;
    bulletNode *m_prevBulletNode;

    // creates a bullet node initialising its members
    bulletNode(bullet *p_newBullet, bulletNode *p_nextBulletNode, bulletNode *p_prevBulletNode);

    // deletes the bullet object 
    ~bulletNode();
};

// double linked list for easy construction and deletion of bullets
class bulletList
{
    bulletNode *m_firstBulletNode;
    bulletNode *m_lastBulletNode;

    unsigned long m_lastBulletUpdate = 0;

public:
    // deletes bullet node 
    void deleteBulletNode(bulletNode *p_bulletNode);  

    // creates new bullet node and new bullet 
    void addBulletNode(bullet *p_newBullet);

    // every BULLET_TICK_RATE millis updates each bullet in the data structure
    void updateBullets();

    // sets the last bullet uupdate time to the current time for better sync
    // of the new bullets to the tick rate ( better to have slower bullets than faster ones)
    void setLastBulletUpdate();
};


#endif