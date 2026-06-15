#include "radio.h"
#include "radio_api.h"
#include <stdint.h>
#include <string.h>
#include "cmsis_os2.h"
#include "main.h"

// Import interfaces headers ------------------------------------------------------

#include "sx1262_interface.h"
extern const moduleInterface_t sx1262ModuleInterface;

// Private define -----------------------------------------------------------------

// Private typedef ----------------------------------------------------------------

typedef struct {
    uint8_t isInstalled             : 1;
    uint8_t interfaceTypeCode       : 5;
    uint8_t isInterfaceLoaded       : 1;
    uint8_t                         : 0;
    moduleInterface_t *interface;
    interfaceMemory_t interfaceMemory;
} modControl_t;

typedef struct {
    modControl_t *modControl;
} radioControl_t;

typedef struct {
    uint8_t const interfaceTypeCode;
    moduleInterface_t const *PPIAddress;
} PPILocator_t;

// Functions ----------------------------------------------------------------------

// Load settings from the external memory
void loadSettings(radioControl_t radioControl)
{
    radioControl.modControl[0].isInstalled = 1;
    radioControl.modControl[0].interfaceTypeCode = 0x0A;
    
    radioControl.modControl[1].isInstalled = 0;
    radioControl.modControl[2].isInstalled = 0;
    radioControl.modControl[3].isInstalled = 0;
}

// Load Pulsar Package Interface, this function receives in input the installed modules (via settings) and relative requested interfaces and tries to load them, if they exist.
void loadPPI(radioControl_t radioControl)
{
    // Add here all installed interfaces with the relative address
    uint8_t const PPITableSize = 1;
    PPILocator_t const PPITable[PPITableSize] = { 
        [0] = {
            .interfaceTypeCode = 0x0A,
            .PPIAddress = &sx1262ModuleInterface
        }
    };

    // For each module, check if it's installed, load the relative interface, init interface memory...
    for (uint8_t modIndex = 0; modIndex < 4; modIndex++)
    {
        if (radioControl.modControl[modIndex].isInstalled)
        {
            // Loop through all installed interfaces (PPI) to get the one with the same interfaceTypeCode of the installed mod
            for (uint8_t PPITableSearchIndex = 0; PPITableSearchIndex < PPITableSize; PPITableSearchIndex++)
            {
                if (PPITable[PPITableSearchIndex].interfaceTypeCode == radioControl.modControl[modIndex].interfaceTypeCode)
                {
                    radioControl.modControl[modIndex].interface = PPITable[PPITableSearchIndex].PPIAddress;
                    memset(&radioControl.modControl[modIndex].interfaceMemory, 0, sizeof(radioControl.modControl[modIndex].interfaceMemory));
                    radioControl.modControl[modIndex].isInterfaceLoaded = 1;
                    break;
                }
            }

            if (!radioControl.modControl[modIndex].isInterfaceLoaded)
            {
                // Error, the requested interface for an installed module was not found
                radioControl.modControl[modIndex].isInterfaceLoaded = 0;
                radioControl.modControl[modIndex].interface = NULL;
            }
        }
        else
        {
            radioControl.modControl[modIndex].isInterfaceLoaded = 0;
            radioControl.modControl[modIndex].interface = NULL;
        }
    }
}

void RadioTask(void *argument)
{
    // This struct contains all the GPIO "mappings" for the MODx connectors
    connectorPins_t const connectorsPins[4] = {
        [0] = {
            .pin = {
                [0] = {MOD0IO0_GPIO_Port, MOD0IO0_Pin},
                [1] = {MOD0IO1_GPIO_Port, MOD0IO1_Pin},
                [2] = {MOD0IO2_GPIO_Port, MOD0IO2_Pin},
                [3] = {MOD0IO3_GPIO_Port, MOD0IO3_Pin},
                [4] = {MOD0IO4_GPIO_Port, MOD0IO4_Pin},
                [5] = {MOD0IO5_GPIO_Port, MOD0IO5_Pin},
                [6] = {MOD0IO6_GPIO_Port, MOD0IO6_Pin}
            }
        },
        [1] = {
            .pin = {
                [0] = {MOD1IO0_GPIO_Port, MOD1IO0_Pin},
                [1] = {MOD1IO1_GPIO_Port, MOD1IO1_Pin},
                [2] = {MOD1IO2_GPIO_Port, MOD1IO2_Pin},
                [3] = {MOD1IO3_GPIO_Port, MOD1IO3_Pin},
                [4] = {MOD1IO4_GPIO_Port, MOD1IO4_Pin},
                [5] = {MOD1IO5_GPIO_Port, MOD1IO5_Pin},
                [6] = {MOD1IO6_GPIO_Port, MOD1IO6_Pin}
            }
        },
        [2] = {
            .pin = {
                [0] = {MOD2IO0_GPIO_Port, MOD2IO0_Pin},
                [1] = {MOD2IO1_GPIO_Port, MOD2IO1_Pin},
                [2] = {MOD2IO2_GPIO_Port, MOD2IO2_Pin},
                [3] = {MOD2IO3_GPIO_Port, MOD2IO3_Pin},
                [4] = {MOD2IO4_GPIO_Port, MOD2IO4_Pin},
                [5] = {MOD2IO5_GPIO_Port, MOD2IO5_Pin},
                [6] = {MOD2IO6_GPIO_Port, MOD2IO6_Pin}
            }
        },
        [3] = {
            .pin = {
                [0] = {MOD3IO0_GPIO_Port, MOD3IO0_Pin},
                [1] = {MOD3IO1_GPIO_Port, MOD3IO1_Pin},
                [2] = {MOD3IO2_GPIO_Port, MOD3IO2_Pin},
                [3] = {MOD3IO3_GPIO_Port, MOD3IO3_Pin},
                [4] = {MOD3IO4_GPIO_Port, MOD3IO4_Pin},
                [5] = {MOD3IO5_GPIO_Port, MOD3IO5_Pin},
                [6] = {MOD3IO6_GPIO_Port, MOD3IO6_Pin}
            }
        }
    };

    radioControl_t radioControl;
    modControl_t modControl[4];
    radioControl.modControl = &modControl[0];

    loadSettings(radioControl);

    loadPPI(radioControl);

    osDelay(100);

    if (radioControl.modControl[0].isInterfaceLoaded)
    {
        radioControl.modControl[0].interface->init(&radioControl.modControl[0].interfaceMemory, &connectorsPins[0]);
    }

    for(;;)
    {
        osDelay(1000);
        if (radioControl.modControl[0].isInterfaceLoaded)
        {
            radioControl.modControl[0].interface->receive(&radioControl.modControl[0].interfaceMemory, &connectorsPins[0]);
        }
    }
}