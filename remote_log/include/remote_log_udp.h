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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

// #define UDP_HOST_IP_ADDR "192.168.0.53"
// #define UDP_PORT 3333


typedef struct {
    uint8_t *tx_buffer;
    size_t length;
} udp_data_t;


esp_err_t udp_write(const void *payload, size_t len);
void udp_client_task(void *pvParameters);
void set_udp_ip_port(char *ip, uint32_t port);
void get_udp_ip_port(void);