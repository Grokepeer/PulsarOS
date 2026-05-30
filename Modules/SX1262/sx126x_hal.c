#include "./sx126x_driver/sx126x_hal.h"

sx126x_hal_status_t sx126x_hal_write(const void* context, const uint8_t* command, const uint16_t command_length, const uint8_t* data, const uint16_t data_length)
{
    return SX126X_HAL_STATUS_OK;
}

sx126x_hal_status_t sx126x_hal_read(const void* context, const uint8_t* command, const uint16_t command_length, uint8_t* data, const uint16_t data_length)
{
    return SX126X_HAL_STATUS_OK;
}

sx126x_hal_status_t sx126x_hal_reset(const void* context)
{
    return SX126X_HAL_STATUS_OK;
}

sx126x_hal_status_t sx126x_hal_wakeup(const void* context)
{
    return SX126X_HAL_STATUS_OK;
}