#include "inputHwControl.h"

void inputHwControl::playButtonSound(const int p_frequency)
{
    // don't play anything if sound is disabled
    if(g_disableSound)
    {
        return;
    }

    tone(BUZZER_PIN, p_frequency, DEFAULT_SOUND_DURATION);
}

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
                playButtonSound(FREQ_BUTTON);
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
                playButtonSound(FREQ_BUTTON);
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

bool inputHwControl::pressedBackButton()
{
    unsigned long time = millis();
    int state = !digitalRead(BACK_BUTTON_PIN);

    // if the button has a constant state
    if(state == m_backBtn.m_prevState)
    {
        // if the constant state has been kept for a while
        if(time - m_backBtn.m_prevTime > DEBOUNCE_TIME && m_backBtn.m_prevCountedState != state)
        {
            m_backBtn.m_prevCountedState = state;

            if(state == HIGH)
            {
                playButtonSound(FREQ_BACK_BUTTON);
                m_backBtn.m_prevState = state;
                return true;
            }
        }
    }
    else
    {
        m_backBtn.m_prevTime = time;
    }

    m_backBtn.m_prevState = state;
    
    return false;
}

void inputHwControl::playJoystickSound()
{
    // don't play anything if sound is disabled
    if(g_disableSound)
    {
        return;
    }
    
    if(millis() - m_lastJoystickSound > CYCLE_DELAY_MILLIS)
    {
        m_lastJoystickSound = millis();
        tone(BUZZER_PIN, FREQ_JOYSTICK, DEFAULT_SOUND_DURATION);
    }
}


bool inputHwControl::joystickDetected(int &p_xCommand, int &p_yCommand)
{
    p_xCommand = analogRead(JS_X_PIN);
    p_yCommand = analogRead(JS_Y_PIN);

    if(p_xCommand < MIN_JS_THRESHOLD || p_xCommand > MAX_JS_THRESHOLD || p_yCommand < MIN_JS_THRESHOLD || p_yCommand > MAX_JS_THRESHOLD)
    {
        playJoystickSound();
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
                playJoystickSound();
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
                playJoystickSound();
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
                playJoystickSound();
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
                playJoystickSound();
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