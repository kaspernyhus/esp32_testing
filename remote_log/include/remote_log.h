#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "esp_err.h"


#define MAX_LOGS 10
#define MAX_DATA_SIZE 100
#define MAX_LOG_SIZE 150

#define ID_SEND_INTERVAL 100


typedef enum {
  REMOTE_LOG_UDP = 1,
  REMOTE_LOG_UART
} remote_log_transport_type;

typedef enum {
  REMOTE_LOG_ID,
  REMOTE_LOG_DATA
} remote_log_type;

typedef struct {
  remote_log_transport_type transport_type;
  uint32_t log_frequency_ms;
  // for UDP
  char *ip;
  uint32_t port;
  // for UART
  uint8_t uart_num;
  uint32_t baud_rate;
} remote_log_config;

typedef struct {
  remote_log_type log_type;
  uint8_t log_id;
  uint32_t timestamp;
  uint8_t log_data_len;
  uint8_t *log_data;
  uint16_t total_len;
} remote_log_t;

typedef struct {
  uint8_t log_id;
  char tag[20];
  esp_err_t (*data_log_cb)(void *out_data, size_t *len);
  uint16_t called_counter;
  uint16_t total_times_called;
} remote_log_register_t;


esp_err_t remote_log_init(remote_log_config *cfg);
esp_err_t remote_log_register(remote_log_register_t log);
