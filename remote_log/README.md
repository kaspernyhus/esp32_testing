## Remote Logging

# Setup and use
```
esp_err_t log_heap(void *out, size_t *len)
{
    size_t data = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    memcpy(out,&data,sizeof(data));
    *len = sizeof(data);
    
    return ESP_OK;
}


void app_main(void)
{
    remote_log_config cfg = {
        .transport_type = REMOTE_LOG_UART,
        .uart_num = 0,
        .baud_rate = 115200
    };

    remote_log_init(100, &cfg);

    remote_log_id_t log_id = {
        .log_id = 0x01,
        .tag = "test log via uart",
        .tag_len = 18
    };

    remote_log_register_t log_reg = {
        .id = log_id,
        .data_log_cb = log_heap
    };
    
    remote_log_register(log_reg);

}
```

