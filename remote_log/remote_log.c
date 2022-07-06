#include "remote_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_err.h"
#include "remote_log_udp.h"
#include "remote_log_uart.h"


static uint8_t active_logs = 0;
static uint8_t active_events = 0;
remote_log_register_t remote_logs[MAX_LOGS];
remote_log_event_register_t remote_events[MAX_LOGS];
TaskHandle_t remote_log_udp_task_handle = NULL;
QueueHandle_t event_q;
remote_log_transport_type log_transport = 0;

static const char *TAG = "Remote Log";


static void remote_logs_cb(void* arg);
static void call_registered_callbacks(void);
static void check_for_new_events(void);


esp_err_t remote_log_init(remote_log_config *cfg)
{
    if(cfg->transport_type == REMOTE_LOG_UDP) {
        log_transport = REMOTE_LOG_UDP;
        remote_log_set_udp_ip_port(cfg->ip, cfg->port);
        remote_log_get_udp_ip_port();

        // Start UDP tasks
        xTaskCreatePinnedToCore(remote_log_udp_client_task,"udp_client",4096,NULL,5,&remote_log_udp_task_handle,0);
    }

    else if(cfg->transport_type) {
        log_transport = REMOTE_LOG_UART;
        configure_uart(cfg->uart_num, cfg->baud_rate, cfg->tx_pin, cfg->rx_pin);
    }

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = remote_logs_cb,
            .name = "remote log timer"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, cfg->log_frequency_ms*1000));
    ESP_LOGI(TAG, "Periodic timer started: %d ms", cfg->log_frequency_ms);
    ESP_LOGI(TAG, "Remote logging started");
    
    // Create message queue for recording events. Stores an event ID and a timestamp
    event_q = xQueueCreate(EVENT_QUEUE_SZ, sizeof(remote_log_event_t));
    if(event_q == NULL) {
        ESP_LOGE(TAG, "Failed to initialize event queue");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Event queue created");

    return ESP_OK;
}



esp_err_t remote_log_send(remote_log_t *log)
{
    uint8_t log_packet[MAX_LOG_SIZE];

    int index = 0;
    log_packet[index++] = 0xa5;
    log_packet[index++] = log->total_len;
    log_packet[index++] = log->log_type;
    log_packet[index++] = log->log_id;
    log_packet[index++] = (log->timestamp) & 0xff;
    log_packet[index++] = (log->timestamp >> 8) & 0xff;
    log_packet[index++] = (log->timestamp >> 16) & 0xff;
    log_packet[index++] = (log->timestamp >> 24) & 0xff;
    log_packet[index++] = log->log_data_len;
    memcpy(log_packet+index,log->log_data,log->log_data_len);
    index += log->log_data_len;
    log_packet[index++] = 0x0a; // EOL
    log->total_len++;

    ESP_LOGD(TAG,"index: %d, log->total_len: %d", index, log->total_len);
    // ESP_LOG_BUFFER_HEX(TAG,log_packet,log->total_len);

    if (log_transport == REMOTE_LOG_UDP) {
        remote_log_udp_write(log_packet, log->total_len);
    }
    else if (log_transport == REMOTE_LOG_UART)
    {
        remote_uart_send(log_packet, log->total_len);
    }
    else {
        ESP_LOGE(TAG, "Transport not initialized!");
    }
    
    return ESP_OK;
}

static void remote_logs_cb(void* arg)
{
    if(active_logs == 0) {
        ESP_LOGD(TAG, "no logs registered");
    }
    if(active_events == 0) {
        ESP_LOGD(TAG, "no events registered");
    }

    call_registered_callbacks();
    check_for_new_events();
}

