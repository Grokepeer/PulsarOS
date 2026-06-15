#ifndef __RADIO_API__
#define __RADIO_API__

#include "cmsis_os.h"
#include "stm32h733xx.h"
#include <stdint.h>

// Memory space made available to the each module interface to use
typedef struct {
    uint8_t mem[256];
} interfaceMemory_t;

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} gpioPortPin_t;

typedef struct {
    gpioPortPin_t pin[7];
} connectorPins_t;

// Functions that are accessed by the interface and implemented in the radio core
typedef struct {
    osStatus_t (* delay)(uint32_t);
} radioAPI_t;

// Functions that are accessed by the radio core and are implemented in the interface
typedef struct {
    void (* init)(interfaceMemory_t *memory, connectorPins_t const *connectorPins);
    void (* transmit)(interfaceMemory_t *memory, connectorPins_t const *connectorPins);
    void (* receive)(interfaceMemory_t *memory, connectorPins_t const *connectorPins);
} moduleInterface_t;

extern const radioAPI_t radioAPI;

#endif