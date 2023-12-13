#include "gameMenu.h"

//////////////////////////// private methods:

void gameMenu::changeState(int &p_state, int p_newState)
{
    p_state = p_newState;
    
    m_lcd.clear();
    m_changedState = true;
}

bool gameMenu::doIntroMessageSequence()
{
    // don't enter if intro message has been shown
    if(m_showedIntroMessage)
    {
        return false;
    }

    if(millis() - m_introMessageTime < INTRO_MESSAGE_MILLIS)
    {
        if(m_changedState)
        {
            displayStartMessage();
            m_changedState = false;
        }
    }
    else if (!m_showedIntroMessage)
    {
        m_showedIntroMessage = true;
        m_lcd.clear();
        m_changedState = true;
        return false;
    }

    // bypass intro message mechanism
    int xCommand, yCommand;
    if(m_hwCtrl.pressedButton() || m_hwCtrl.joystickDetected(xCommand, yCommand))
    {
        m_showedIntroMessage = true;
        m_lcd.clear();
        m_changedState = true;
        return false;
    }

    return true;
}

bool gameMenu::doEndMessageSequence()
{
    // don't enter if intro message has been shown
    if(!m_showEndMessage)
    {
        return false;
    }

    if(millis() - m_endMessageTime < END_MESSAGE_MILLIS)
    {
        if(m_changedState)
        {
            displayEndMessage();
            m_changedState = false;
        }
    }
    else if (m_showEndMessage)
    {
        m_showEndMessage = false;
        m_lcd.clear();
        m_changedState = true;
        return false;
    }

    // bypass end message mechanism
    int xCommand, yCommand;
    if(m_hwCtrl.pressedButton() || m_hwCtrl.joystickDetected(xCommand, yCommand))
    {
        m_showEndMessage = false;
        m_lcd.clear();
        m_changedState = true;
        return false;
    }
    return true;
}

void gameMenu::displayStartMessage()
{
    m_lcd.setCursor(SECOND_LCD_COL, FIRST_LCD_ROW);
    m_lcd.print(F("Welcome to ..."));
    m_lcd.setCursor(SECOND_LCD_COL, SECOND_LCD_ROW);
    m_lcd.print(F("ROCKET  COWBOY"));
}

void gameMenu::displayEndMessage()
{
    m_lcd.setCursor(FIRST_LCD_COL, FIRST_LCD_ROW);
    m_lcd.print(F("Well done COWBOY"));
    m_lcd.setCursor(SECOND_LCD_COL, SECOND_LCD_ROW);
    m_lcd.print(F("keep  slingin'"));   
}

void gameMenu::keepStateInBounds(int &p_state, const int p_lowerBound, const int p_upperBound)
{
    if(p_state > p_upperBound)
    {
        p_state = p_lowerBound;
    }

    if(p_state < p_lowerBound)
    {
        p_state = p_upperBound;
    }
}

void gameMenu::refreshMenuVariables()
{
    m_showAboutText = false;
}

void gameMenu::goToNextMenuOption(int &p_currentState, const int p_lowerBound, const int p_upperBound)
{
    int xJsCommand, yJsCommand;
    if(m_hwCtrl.joystickDetected(xJsCommand, yJsCommand))
    {
        if(millis() - m_lastCycleTime > CYCLE_DELAY_MILLIS)
        {
            // i want to only account for ups and downs, no lefts or rights
            if(absolute(JS_DEFAULT_VALUE - xJsCommand) < absolute(JS_DEFAULT_VALUE - yJsCommand))
            {
                return;
            }

            m_lastCycleTime = millis();

            if(xJsCommand < MIN_JS_THRESHOLD)
            {
                p_currentState --;
            }
            else if(xJsCommand > MIN_JS_THRESHOLD)
            {
                p_currentState ++;
            }
    
            keepStateInBounds(p_currentState, p_lowerBound, p_upperBound);

            m_lcd.clear();
            m_changedState = true;
            refreshMenuVariables();
        }
    }
}

void gameMenu::printHashesLCD(int p_fillAmount)
{
    m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);

    int percent = map(p_fillAmount, 0, PWM_RESOLUTION, LCD_COLS, 0);
    for(int i = 0; i < LCD_COLS; i++)
    {
        if(i < percent)
        {
            m_lcd.print("#");
        }
        else
        {
            m_lcd.print(" ");
        }
    }
}

