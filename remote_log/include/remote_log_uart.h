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

#define ECHO_TEST_TXD (4)
#define ECHO_TEST_RXD (5)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

esp_err_t configure_uart(uint8_t uart_num, uint32_t baud_rate);
esp_err_t remote_uart_send(const void *payload, size_t len);