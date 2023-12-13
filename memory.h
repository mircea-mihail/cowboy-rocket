#ifndef MEMORY_H
#define MEMORY_H
// in this file all the information related to the arduino memory is stored

// all memory will be stored in relation to the previous memory spot in order to prevent
// aliasing and using more memory than needed

// the start memory address is not zero in order to prevent early wear on the eeprom

// score related
#define SCORE_MEMORY_ADDRESS 900
#define NUMBER_OF_SCORES_KEPT 3 
#define ADDRESS_AFTER_LAST_SCORE (SCORE_MEMORY_ADDRESS + NUMBER_OF_SCORES_KEPT * sizeof(unsigned long))

// brightness and contrast settings all have a size of max 255 (are bytes)
// because they are dictated by the resolution of the arduino PWM
#define SIZE_OF_PWM sizeof(byte)

#define EEPROM_MATRIX_BRIGHTNESS_ADDRESS ADDRESS_AFTER_LAST_SCORE
#define EEPROM_LCD_BRIGHTNESS_ADDRESS (EEPROM_MATRIX_BRIGHTNESS_ADDRESS + SIZE_OF_PWM)
#define EEPROM_LCD_CONTRAST_ADDRESS (EEPROM_LCD_BRIGHTNESS_ADDRESS + SIZE_OF_PWM)



/// visual map:
/*               addr (in EEPROM)
---------------- 1024 (Max size)
...
---------------- 915
brightness
and contrast
settings
---------------- 912 
scores kept
(ulong)
---------------- 900 (score mem addr)
*/

#endif