#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "usart.h"

void BluetoothTask(void *argument);
void BluetoothUARTCallback(UART_HandleTypeDef *huart);
void BluetoothStatChange(void);

#endif