## Remote Logging

### Data logging setup and use
```
esp_err_t log_heap(void *out, size_t *len)
{
    size_t data = esp_random();

    // boilerplate   
    memcpy(out,&data,sizeof(data));
    *len = sizeof(data);
    return ESP_OK;
}


void app_main(void)
{
    remote_log_config cfg = {
        .transport_type = REMOTE_LOG_UART,
        .uart_num = 1,
        .baud_rate = 9600,
        .log_frequency_ms = 100
    };
    remote_log_init(&cfg);

    remote_log_register_t new_log = {
        .log_id = 0x01,
        .tag = "Heap tracking",
        .tag_len = 14,
        .data_log_cb = log_heap
    };
    remote_log_register(new_log);
}
```

### Event logging setup and use
```
void app_main(void)
{
    remote_log_config cfg = {
        .transport_type = REMOTE_LOG_UART,
        .uart_num = 1,
        .baud_rate = 9600,
        .log_frequency_ms = 100
    };
    remote_log_init(&cfg);

    remote_log_event_register_t new_event = {
        .event_id = 0x01,
        .tag = "Buffer overflow"
    };
    remote_log_register_event(new_event);
}

...
if(buffer_overflow) {
    remote_log_record_event(0x01);
}
```