void gameMenu::printHashesForMatrix(int p_fillAmount)
{
    m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);

    for(int i = 0; i < LCD_COLS; i++)
    {
        if(i <= p_fillAmount)
        {
            m_lcd.print("#");
        }
        else
        {
            m_lcd.print(" ");
        }
    }
}

void gameMenu::goToSettingsMenu()
{
    goToNextMenuOption(m_settingsState, IN_MATRIX_BRIGHTNESS, IN_LCD_BRIGHTNESS);

    if(m_hwCtrl.joystickLeft() && m_hwCtrl.pressedButton())
    {
        m_inSubmenu = false;
        changeState(m_settingsState, RETURN_FROM_SETTINGS);
    }

    switch (m_settingsState)
    {
    case IN_MATRIX_BRIGHTNESS:
        if(m_changedState)
        {
            m_lcd.print(F("<     mat "));
            m_lcd.write(SUN_CHAR);
            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
            m_lcd.print(F("< "));
            printHashesForMatrix(g_map.getMatrixBrightness());

            m_changedState = false;
        }
        
        if(m_hwCtrl.joystickLeft())
        {
            if(millis() - m_lastMatrixBrightnessChange > CYCLE_DELAY_MILLIS)
            {
                m_lastMatrixBrightnessChange = millis();
                byte matrixBrightness = g_map.decrementMatrixBrightness();
                printHashesForMatrix(matrixBrightness);
                
                EEPROM.update(EEPROM_MATRIX_BRIGHTNESS_ADDRESS, matrixBrightness);
            }
        }

        if(m_hwCtrl.joystickRight())
        {
            if(millis() - m_lastMatrixBrightnessChange > CYCLE_DELAY_MILLIS)
            {
                m_lastMatrixBrightnessChange = millis();
                byte matrixBrightness = g_map.incrementMatrixBrightness();
                printHashesForMatrix(matrixBrightness);
                
                EEPROM.update(EEPROM_MATRIX_BRIGHTNESS_ADDRESS, matrixBrightness);
            }
        }

        break;

    case IN_LCD_CONTRAST:
        if(m_changedState)
        {
            m_lcd.print(F("<     lcd "));
            m_lcd.write(CONTRAST_CHAR);
            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
            m_lcd.print(F("< "));
            printHashesLCD(m_lcdContrast);
            
            m_changedState = false;
        }

        if(m_hwCtrl.joystickLeft())
        {
            if(millis() - m_lastContrastChange > CYCLE_DELAY_MILLIS)
            {
                m_lastContrastChange = millis();
                m_lcdContrast += LCD_INCREMENT_VAL;
                printHashesLCD(m_lcdContrast);

                analogWrite(LCD_CONTRAST, m_lcdContrast);
                EEPROM.update(EEPROM_LCD_CONTRAST_ADDRESS, m_lcdContrast);
            }
        }

        if(m_hwCtrl.joystickRight())
        {
            if(millis() - m_lastContrastChange > CYCLE_DELAY_MILLIS)
            {
                m_lastContrastChange = millis();
                m_lcdContrast -= LCD_INCREMENT_VAL;
                printHashesLCD(m_lcdContrast);

                analogWrite(LCD_CONTRAST, m_lcdContrast);
                EEPROM.update(EEPROM_LCD_CONTRAST_ADDRESS, m_lcdContrast);

            }
        }
        break;

    case IN_LCD_BRIGHTNESS:
        if(m_changedState)
        {
            m_lcd.print(F("<     lcd "));
            m_lcd.write(SUN_CHAR);
            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
            m_lcd.print(F("< "));
            printHashesLCD(PWM_RESOLUTION - m_lcdBrightness);

            m_changedState = false;
        }

        if(m_hwCtrl.joystickLeft())
        {
            if(millis() - m_lastBrightnessChange > CYCLE_DELAY_MILLIS)
            {
                m_lastBrightnessChange = millis();
                m_lcdBrightness -= LCD_INCREMENT_VAL;
                printHashesLCD(PWM_RESOLUTION - m_lcdBrightness);

                analogWrite(LCD_BRIGHTNESS, m_lcdBrightness);
                EEPROM.update(EEPROM_LCD_BRIGHTNESS_ADDRESS, m_lcdBrightness);
            }
        }

        if(m_hwCtrl.joystickRight())
        {
            if(millis() - m_lastBrightnessChange > CYCLE_DELAY_MILLIS)
            {
                m_lastBrightnessChange = millis();
                m_lcdBrightness += LCD_INCREMENT_VAL;
                printHashesLCD(PWM_RESOLUTION - m_lcdBrightness);

                analogWrite(LCD_BRIGHTNESS, m_lcdBrightness);
                EEPROM.update(EEPROM_LCD_BRIGHTNESS_ADDRESS, m_lcdBrightness);
            }
        }
        break;
    case RETURN_FROM_SETTINGS:
        break;
    
    default:
        break;
    }
}

