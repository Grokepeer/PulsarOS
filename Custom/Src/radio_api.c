#include "radio_api.h"
#include "cmsis_os2.h"

// Functions ----------------------------------------------------------------------

// Radio API assigments -----------------------------------------------------------

const radioAPI_t radioAPI = {
    .delay = osDelay
};