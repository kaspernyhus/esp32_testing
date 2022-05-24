#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "esp_err.h"


#define MAX_LOGS 10
#define MAX_DATA_SIZE 1000

// typedef void *(*remote_cbs_t)(void);

typedef struct {
  uint8_t log_id;
  uint8_t tag_len;
  char *tag;
} remote_log_id_t;

typedef struct {
  uint16_t total_len;
  remote_log_id_t log_id;
  uint32_t timestamp;
  uint8_t data_len;
  uint8_t *data;
} remote_log_t;

typedef struct {
  remote_log_id_t id;
  int8_t (*cb)(uint8_t *out, size_t *len);
} remote_log_register_t;


esp_err_t remote_log_init(uint32_t log_frequency_ms, char *log_ip, uint32_t log_port);
esp_err_t remote_log_register(remote_log_register_t log);
esp_err_t remote_log_send(remote_log_t *log);

