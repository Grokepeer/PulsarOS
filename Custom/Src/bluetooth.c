#include "bluetooth.h"
#include "gpio.h"
#include "cmsis_os2.h"
#include "main.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_uart.h"
#include "usart.h"
#include <string.h>

// Private define -----------------------------------------------------------------

#define INIT            (uint8_t) 0
#define RESETTING       (uint8_t) 1
#define RUNNING         (uint8_t) 2
#define CONNECTED       (uint8_t) 6

// Global variables ---------------------------------------------------------------

uint8_t buffer[40];
uint8_t outBuffer[40];
uint8_t longBuffer[100];

uint8_t bleState = 0;

// Functions ----------------------------------------------------------------------

// This function reset the BLE module to factory then sets the UART to DMA, after the module has reset the BluetoothSet function will be called that will finish resetting the module to the Pulsar default settings
void BluetoothResetFactory(void)
{
    bleState = RESETTING;

    strcpy(outBuffer, "AT+DEFAULT\r\n");
    HAL_UART_Transmit(&huart4, (uint8_t*)outBuffer, strlen(outBuffer), 100);
    memset(buffer, 0, sizeof(buffer));
    HAL_UART_Receive(&huart4, buffer, 5, 100);  // Wait for +OK

    huart4.Instance->BRR = 417; // Set UART4 baud rate to the module's default 9600 Mbit/s for resetting

    // Hard reset the module
    HAL_GPIO_WritePin(BLE_RST_GPIO_Port, BLE_RST_Pin, RESET);
    osDelay(200);
    HAL_GPIO_WritePin(BLE_RST_GPIO_Port, BLE_RST_Pin, SET);

    // Wait for +READY
    HAL_UART_Receive_DMA(&huart4, buffer, 9);
}

// Following the BluetoothResetFactory this function sets everything that is necessary for the Pulsar to work
void BluetoothSet(void)
{
    strcpy(outBuffer, "AT+BAUD0\r\n");
    HAL_UART_Transmit(&huart4, (uint8_t *)outBuffer, strlen(outBuffer), 100);
    memset(buffer, 0, sizeof(buffer));
    HAL_UART_Receive(&huart4, buffer, 5, 100);  // Wait for +OK

    strcpy(outBuffer, "AT+NAMEPulsarBLE\r\n\0");
    HAL_UART_Transmit(&huart4, (uint8_t *)outBuffer, strlen(outBuffer), 100);

    huart4.Instance->BRR = 35;  // Set UART4 baud rate back to 115200 Mbit/s after resetting

    memset(buffer, 0, sizeof(buffer));
    HAL_UART_Receive(&huart4, buffer, 5, 100);  // Wait for +OK from the AT+NAME command

    strcpy(outBuffer, "AT+ALED1\r\n");
    HAL_UART_Transmit(&huart4, (uint8_t *)outBuffer, strlen(outBuffer), 100);
    memset(buffer, 0, sizeof(buffer));
    HAL_UART_Receive(&huart4, buffer, 5, 100);  // Wait for +OK

    bleState = RUNNING;
}

// Quickly checks that the BLE module is setup correctly, if not, calls the BluetoothResetFactory function
void BluetoothInit(void)
{
    bleState = INIT;
    osDelay(2000);  // Necessary to let the module start from cold start

    uint8_t dummy[200]; // Flush UART RX buffer that might have filled with startup "grabage"
    HAL_UART_Receive(&huart4, (uint8_t *)dummy, 200, 1);
    
    strcpy(outBuffer, "AT+DISC\r\n");
    HAL_UART_Transmit(&huart4, (uint8_t *)outBuffer, strlen(outBuffer), 100);
    memset(buffer, 0, sizeof(buffer));
    HAL_UART_Receive(&huart4, buffer, 18, 160);  // Wait for +OK and +DISCONNECTED

    // Checks that the BLE module has already be setup by the system by checking the name is not something different than PulsarBLE
    strcpy(outBuffer, "AT+NAME\r\n");
    HAL_UART_Transmit(&huart4, (uint8_t *)outBuffer, strlen(outBuffer), 100);
    memset(buffer, 0, sizeof(buffer));
    HAL_UART_Receive(&huart4, buffer, 40, 100);

    //uint32_t volatile tmp = huart4.Instance->BRR;
    // 417 BRR for 9600
    // 208 BRR for 19200
    // 35 BRR for 115200

    int8_t volatile cmp = strcmp(buffer, "+NAME:PulsarBLE\r\n");    // Without volatile it showed some weird behaviour
    if (cmp == 0)
    {
        bleState = RUNNING;
    }
    else
    {
        BluetoothResetFactory();   // If the name is different (or the baud rate is mistached generating framing errors in the rx reading), reset the module.
    }
}

// FreeRTOS task
void BluetoothTask(void *argument)
{
    BluetoothInit();

    for(;;)
    {
        osDelay(40);
    }
}

// Called by the UART Callback function if the UART interface is the UART4 dedicated to the BLE module
void BluetoothUARTCallback(UART_HandleTypeDef *huart)
{
    if (bleState == RESETTING)
    {   
        BluetoothSet(); // If the interrupt was called from the BluetoothResetFactory function, finish the module setup
    }
    else
    {
        HAL_UART_Transmit(&huart4, buffer, strlen(buffer), 100);
        memset(buffer, 0, sizeof(buffer));
        HAL_UART_Receive_DMA(&huart4, buffer, 4);
    }
}

// Called by the EXTI triggered at the rising/falling edge of the BLE STAT line (high -> connected, low -> disconnected) to update the bleState
void BluetoothStatChange(void)
{
    uint8_t volatile gpio = HAL_GPIO_ReadPin(BLE_STAT_GPIO_Port, BLE_STAT_Pin);
    if (bleState == RUNNING)
    {
        bleState = CONNECTED;

        uint8_t dummy[200]; //Flush UART RX buffer that might have filled with +CONNECTED
        HAL_UART_Receive(&huart4, (uint8_t *)dummy, 200, 100);

        memset(buffer, 0, sizeof(buffer));
        HAL_UART_Receive_DMA(&huart4, buffer, 4);
    }
    else if (bleState == CONNECTED) // When the module disconnects from the device
    {
        bleState = RUNNING;
        HAL_UART_DMAStop(&huart4);
    }
}