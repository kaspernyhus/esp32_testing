#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "esp_err.h"


#define MAX_LOGS 10

// typedef void *(*remote_cbs_t)(void);

typedef struct
{
  uint16_t payload_id;
  char *name;
} payload_id_t;

typedef struct
{
  int64_t timestamp;
  uint32_t seq;
  payload_id_t payload_id;
  uint8_t payload_length;
} remote_log_header_t;

typedef struct {
  payload_id_t id;
  void (*cb)(void);
} remote_log_t;


esp_err_t remote_log_init(uint32_t log_frequency_ms);
esp_err_t remote_log_register(remote_log_t log);

