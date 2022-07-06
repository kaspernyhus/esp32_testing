/*
 *  B&O Classics Adaptor
 *
 *  UDP streming module
 *
 *  LAPJ/KANK
 *  2022
 * 
*/

#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"

// #define REMOTE_LOG_TXD (17)
// #define REMOTE_LOG_RXD (18)
#define REMOTE_LOG_RTS (UART_PIN_NO_CHANGE)
#define REMOTE_LOG_CTS (UART_PIN_NO_CHANGE)

esp_err_t configure_uart(uint8_t uart_num, uint32_t baud_rate, uint8_t tx_pin, uint8_t rx_pin);
esp_err_t remote_uart_send(const void *payload, size_t len);