esp_err_t remote_log_register_log(remote_log_register_t log)
{
    if(log_transport == 0) {
        ESP_LOGE(TAG,"module not initialized!");
        return ESP_FAIL;
    }
    if(!(active_logs < MAX_LOGS)) {
        ESP_LOGE(TAG,"Can't register more logs - too many");
        return ESP_FAIL;
    }
    log.called_counter = 0; // reset call counter
    log.total_times_called = 0;
    if(log.call_interval_multiplier != 0) {
        log.call_interval_counter = log.call_interval_multiplier;
    }
    else {
        log.call_interval_counter = 0;
    }
    remote_logs[active_logs++] = log;
    
    remote_log_t initial_log = {
            .log_type = REMOTE_LOG_ID,
            .log_id = log.log_id,
            .timestamp = 0,
            .log_data_len = 20,
            .log_data = (uint8_t*)log.tag,
            .total_len = 9 + 20
        };
    remote_log_send(&initial_log);

    ESP_LOGI(TAG, "New log registered. Log ID%d: %s", log.log_id, log.tag);

    return ESP_OK;
}

esp_err_t remote_log_register_event(remote_log_event_register_t event)
{
    if(log_transport == 0) {
        ESP_LOGE(TAG,"module not initialized!");
        return ESP_FAIL;
    }
    if(!(active_events < MAX_LOGS)) {
        ESP_LOGE(TAG,"Can't register more events - too many");
        return ESP_FAIL;
    }

    event.called_counter = 0; // reset call counter
    event.total_times_called = 0;
    remote_events[event.event_id] = event;

    ESP_LOGI(TAG, "New event registered. Event ID%d: %s", event.event_id, event.tag);
    
    return ESP_OK;
}

esp_err_t remote_log_record_event(uint8_t event_id)
{
    uint32_t timestamp = (uint32_t)(esp_timer_get_time()/1000);
    remote_log_event_t event = {
        .event_id = event_id,
        .timestamp = timestamp
    };
    xQueueSend(event_q, (void *)&event, 0); // don't block

    return ESP_OK;
}

static void call_registered_callbacks(void)
{
    for(int i=0;i<active_logs;i++) {
        // Check call interval counter
        if( remote_logs[i].call_interval_counter != 0 ) {
            remote_logs[i].call_interval_counter--;
            continue;
        }

        ESP_LOGD(TAG,"creating log #%d, id: %d",i,remote_logs[i].log_id);

        uint8_t logging_data[MAX_DATA_SIZE];
        size_t logging_data_len;
        uint32_t timestamp = (uint32_t)(esp_timer_get_time()/1000);
        esp_err_t err = remote_logs[i].data_log_cb(logging_data, &logging_data_len);
        if(err != ESP_OK) {
            ESP_LOGE(TAG, "Data logging callback error. Log ID: %i", remote_logs[i].log_id);
        }
        size_t total_len = 9 + logging_data_len;

        remote_log_t new_log = {
            .log_type = REMOTE_LOG_DATA,
            .log_id = remote_logs[i].log_id,
            .timestamp = timestamp,
            .log_data_len = logging_data_len,
            .log_data = logging_data,
            .total_len = total_len,
        };
        remote_log_send(&new_log);

        remote_logs[i].total_times_called++;
        remote_logs[i].call_interval_counter = remote_logs[i].call_interval_multiplier;  // reset interval counter

        // if time, resend ID packet
        if(remote_logs[i].called_counter++ > ID_SEND_INTERVAL) {
            remote_logs[i].called_counter = 0;
            remote_log_t id_log = {
                .log_type = REMOTE_LOG_ID,
                .log_id = remote_logs[i].log_id,
                .timestamp = timestamp,
                .log_data_len = 20,
                .log_data = (uint8_t*)remote_logs[i].tag,
                .total_len = 9 + 20
            };
            remote_log_send(&id_log);
        }
    }
}

static void check_for_new_events(void) {
    while(uxQueueMessagesWaiting(event_q)) {
        // int event_on_queue = uxQueueMessagesWaiting(event_q);
        // ESP_LOGI(TAG,"Events on queue: %d", event_on_queue);
        
        remote_log_event_t event;
        xQueueReceive(event_q, &event, 0); // don't block
        
        remote_log_t log = {
            .log_type = REMOTE_LOG_EVENT,
            .log_id = remote_events[event.event_id].event_id,
            .timestamp = event.timestamp,
            .log_data_len = 20,
            .log_data = (uint8_t*)remote_events[event.event_id].tag,
            .total_len = 9 + 20
        };

        remote_log_send(&log);
        ESP_LOGI(TAG,"Event id %d send @%d", event.event_id, event.timestamp);
    }
}
