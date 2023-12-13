#ifndef HARDWARE_CONTROL_H
#define HARDWARE_CONTROL_H

#include <Arduino.h>
#include "utility.h"

// button
#define DEBOUNCE_TIME 50

class inputHwControl
{
private:

    struct button
    {
        int m_prevState = LOW;
        int m_prevCountedState = LOW;
        unsigned long m_prevTime = 0;
    };

    button m_btn;

public:
    // returns true if the button has been pressed -> does debounce and sig rise detection 
    bool pressedButton();

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