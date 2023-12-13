#include "matrixEntity.h"

void matrixEntity::initialPositionUpdate(int &p_xPos, int &p_yPos, int p_xCommand, int p_yCommand)
{
    if(p_xCommand < MIN_JS_THRESHOLD)
    {
        p_xPos = p_xPos - 1;
    }
    if(p_yCommand < MIN_JS_THRESHOLD)
    {
        p_yPos = p_yPos + 1;
    }
    if(p_xCommand > MAX_JS_THRESHOLD)
    {
        p_xPos = p_xPos + 1;
    }
    if(p_yCommand > MAX_JS_THRESHOLD)
    {
        p_yPos = p_yPos - 1;
    }
}

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

void matrixEntity::pickAnAxes(int &p_xNextPos, int &p_yNextPos, int p_xPos, int p_yPos, int p_xCommand, int p_yCommand)
{
    if(p_xNextPos != p_xPos && p_yNextPos != p_yPos)
    {
        if(absolute(p_xCommand - JS_DEFAULT_VALUE) > absolute(p_yPos - JS_DEFAULT_VALUE))
        {
            p_yNextPos = p_yPos;
        }
        else
        {
            p_xNextPos = p_xPos;
        }
    }
}