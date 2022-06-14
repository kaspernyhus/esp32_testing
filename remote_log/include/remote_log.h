#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "esp_err.h"


#define MAX_LOGS 10
#define MAX_DATA_SIZE 100

// typedef void *(*remote_cbs_t)(void);

typedef enum {
  REMOTE_LOG_UDP,
  REMOTE_LOG_UART,
  invalid
} remote_log_transport_type;

typedef struct {
  remote_log_transport_type transport_type;
  // for UDP
  char *ip;
  uint32_t port;
  // for UART
  uint8_t uart_num;
  uint32_t baud_rate;
} remote_log_config;

typedef struct {
  uint8_t log_id;
  uint8_t tag_len;
  char *tag;
} remote_log_id_t;

typedef struct {
  uint16_t total_len;
  remote_log_id_t log_id;
  uint32_t timestamp;
  uint8_t log_data_len;
  uint8_t *log_data;
} remote_log_t;

typedef struct {
  remote_log_id_t id;
  esp_err_t (*data_log_cb)(void *out_data, size_t *len);
} remote_log_register_t;


esp_err_t remote_log_init(uint32_t log_frequency_ms, remote_log_config *cfg);
esp_err_t remote_log_register(remote_log_register_t log);
esp_err_t remote_log_send(remote_log_t *log);
