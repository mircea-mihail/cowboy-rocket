#include "inputHwControl.h"

bool inputHwControl::pressedButton()
{
    unsigned long time = millis();
    int state = !digitalRead(BUTTON_PIN);

    // if the button has a constant state
    if(state == m_btn.m_prevState)
    {
        // if the constant state has been kept for a while
        if(time - m_btn.m_prevTime > DEBOUNCE_TIME && m_btn.m_prevCountedState != state)
        {
            m_btn.m_prevCountedState = state;

            if(state == HIGH)
            {
                m_btn.m_prevState = state;
                return true;
            }
        }
    }
    else
    {
        m_btn.m_prevTime = time;
    }

    m_btn.m_prevState = state;
    
    return false;
}

bool inputHwControl::pressedButtonFor(unsigned long p_sameStateMs)
{
    unsigned long time = millis();
    int state = !digitalRead(BUTTON_PIN);

    // if the button has a constant state
    if(state == m_btnLongerWait.m_prevState)
    {
        // if the constant state has been kept for a while
        if(time - m_btnLongerWait.m_prevTime > p_sameStateMs && m_btnLongerWait.m_prevCountedState != state)
        {
            m_btnLongerWait.m_prevCountedState = state;

            if(state == HIGH)
            {
                m_btnLongerWait.m_prevState = state;
                m_btnLongerWait.resetStates(); 
                return true;
            }
        }
    }
    else
    {
        m_btnLongerWait.m_prevTime = time;
    }

    m_btnLongerWait.m_prevState = state;
    
    return false;
}

bool inputHwControl::joystickDetected(int &p_xCommand, int &p_yCommand)
{
    p_xCommand = analogRead(JS_X_PIN);
    p_yCommand = analogRead(JS_Y_PIN);

    if(p_xCommand < MIN_JS_THRESHOLD || p_xCommand > MAX_JS_THRESHOLD || p_yCommand < MIN_JS_THRESHOLD || p_yCommand > MAX_JS_THRESHOLD)
    {
        return true;
    }
    return false;
}

bool inputHwControl::joystickLeft()
{
    int xCommand = analogRead(JS_X_PIN);
    int yCommand = analogRead(JS_Y_PIN);

    if(xCommand < MIN_JS_THRESHOLD || xCommand > MAX_JS_THRESHOLD || yCommand < MIN_JS_THRESHOLD || yCommand > MAX_JS_THRESHOLD)
    {
        if(absolute(JS_DEFAULT_VALUE - xCommand) < absolute(JS_DEFAULT_VALUE - yCommand))
        {
            if(yCommand > MAX_JS_THRESHOLD)
            {
                return true;
            }
        }   
    }
    return false;
}

bool inputHwControl::joystickRight()
{
    int xCommand = analogRead(JS_X_PIN);
    int yCommand = analogRead(JS_Y_PIN);

    if(xCommand < MIN_JS_THRESHOLD || xCommand > MAX_JS_THRESHOLD || yCommand < MIN_JS_THRESHOLD || yCommand > MAX_JS_THRESHOLD)
    {
        if(absolute(JS_DEFAULT_VALUE - xCommand) < absolute(JS_DEFAULT_VALUE - yCommand))
        {
            if(yCommand < MIN_JS_THRESHOLD)
            {
                return true;
            }
        }   
    }
    return false;   
}

bool inputHwControl::joystickUp()
{
    int xCommand = analogRead(JS_X_PIN);
    int yCommand = analogRead(JS_Y_PIN);

    if(xCommand < MIN_JS_THRESHOLD || xCommand > MAX_JS_THRESHOLD || yCommand < MIN_JS_THRESHOLD || yCommand > MAX_JS_THRESHOLD)
    {
        if(absolute(JS_DEFAULT_VALUE - xCommand) > absolute(JS_DEFAULT_VALUE - yCommand))
        {
            if(xCommand < MIN_JS_THRESHOLD)
            {
                return true;
            }
        }   
    }
    return false;
}

bool inputHwControl::joystickDown()
{
    int xCommand = analogRead(JS_X_PIN);
    int yCommand = analogRead(JS_Y_PIN);

    if(xCommand < MIN_JS_THRESHOLD || xCommand > MAX_JS_THRESHOLD || yCommand < MIN_JS_THRESHOLD || yCommand > MAX_JS_THRESHOLD)
    {
        if(absolute(JS_DEFAULT_VALUE - xCommand) > absolute(JS_DEFAULT_VALUE - yCommand))
        {
            if(xCommand > MAX_JS_THRESHOLD)
            {
                return true;
            }
        }   
    }
    return false;   
}



int inputHwControl::getBrightness()
{
    return analogRead(BRIGHTNESS_PIN);
}