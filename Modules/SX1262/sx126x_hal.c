#include "./sx126x_driver/sx126x_hal.h"
#include "./sx126x_driver/sx126x.h"
#include "spi.h"
#include "stm32h7xx_hal_gpio.h"

static void sx126x_wait_on_busy(void)
{
    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET)
    {
        HAL_Delay(1);
    }
}

sx126x_hal_status_t sx126x_hal_write(const void* context, const uint8_t* command, const uint16_t command_length, const uint8_t* data, const uint16_t data_length)
{
    sx126x_wait_on_busy();

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);

    if (HAL_SPI_Transmit(&hspi1, (uint8_t*)command, command_length, 1000) != HAL_OK) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
        return SX126X_HAL_STATUS_ERROR;
    }

    if (data_length > 0 && data != NULL) {
        if (HAL_SPI_Transmit(&hspi1, (uint8_t*)data, data_length, 1000) != HAL_OK) {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
            return SX126X_HAL_STATUS_ERROR;
        }
    }

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

    return SX126X_HAL_STATUS_OK;
}

sx126x_hal_status_t sx126x_hal_read(const void* context, const uint8_t* command, const uint16_t command_length, uint8_t* data, const uint16_t data_length)
{
    sx126x_wait_on_busy();

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);

    if (HAL_SPI_Transmit(&hspi1, (uint8_t*)command, command_length, 1000) != HAL_OK) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
        return SX126X_HAL_STATUS_ERROR;
    }

    if (data_length > 0 && data != NULL) {
        if (HAL_SPI_Receive(&hspi1, data, data_length, 1000) != HAL_OK) {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
            return SX126X_HAL_STATUS_ERROR;
        }
    }

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

    return SX126X_HAL_STATUS_OK;
}

sx126x_hal_status_t sx126x_hal_reset(const void* context)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
    
    HAL_Delay(10); 
    return SX126X_HAL_STATUS_OK;
}

sx126x_hal_status_t sx126x_hal_wakeup(const void* context)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
    
    // Semtech spec requires sending a dummy byte (or waiting) while NSS is low to wake it up
    uint8_t dummy = 0x00;
    HAL_SPI_Transmit(&hspi1, &dummy, 1, 1000);
    
    // Bring NSS back high
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
    
    // Wait for the chip to process the wake up and drop the BUSY line
    sx126x_wait_on_busy();

    return SX126X_HAL_STATUS_OK;
}