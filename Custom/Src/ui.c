#include "ui.h"
#include "gpio.h"
#include "cmsis_os2.h"

// Global variables ---------------------------------------------------------------

uint8_t LEDsPatterns[6] = {0, 0b10000000, 0, 0, 0, 0xFF};
uint8_t LEDSetting[3] = {0, 0, 1};

// Functions ----------------------------------------------------------------------

void DIPsSettingsReading(void)
{

}

void LEDsEdit(uint8_t LEDNumber, uint8_t patternNumber)
{
    LEDSetting[LEDNumber] = patternNumber;
}

void LEDsTask(void *argument)
{
    uint8_t counter = 1;
    for(;;)
    {
        //HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, LEDsPatterns[LEDSetting[0]] & counter);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, LEDsPatterns[LEDSetting[1]] & counter);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, LEDsPatterns[LEDSetting[2]] & counter);

        counter = counter << 1;

        if (counter == 0)   // Overflow case
        {
            counter = 1;
        }

        osDelay(200);
    }
}