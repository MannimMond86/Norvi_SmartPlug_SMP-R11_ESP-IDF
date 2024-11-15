#ifndef TOUCH_BUTTON_H
#define TOUCH_BUTTON_H

#include "driver/touch_pad.h"

#ifdef __cplusplus
extern "C" {
#endif

void touch_button_init(void);
void touch_button_read(void);

#ifdef __cplusplus
}
#endif

#endif