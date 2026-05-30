#include "main.h"
#include "radio_api.h"
#include "sx1262_interface.h"
#include "./sx126x_driver/sx126x.h"

extern const radioAPI_t radioAPI;

// Functions ----------------------------------------------------------------------

static void test(void)
{
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

    radioAPI.requestIT();
}

// Module interface assignments ---------------------------------------------------

const moduleInterface_t sx1262ModuleInterface = {
    .test = test
};