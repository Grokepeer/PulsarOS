#ifndef __UI_H__
#define __UI_H__

#include "cmsis_os2.h"

void DIPsSettingsReading(void);
void LEDsEdit(uint8_t LEDNumber, uint8_t patternNumber);
void LEDsTask(void *argument);

#endif