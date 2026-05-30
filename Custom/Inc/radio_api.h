#ifndef __RADIO_API__
#define __RADIO_API__

#include <stdint.h>

typedef struct {
    void (* requestIT)(void);
} radioAPI_t;

typedef struct {
    void (* test)(void);
} moduleInterface_t;

typedef struct {
    uint8_t mem[1024];
} interfaceMemory_t;

extern const radioAPI_t radioAPI;

#endif