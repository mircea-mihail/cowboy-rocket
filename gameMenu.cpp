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
    if(m_hwCtrl.pressedBackButton() || m_hwCtrl.joystickDetected(xCommand, yCommand))
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
    if(m_hwCtrl.pressedBackButton() || m_hwCtrl.joystickDetected(xCommand, yCommand))
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
    m_lcd.print(F("COWBOY  ROCKET"));
}

void gameMenu::displayEndMessage()
{
    m_lcd.setCursor(FIRST_LCD_COL, FIRST_LCD_ROW);
    m_lcd.print(F("Well done COWBOY"));
    m_lcd.setCursor(SECOND_LCD_COL, SECOND_LCD_ROW);
    m_lcd.print(F("keep  slingin'"));   
}

void gameMenu::keepInBounds(int &p_value, const int p_lowerBound, const int p_upperBound)
{
    if(p_value > p_upperBound)
    {
        p_value = p_lowerBound;
    }

    if(p_value < p_lowerBound)
    {
        p_value = p_upperBound;
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
    
            keepInBounds(p_currentState, p_lowerBound, p_upperBound);

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

void gameMenu::updateMatrixBrightness()
{
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
}

void gameMenu::updateLcdContrast()
{
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
}

void gameMenu::updateLcdBrightness()
{
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
}

void gameMenu::displayNameCharArrow(int p_initialSpaceOffset, int p_spaceOffset, int p_numberOfOffsets)
{
    for(int i = 0; i < p_initialSpaceOffset; i++)
    {
        m_lcd.print(" ");
    }

    for(int idx = 0; idx < p_numberOfOffsets; idx++)
    {
        for(int i = 0; i < p_initialSpaceOffset; i++)
        {
            m_lcd.print(" ");
        }
    }
    
    m_lcd.write(ARROW_UP_CHAR);
}

void gameMenu::goToChangeName()
{
    if(m_hwCtrl.pressedBackButton())
    {
        m_inSettingsSubmenu = false;
        m_changedState = true;
        m_lcd.clear();

        // save last name used in memory
        for(int nameIdx = 0; nameIdx < LETTERS_IN_NAME; nameIdx ++)
        {
            EEPROM.update(ADDRESS_OF_LAST_NAME_USED + nameIdx, m_nameArray[nameIdx]);
        }
        
        return;
    }

    if(m_changedState)
    {
        m_lcd.print("   ");
        for(int idx = 0; idx < LETTERS_IN_NAME; idx++)
        {
            m_lcd.print(m_nameArray[idx]);
            m_lcd.print("  ");
        }
        m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
        m_changedState = false;

        const byte spacesBeforeFirstArrow = 3;
        const byte spacesBetweenArrows = 3;
        displayNameCharArrow(spacesBeforeFirstArrow, spacesBetweenArrows, m_nameArrayIdx);
    }

    if(m_hwCtrl.joystickLeft())
    {
        if(millis() - m_lastNameArrowChange > CYCLE_DELAY_MILLIS)
        {
            m_changedState = true;
            m_nameArrayIdx--;
            keepInBounds(m_nameArrayIdx, 0, LETTERS_IN_NAME - 1);
            
            m_lastNameArrowChange = millis();
            m_lcd.clear();
        }
    }

    if(m_hwCtrl.joystickRight())
    {
        if(millis() - m_lastNameArrowChange > CYCLE_DELAY_MILLIS)
        {
            m_changedState = true;
            m_nameArrayIdx++;
            keepInBounds(m_nameArrayIdx, 0, LETTERS_IN_NAME - 1);
            
            m_lastNameArrowChange = millis();
            m_lcd.clear();
        }
    }

    if(m_hwCtrl.joystickUp())
    {
        if(millis() - m_lastNameArrowChange > CYCLE_DELAY_MILLIS)
        {
            if(m_nameArray[m_nameArrayIdx] == 'Z')
            {
                m_nameArray[m_nameArrayIdx] = ' ';
            }
            else if(m_nameArray[m_nameArrayIdx] == ' ')
            {
                m_nameArray[m_nameArrayIdx] = 'A';
            }
            else
            {
                m_nameArray[m_nameArrayIdx] ++;
            }

            m_lastNameArrowChange = millis();
            m_changedState = true;
            m_lcd.clear();
        }
    }

    if(m_hwCtrl.joystickDown())
    {
        if(millis() - m_lastNameArrowChange > CYCLE_DELAY_MILLIS)
        {
            if(m_nameArray[m_nameArrayIdx] == 'A')
            {
                m_nameArray[m_nameArrayIdx] = ' ';
            }
            else if(m_nameArray[m_nameArrayIdx] == ' ')
            {
                m_nameArray[m_nameArrayIdx] = 'Z';
            }
            else
            {
                m_nameArray[m_nameArrayIdx] --;
            }

            m_lastNameArrowChange = millis();
            m_changedState = true;
            m_lcd.clear();
        }
    }
}

void gameMenu::doResetScoresLogic()
{
    if(m_hwCtrl.pressedBackButton())
    {
        m_deleteHighScores = NOT_SURE_YET;
        m_inSettingsSubmenu = false;
        m_changedState = true;
        m_lcd.clear();
    }

    switch (m_deleteHighScores)
    {
    case NOT_SURE_YET:
        if(m_changedState)
        {
            //             ----\\\\----////
            m_lcd.print(F("  reset scores"));
            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
            
            m_lcd.write(ARROW_RIGHT_CHAR);
            m_lcd.print(F("confirm delete"));
            m_lcd.write(ARROW_LEFT_CHAR);

            m_changedState = false;
        }
        if(m_hwCtrl.pressedButton())
        {
            m_changedState = true;
            m_inSettingsSubmenu = true;
            m_lcd.clear();
            m_deleteHighScores ++;
        }

        break;
    
    case ARE_YOU_SURE:
        if(m_changedState)
        {
            //             ----\\\\----////
            m_lcd.print(F("? are you sure ?"));
            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
            
            m_lcd.print("  ");
            m_lcd.write(ARROW_RIGHT_CHAR);
            m_lcd.print(F(" yes I am "));
            m_lcd.write(ARROW_LEFT_CHAR);

            m_changedState = false;
        }
        if(m_hwCtrl.pressedButton())
        {
            m_changedState = true;
            m_inSettingsSubmenu = true;
            m_lcd.clear();
            m_deleteHighScores ++;
        }

        break;

    case SURE_SURE:
        if(m_changedState)
        {
            //             ----\\\\----////
            m_lcd.print(F("  ? positive ?"));
            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
            
            m_lcd.write(ARROW_RIGHT_CHAR);
            m_lcd.print(F("delete my data"));
            m_lcd.write(ARROW_LEFT_CHAR);

            m_changedState = false;
        }
        if(m_hwCtrl.pressedButton())
        {
            m_changedState = true;
            m_inSettingsSubmenu = true;
            m_lcd.clear();
            m_deleteHighScores ++;

            unsigned long smallestScore = 0;
            char defaultLetter = ' ';
            for(int memPtr = SCORE_MEMORY_ADDRESS; memPtr < ADDRESS_AFTER_LAST_SCORE; memPtr += SCORE_SIZE_IN_MEMORY)
            {
                EEPROM.put(memPtr, smallestScore);
            }
            for(int memPtr = SCORE_NAMES_ADDRESS; memPtr < ADDRESS_AFTER_LAST_SCORE_NAME; memPtr += sizeof(char))
            {
                EEPROM.put(memPtr, defaultLetter);
            }
        }

        break;

    case ABSOLUTELY_POSITIVE:
        if(m_changedState)
        {
            //             ----\\\\----////
            m_lcd.print(F("  data deleted"));
            m_changedState = false;
        }
        if(m_hwCtrl.pressedButton())
        {
            m_changedState = true;
            m_inSettingsSubmenu = false;
            m_lcd.clear();
            m_deleteHighScores = NOT_SURE_YET;
        }

        break;

    default:
        break;
    }
}

void gameMenu::goToSettingsMenu()
{
    if(!m_inSettingsSubmenu)
    {
        goToNextMenuOption(m_settingsState, LOWEST_SETTINGS_STATE, HIGHEST_SETTINGS_STATE);
        
        // to exit submenu
        if(m_hwCtrl.pressedBackButton())
        {
            m_inSubmenu = false;
            changeState(m_settingsState, RETURN_FROM_SETTINGS);
        }
    }

    // remember last submenu state
    if(m_settingsState != RETURN_FROM_SETTINGS)
    {
        m_previousSettingsState = m_settingsState;
    }

    switch (m_settingsState)
    {
    case IN_MATRIX_BRIGHTNESS:
        if(m_changedState)
        {
            m_lcd.print(F("      mat "));
            m_lcd.write(SUN_CHAR);
            printHashesForMatrix(g_map.getMatrixBrightness());

            m_changedState = false;
        }
        updateMatrixBrightness();

        break;

    case IN_LCD_CONTRAST:
        if(m_changedState)
        {
            m_lcd.print(F("      lcd "));
            m_lcd.write(CONTRAST_CHAR);
            printHashesLCD(m_lcdContrast);
            
            m_changedState = false;
        }
        updateLcdContrast();

        break;

    case IN_LCD_BRIGHTNESS:
        if(m_changedState)
        {
            m_lcd.print(F("      lcd "));
            m_lcd.write(SUN_CHAR);
            printHashesLCD(PWM_RESOLUTION - m_lcdBrightness);

            m_changedState = false;
        }
        updateLcdBrightness();
        
        break;
        
    case IN_ENTER_NAME:
        if(!m_inSettingsSubmenu)
        {
            if(m_changedState)
            {
                m_lcd.print(F("   name: "));
                for(int nameIdx = 0; nameIdx < LETTERS_IN_NAME; nameIdx++)
                {
                    m_lcd.print(m_nameArray[nameIdx]);
                }
                m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
                m_lcd.print("   ");
                m_lcd.write(ARROW_RIGHT_CHAR);       
                m_lcd.print(F(" change "));
                m_lcd.write(ARROW_LEFT_CHAR);       

                m_changedState = false;
            }

            if(m_hwCtrl.pressedButton())
            {   
                m_inSettingsSubmenu = true;
                m_changedState = true;
                m_lcd.clear();
            }
        }
        else
        {
            goToChangeName();
        }

        break;
    case IN_TOGGLE_SOUND:
        if(m_changedState)
        {
            m_lcd.print(F("    sound "));
            m_lcd.print(g_disableSound ? "off" : "on");
            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
            m_lcd.print("   ");
            m_lcd.write(ARROW_RIGHT_CHAR);
            m_lcd.print(F(" toggle "));    
            m_lcd.write(ARROW_LEFT_CHAR);

            m_changedState = false;
        }
        if(m_hwCtrl.pressedButton())
        {
            g_disableSound = !g_disableSound;
            m_changedState = true;
            m_lcd.clear();
            EEPROM.update(ADDRESS_OF_SOUND_TOGGLE, g_disableSound);
        }

        break;

    case IN_RESET_HIGH_SCORES:
        doResetScoresLogic();
        break;

    case RETURN_FROM_SETTINGS:
        break;
    
    default:
        break;
    }
}

void gameMenu::displayAboutMenu()
{
    g_map.displayIcon(ICON_ABOUT);

    if(!m_showAboutText)
    {
        m_lcd.print(F(" "));
        m_lcd.write(ARROW_RIGHT_CHAR);
        m_lcd.print(F(" learn more "));
        m_lcd.write(ARROW_LEFT_CHAR);

        m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
        m_lcd.print(F("   start game"));

        m_changedState = false;
    }
    else
    {
        m_lcd.print(F("Cowboy Rocket by Mircea Mihail Ionescu"));
        m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_COL);
        m_lcd.print(F("github.com/mircea-mihail for more info"));
        m_changedState = false;
    }
}

void gameMenu::doScrollMenuLogic(bool &p_showText)
{
    if(p_showText)
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

    if(m_hwCtrl.pressedButton() && !p_showText)
    {
        m_inSubmenu = true;
        p_showText = !p_showText;
        m_lcd.clear();
        m_changedState = true;
    }
    
    if(m_hwCtrl.pressedBackButton() && p_showText)
    {
        m_inSubmenu = false;
        p_showText = !p_showText;
        m_lcd.clear();
        m_changedState = true;
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
    
    for(int nameIdx = 0; nameIdx < LETTERS_IN_NAME; nameIdx ++)
    {
        m_nameArray[nameIdx] = EEPROM.read(ADDRESS_OF_LAST_NAME_USED + nameIdx);
    }
    g_disableSound = EEPROM.read(ADDRESS_OF_SOUND_TOGGLE);

    m_lcd.createChar(SUN_CHAR, m_customCharArray[SUN_CHAR]);
    m_lcd.createChar(CONTRAST_CHAR, m_customCharArray[CONTRAST_CHAR]);
    m_lcd.createChar(ARROW_RIGHT_CHAR, m_customCharArray[ARROW_RIGHT_CHAR]);
    m_lcd.createChar(ARROW_LEFT_CHAR, m_customCharArray[ARROW_LEFT_CHAR]);
    m_lcd.createChar(ARROW_UP_CHAR, m_customCharArray[ARROW_UP_CHAR]);    
}

void gameMenu::getWinnersAndScores(char p_namesOfWinners[NUMBER_OF_SCORES_KEPT][SIZE_OF_NAME_IN_EEPROM], unsigned long p_winnerScores[NUMBER_OF_SCORES_KEPT])
{
    for(int scoreIdx = 0; scoreIdx < NUMBER_OF_SCORES_KEPT; scoreIdx ++)
    {
        const int memoryIdx = SCORE_MEMORY_ADDRESS + scoreIdx + SCORE_SIZE_IN_MEMORY;
        EEPROM.get(memoryIdx, p_winnerScores[scoreIdx]);
    }
    for(int nameIdx = 0; nameIdx < NUMBER_OF_SCORES_KEPT; nameIdx ++)
    {
        for(int charIdx = 0; charIdx < LETTERS_IN_NAME; charIdx ++)
        {
            const int memoryIdx = SCORE_NAMES_ADDRESS + nameIdx * SIZE_OF_NAME_IN_EEPROM + charIdx;
            EEPROM.get(memoryIdx, p_namesOfWinners[nameIdx][charIdx]);
        }
    }
}

void gameMenu::displayWinnersOnce(char p_namesOfWinners[NUMBER_OF_SCORES_KEPT][SIZE_OF_NAME_IN_EEPROM], unsigned long p_winnerScores[NUMBER_OF_SCORES_KEPT], const byte p_offset = 0)
{
    if(m_changedState)
    {
        for(int placeIdx = 0 + p_offset; placeIdx < SCORES_TO_SHOW_AT_A_TIME + p_offset; placeIdx ++)
        {   
            m_lcd.print(placeIdx + 1);
            m_lcd.print(". ");
            for(int charIdx = 0; charIdx < LETTERS_IN_NAME; charIdx ++)
            {
                m_lcd.print(p_namesOfWinners[placeIdx][charIdx]);
            }
            m_lcd.print(": ");
            m_lcd.print(p_winnerScores[placeIdx]);
            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
        }

        m_changedState = false;
    }
}


void gameMenu::displayHowToPlay()
{
    g_map.displayIcon(ICON_ABOUT);

    if(!m_showHowToPlay)
    {
        m_lcd.write(ARROW_RIGHT_CHAR);
        m_lcd.print(F(" how to play? "));
        m_lcd.write(ARROW_LEFT_CHAR);

        m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
        m_lcd.print(F("   learn more"));

        m_changedState = false;
    }
    else
    {                //____----____----____----____----____----
        m_lcd.print(F("use js to move and blue button to shoot"));
        m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_COL);
        m_lcd.print(F("shoot the enemies, walls and don't die!"));
        m_changedState = false;
    }
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
        goToNextMenuOption(m_state, LOWEST_MENU_STATE, HIGHEST_MENU_STATE);
    }

    switch (m_state)
    {
    case MENU_IN_START_GAME:
        if(m_changedState)
        {
            g_map.displayIcon(ICON_START);
            
            m_lcd.print(F(" "));
            m_lcd.write(ARROW_RIGHT_CHAR);
            m_lcd.print(F(" start game "));
            m_lcd.write(ARROW_LEFT_CHAR);

            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
            m_lcd.print(F("  high  scores"));

            m_changedState = false;
        }

        if(m_hwCtrl.pressedButton())
        {
            changeState(m_state, MENU_IN_GAME);   
        }
        break;

    case MENU_IN_HIGH_SCORES:
        char namesOfWinners[NUMBER_OF_SCORES_KEPT][SIZE_OF_NAME_IN_EEPROM];
        unsigned long winnerScores[NUMBER_OF_SCORES_KEPT];

        if(!m_inSubmenu)
        {
            if(m_changedState)
            {   
                g_map.displayIcon(ICON_TROPHY);
                
                m_lcd.write(ARROW_RIGHT_CHAR);
                m_lcd.print(F(" high  scores "));
                m_lcd.write(ARROW_LEFT_CHAR);

                m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
                m_lcd.print(F("    settings"));

                m_changedState = false;

                getWinnersAndScores(namesOfWinners, winnerScores);
            }
            if(m_hwCtrl.pressedButton())
            {
                m_inSubmenu = true;
                m_changedState = true;
                m_lcd.clear();
            }
        }
        else
        {
            if(m_showFirstScores)
            {
                displayWinnersOnce(namesOfWinners, winnerScores);
                
                if(m_hwCtrl.joystickDown())
                {
                    m_showFirstScores = !m_showFirstScores;
                    m_changedState = true;
                    m_lcd.clear();
                }
            }
            else
            {   
                const byte offset = 1;
                displayWinnersOnce(namesOfWinners, winnerScores, offset);
                
                if(m_hwCtrl.joystickUp())
                {
                    m_showFirstScores = !m_showFirstScores;
                    m_changedState = true;
                    m_lcd.clear();
                }
            }

            if(m_hwCtrl.pressedBackButton())
            {
                m_changedState = true;
                m_lcd.clear();
                m_inSubmenu = false;
            }
        }
        

        break;

    case MENU_IN_SETTINGS:
        if(m_settingsState == RETURN_FROM_SETTINGS)
        {
            if(m_changedState)
            {
                g_map.displayIcon(ICON_HAMMER);

                m_lcd.print(F("  "));
                m_lcd.write(ARROW_RIGHT_CHAR);
                m_lcd.print(F(" settings "));
                m_lcd.write(ARROW_LEFT_CHAR);

                m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
                m_lcd.print(F("  how to play?"));

                m_changedState = false;
                m_inSubmenu = false;
            }

            if(m_hwCtrl.pressedButton())
            {
                m_inSubmenu = true;
                changeState(m_settingsState, m_previousSettingsState);
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
            displayAboutMenu();
        }

        doScrollMenuLogic(m_showAboutText);

        break; 

    case MENU_IN_HOW_TO_PLAY:
        if(m_changedState)
        {
            displayHowToPlay();
        }
        
        doScrollMenuLogic(m_showHowToPlay);
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