////////////////////////// public methods:

gameMenu::gameMenu()
{
    m_lcdBrightness = EEPROM.read(EEPROM_LCD_BRIGHTNESS_ADDRESS);
    m_lcdContrast = EEPROM.read(EEPROM_LCD_CONTRAST_ADDRESS);

    analogWrite(LCD_BRIGHTNESS, m_lcdBrightness);
    m_lcd.begin(LCD_COLS, LCD_ROWS);
    m_lcd.clear();
    analogWrite(LCD_CONTRAST, m_lcdContrast);

    m_lcd.createChar(SUN_CHAR, m_customCharArray[SUN_CHAR]);
    m_lcd.createChar(CONTRAST_CHAR, m_customCharArray[CONTRAST_CHAR]);
}

int gameMenu::menuSequence()
{
    // only do intro/outro message seq in menu sequence;
    if(doIntroMessageSequence())
    {
        return m_state;
    }
    if(doEndMessageSequence())
    {
        return m_state;
    }

    if(m_state != MENU_IN_GAME && !m_inSubmenu)
    {
        goToNextMenuOption(m_state, MENU_IN_START_GAME, MENU_IN_ABOUT);
    }

    switch (m_state)
    {
    case MENU_IN_START_GAME:
        if(m_changedState)
        {
            g_map.displayIcon(ICON_START);
            
            m_lcd.print(F("   start game"));
            m_changedState = false;
        }

        if(m_hwCtrl.pressedButton())
        {
            changeState(m_state, MENU_IN_GAME);   
        }
        break;

    case MENU_IN_SETTINGS:
        if(m_settingsState == RETURN_FROM_SETTINGS)
        {
            if(m_changedState)
            {
                g_map.displayIcon(ICON_HAMMER);

                m_lcd.print(F("    settings   >"));
                m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
                m_lcd.print(F("               >"));

                m_changedState = false;
                m_inSubmenu = false;
            }

            if(m_hwCtrl.joystickRight() && m_hwCtrl.pressedButton())
            {
                m_inSubmenu = true;
                changeState(m_settingsState, IN_MATRIX_BRIGHTNESS);
            }
        }
        else
        {
            goToSettingsMenu();
        }
        break;

    case MENU_IN_ABOUT:
        if(m_changedState)
        {
            g_map.displayIcon(ICON_ABOUT);

            if(!m_showAboutText)
            {
                m_lcd.print(F("   learn more"));

                m_changedState = false;
            }
            else
            {
                m_lcd.print(F("Rocket Cowboy by Mircea Mihail Ionescu"));
                m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_COL);
                m_lcd.print(F("github.com/mircea-mihail for more info"));
                m_changedState = false;
            }
        }
        if(m_showAboutText)
        {
            if(m_hwCtrl.joystickLeft())
            {
                if(millis() - m_lcdScrollChange > CYCLE_DELAY_MILLIS)
                {
                    m_lcd.scrollDisplayRight();
                    m_lcdScrollChange = millis();
                }
            }
            if(m_hwCtrl.joystickRight())
            {
                if(millis() - m_lcdScrollChange > CYCLE_DELAY_MILLIS)
                {
                    m_lcd.scrollDisplayLeft();
                    m_lcdScrollChange = millis();
                }
            }
        }

        if(m_hwCtrl.pressedButton())
        {
            m_showAboutText = !m_showAboutText;
            m_lcd.clear();
            m_changedState = true;
        }
        break; 

    case MENU_IN_GAME:
        int wallsOnMap = g_map.getWallsLeft();
        if(wallsOnMap != m_wallsLeftOnMap)
        {
            m_lcd.clear();
            m_wallsLeftOnMap = wallsOnMap;
            m_lcd.print(F(" walls left:"));
            m_lcd.print(m_wallsLeftOnMap);
                        
            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
            m_lcd.print(F(" get 'em COWBOY"));
        }
        break;    
    
    default:
        break;
    }

    return m_state;
}  

void gameMenu::printEndMessage()
{
    m_showEndMessage = true;
    m_endMessageTime = millis();
}

void gameMenu::setStateToDefault()
{
    changeState(m_state, MENU_IN_START_GAME);
}