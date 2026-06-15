#include "main.h"
#include "radio_api.h"
#include "stm32h7xx_hal_gpio.h"
#include "sx1262_interface.h"
#include "./sx126x_driver/sx126x.h"
#include "./sx126x_driver/sx126x_hal.h"
#include <stdint.h>

extern const radioAPI_t radioAPI;

typedef struct {
    uint8_t modNumber;
} modContext;

// Functions ----------------------------------------------------------------------

static void init(interfaceMemory_t *memory, connectorPins_t const *connectorPins)
{
    sx126x_hal_reset(NULL);
    sx126x_set_reg_mode(NULL, SX126X_REG_MODE_LDO);
    sx126x_set_standby(NULL, SX126X_STANDBY_CFG_RC);

    sx126x_set_dio3_as_tcxo_ctrl(NULL, SX126X_TCXO_CTRL_1_7V, 320);
    sx126x_cal(NULL, 0x7F);

    sx126x_set_pkt_type(NULL, SX126X_PKT_TYPE_LORA);
    sx126x_set_rf_freq(NULL, 868000000);

    sx126x_pa_cfg_params_t pa_cfg = {
        .pa_duty_cycle = 0x02,
        .hp_max        = 0x02,
        .device_sel    = 0x00,
        .pa_lut        = 0x01
    };
    sx126x_set_pa_cfg(NULL, &pa_cfg);

    sx126x_set_tx_params(NULL, 9, SX126X_RAMP_200_US);

    sx126x_mod_params_lora_t modParams = {
        .bw     = SX126X_LORA_BW_125,
        .sf     = SX126X_LORA_SF7,
        .cr     = SX126X_LORA_CR_4_5,
        .ldro   = 0x00
    };
    sx126x_set_lora_mod_params(NULL, &modParams);

    sx126x_pkt_params_lora_t pktParams = {
        .preamble_len_in_symb   = 8,
        .header_type            = SX126X_LORA_PKT_EXPLICIT,
        .pld_len_in_bytes       = 2,
        .crc_is_on              = true,
        .invert_iq_is_on        = false
    };
    sx126x_set_lora_pkt_params(NULL, &pktParams);

    sx126x_set_buffer_base_address(NULL, 0x00, 0x00);
}

void transmit(interfaceMemory_t *memory, connectorPins_t const *connectorPins)
{
    sx126x_set_dio_irq_params(NULL, SX126X_IRQ_TX_DONE | SX126X_IRQ_TIMEOUT, SX126X_IRQ_TX_DONE | SX126X_IRQ_TIMEOUT, SX126X_IRQ_NONE, SX126X_IRQ_NONE);

    uint8_t txBuffer[2] = {0xAB, 0xBC};
    uint16_t payloadLength = 2;
    sx126x_write_buffer(NULL, 0x00, txBuffer, payloadLength);

    HAL_GPIO_WritePin(connectorPins->pin[1].port, connectorPins->pin[1].pin, GPIO_PIN_SET);

    sx126x_clear_irq_status(NULL, SX126X_IRQ_ALL);

    sx126x_set_tx(NULL, 100);

    sx126x_irq_mask_t irq_status = SX126X_IRQ_NONE;
    while ((irq_status & SX126X_IRQ_TX_DONE) == 0)
    {
        sx126x_get_irq_status(NULL, &irq_status);
        
        if (irq_status & SX126X_IRQ_TIMEOUT) {
            break;
        }
    }

    sx126x_clear_irq_status(NULL, SX126X_IRQ_TX_DONE);

    HAL_GPIO_WritePin(connectorPins->pin[1].port, connectorPins->pin[1].pin, GPIO_PIN_RESET);
}

void receive(interfaceMemory_t *memory, connectorPins_t const *connectorPins)
{
    sx126x_set_dio_irq_params(NULL, SX126X_IRQ_ALL, SX126X_IRQ_ALL, SX126X_IRQ_NONE, SX126X_IRQ_NONE);

    uint8_t rx_payload_out[2];

    sx126x_clear_irq_status(NULL, SX126X_IRQ_ALL);

    sx126x_set_rx(NULL, 0x00);

    sx126x_irq_mask_t irq_status = SX126X_IRQ_NONE;
    while (1)
    {
        sx126x_get_irq_status(NULL, &irq_status);

        if (irq_status & SX126X_IRQ_RX_DONE) 
        {
            break; 
        }

        if (irq_status & (SX126X_IRQ_TIMEOUT | SX126X_IRQ_CRC_ERROR)) 
        {
            sx126x_clear_irq_status(NULL, SX126X_IRQ_ALL);
            return;
        }
        
        osDelay(10);
    }

    // Fetch the size and memory location of the received packet
    sx126x_rx_buffer_status_t rx_buffer_status;
    sx126x_get_rx_buffer_status(NULL, &rx_buffer_status);

    if (rx_buffer_status.pld_len_in_bytes == 2) 
    {
        sx126x_read_buffer(NULL, rx_buffer_status.buffer_start_pointer, rx_payload_out, 2);
        
        sx126x_clear_irq_status(NULL, SX126X_IRQ_ALL);
        return;
    }

    sx126x_clear_irq_status(NULL, SX126X_IRQ_ALL);
    return;
}

// Module interface assignments ---------------------------------------------------

const moduleInterface_t sx1262ModuleInterface = {
    .init = init,
    .transmit = transmit,
    .receive = receive
};