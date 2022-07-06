# Remote Logging

## Data logging setup and use
### Setup
The module is initialized with a transport type: `REMOTE_LOG_UART / REMOTE_LOG_UDP`
<br />
log_frequency_ms is the interval the callbacks will be called with and also transported to the python app.

```
remote_log_config cfg = {
        .transport_type = REMOTE_LOG_UART,
        .uart_num = 1,
        .baud_rate = 9600,
        .log_frequency_ms = 100
    };
    remote_log_init(&cfg);
```

### Use
A call back with prototype: `esp_err_t cb_func(void *out, size_t *len)` must be supplied by the user. <br />
To register a log, user fills the `remote_log_register_t` <br />
log id must be unique and corresponds to the logging window in the python app <br />
tag is a string<br />
data_log_cb is the callback that produces logging data<br />
call_interval_multiplier can be used to slow down logging. E.g. log_frequency_ms = 100, call_interval_multiplier = 10 --> data is produced and transported at a rate of 1000ms.

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
        .data_log_cb = log_heap,
        .call_interval_multiplier = 10
    };
    remote_log_register(new_log);
}
```

### Event logging setup and use
Records events and visualizes them on the python plot.
NOTE: if log_id is the same as event_id e.g. 0x01, the event will be shown in the same window as the other graph.
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

