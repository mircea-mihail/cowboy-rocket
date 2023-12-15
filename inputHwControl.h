#ifndef HARDWARE_CONTROL_H
#define HARDWARE_CONTROL_H

#include <Arduino.h>
#include "utility.h"
#include "mySounds.h"

// button
#define DEBOUNCE_TIME 50

extern bool g_disableSound;

class inputHwControl
{
private:

    struct button
    {
        int m_prevState = LOW;
        int m_prevCountedState = LOW;
        unsigned long m_prevTime = 0;

        // needed when the readings are rare and biased( as in the case for the hold ) 
        // -> prevState and prevCountedState don't get to even out as in the case with normal button reads
        void resetStates()
        {
            m_prevState = LOW;   
            m_prevCountedState = LOW;
            m_prevTime = 0;
        }
    };

    button m_btn;
    button m_btnLongerWait;
    button m_backBtn;

    unsigned long m_lastJoystickSound = 0;

public:
    // plays the buzzer sound only once every button press. Checks if sound is off and does not play if the case
    void playButtonSound(const int p_frequency);

    // returns true if the button has been pressed -> does debounce and sig rise detection 
    bool pressedButton();

    // returns true if the button stayed pressed for p_ms millis
    bool pressedButtonFor(unsigned long p_p_sameStateMsms);

    // returns true if the back button has been pressed (debounce and sig rise detection)
    bool pressedBackButton();

    // plays the buzzer sound only once every js move. Checks if sound is off and does not play if the case
    void playJoystickSound();

    // detects joystick movement (any direction) and returns the values of x and y read
    bool joystickDetected(int &p_xCommand, int &p_yCommand);

    // returns true when the player moves the joystick left. False otherwise
    bool joystickLeft();

    // returns true when the player moves the joystick right. False otherwise
    bool joystickRight();
    
    // returns true when the player moves the joystick down. False otherwise
    bool joystickDown();
    
    // returns true when the player moves the joystick up. False otherwise
    bool joystickUp();

    // returns the brightness value returned by the brightness sensor
    int getBrightness();
};

#endif