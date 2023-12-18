#include "matrixEntity.h"

void matrixEntity::dealWithOutOfBounds(int &p_xPos, int &p_yPos)
{
    if(p_xPos < 0)
    {
        p_xPos = MATRIX_SIZE - 1;
    }
    if(p_yPos < 0)
    {
        p_yPos = MATRIX_SIZE - 1;
    }
    if(p_xPos >= MATRIX_SIZE)
    {
        p_xPos = 0;
    }
    if(p_yPos >= MATRIX_SIZE)
    {
        p_yPos = 0;
    }
}  

void matrixEntity::getCoordonates(int &p_xPos, int &p_yPos)
{
    p_xPos = m_xPos;
    p_yPos = m_yPos;
}