/***********************************************************************************************
* FILENAME       : touch_button.c
 * VERSION        : 0.0.1
 * DESCRIPTION    :
 *
 * PUBLIC FUNCTIONS:
 *    - func()
 *
 * NOTES:
 *    Prefixes denote variable or return types:
 *    - "c" for char, "s" for short, "v" for void, "u" for unsigned, and "x" for non-standard types.
 *
 * AUTHOR         : Tjark Ziehm
 * START DATE     : October 1, 2024
 * COPYRIGHT      : © 2024 Tjark Ziehm
 * LICENCE        : CC BY-NC-ND 4.0
 * VERSION FORMAT : <major>.<minor>.<patch>
 *
 * Contributors   :
 *
 * CHANGE LOG:
 *    - v0.0.1:
 *    - v0.0.2:
 *    - v0.1.0:
 *    - v1.0.0:
 ***********************************************************************************************/

#include <stdio.h>
#include "esp_log.h"
#include "driver/touch_pad.h"
#include "touch_button.h"
#include "sdkconfig.h"

#define TOUCH_PIN_NUM 13 // Replace with your touch pin number
#define TOUCH_THRESHOLD 1000 // Adjust threshold as needed

void touch_button_init(void){

}

void touch_button_read(void){

}