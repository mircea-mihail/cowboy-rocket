#include "bulletList.h"

//////////////// bullet Node:

bulletNode::bulletNode(bullet *p_newBullet, bulletNode *p_nextBulletNode, bulletNode *p_prevBulletNode)
{
    m_bullet = p_newBullet;
    m_nextBulletNode = p_nextBulletNode;
    m_prevBulletNode = p_prevBulletNode;
}

bulletNode::~bulletNode()
{
    delete m_bullet;
}

//////////////// bullet List:

void bulletList::deleteBulletNode(bulletNode *p_bulletNode)
{
    // if this is the only bullet, delete it and make the first bullet null
    if(p_bulletNode->m_prevBulletNode == NULL && p_bulletNode->m_nextBulletNode == NULL)
    {
        m_firstBulletNode = NULL;
        m_lastBulletNode = NULL;
    }
    else if(p_bulletNode->m_prevBulletNode == NULL)
    {
        m_firstBulletNode = p_bulletNode->m_nextBulletNode;
        p_bulletNode->m_nextBulletNode->m_prevBulletNode = NULL;
    }
    else if(p_bulletNode->m_nextBulletNode == NULL)
    {
        m_lastBulletNode = p_bulletNode->m_prevBulletNode;
        p_bulletNode->m_prevBulletNode->m_nextBulletNode = NULL;
    }
    else
    {
        p_bulletNode->m_prevBulletNode->m_nextBulletNode = p_bulletNode->m_nextBulletNode;
        p_bulletNode->m_nextBulletNode->m_prevBulletNode = p_bulletNode->m_prevBulletNode;
    }

    delete p_bulletNode;
}    

void bulletList::addBulletNode(bullet *p_newBullet)
{
    if(m_lastBulletNode == NULL)
    {
        m_lastBulletNode = new bulletNode(p_newBullet, NULL, NULL);
        m_firstBulletNode = m_lastBulletNode;
    }
    else
    {
        m_lastBulletNode->m_nextBulletNode = new bulletNode(p_newBullet, NULL, m_lastBulletNode);
        m_lastBulletNode = m_lastBulletNode->m_nextBulletNode;
    }
}

void bulletList::updateBullets()
{
    if(millis() - m_lastBulletUpdate > BULLET_TICK_RATE)
    {
        bulletNode *currentNode = m_firstBulletNode;
        while(currentNode != NULL)
        {
            if(!currentNode->m_bullet->hasRange())
            {
                currentNode->m_bullet->playBulletCollisionSound();
                
                if(!currentNode->m_bullet->isExplodingType())
                {
                    bulletNode *nodeToDelete = currentNode; 
                    currentNode = currentNode->m_nextBulletNode;
                    deleteBulletNode(nodeToDelete);
                }
                else
                {
                    if(!currentNode->m_bullet->hasExploded())
                    {
                        currentNode->m_bullet->explodeBullet();
                        currentNode = currentNode->m_nextBulletNode;

                    }
                    else
                    {
                        bulletNode *nodeToDelete = currentNode; 
                        currentNode = currentNode->m_nextBulletNode;
                        deleteBulletNode(nodeToDelete);
                    }
                }
            }
            else
            {
                currentNode->m_bullet->updatePosition();
                currentNode = currentNode->m_nextBulletNode;
            }
        }
        m_lastBulletUpdate = millis();
    }
}

bool bulletList::checkBulletOnPos(int p_xPos, int p_yPos)
{
    bulletNode *currentNode = m_firstBulletNode;
    while(currentNode != NULL)
    {
        int xBulletPos, yBulletPos;
        currentNode->m_bullet->getCoordonates(xBulletPos, yBulletPos);
        if(xBulletPos == p_xPos && yBulletPos == p_yPos)
        {
            return true;
        }

        // if in explosion make it damage the enemy as well 
        if(currentNode->m_bullet->isExplodingType() && !currentNode->m_bullet->hasRange())
        {
            int leftOfExplosion = xBulletPos - 1;
            int rightOfExplosion = xBulletPos + 1;
            int topOfExplosion = yBulletPos - 1;
            int bottomOfExplosion = yBulletPos + 1;

            if(leftOfExplosion < 0)
            {
                leftOfExplosion = MATRIX_SIZE - 1;
            }
            if(topOfExplosion < 0)
            {
                topOfExplosion = MATRIX_SIZE - 1;
            }
            if(rightOfExplosion == MATRIX_SIZE)
            {
                rightOfExplosion = 0;
            }
            if(bottomOfExplosion == MATRIX_SIZE)
            {
                bottomOfExplosion = 0;
            }

            if(    leftOfExplosion == p_xPos && yBulletPos == p_yPos
                || rightOfExplosion == p_xPos && yBulletPos == p_yPos 
                || xBulletPos == p_xPos && topOfExplosion == p_yPos 
                || xBulletPos == p_xPos && bottomOfExplosion == p_yPos )
            {
                return true;
            }

        }

        currentNode = currentNode->m_nextBulletNode;
    }
    return false;
}

void bulletList::setLastBulletUpdate()
{
    m_lastBulletUpdate = millis();
}