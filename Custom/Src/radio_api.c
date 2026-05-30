#include "radio_api.h"
#include "main.h"
#include "stm32h7xx_hal_gpio.h"

// Functions ----------------------------------------------------------------------

static void requestIT(void)
{
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}

// Radio API assigments -----------------------------------------------------------

const radioAPI_t radioAPI = {
    .requestIT = requestIT
};