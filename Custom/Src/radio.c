#include "radio.h"
#include <stdint.h>
#include <string.h>
#include "cmsis_os2.h"

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
    moduleInterface_t* interface;
    interfaceMemory_t interfaceMemory;
} modControl_t;

typedef struct {
    uint8_t interfaceTypeCode;
    moduleInterface_t* PPIAddress;
} PPILocator_t;

typedef struct {
    modControl_t* modControl;
} radioControl_t;

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
    uint8_t PPITableSize = 1;
    PPILocator_t PPITable[PPITableSize];

    // This will be changed with the code that looks for interfaces installed in memory and loads the .ppi addresses of all the available interfaces
    PPITable[0].interfaceTypeCode = 0x0A;
    PPITable[0].PPIAddress = &sx1262ModuleInterface;
    // ---------------------------------------------------------------------------------------------------------------------------------------------

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
    modControl_t modControl[4];
    radioControl_t radioControl;
    radioControl.modControl = &modControl[0];

    loadSettings(radioControl);

    loadPPI(radioControl);

    if (radioControl.modControl[0].isInterfaceLoaded)
    {
        radioControl.modControl[0].interface->test();
    }

    for(;;)
    {
        //
    }
}