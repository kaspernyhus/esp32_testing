#include "remote_log.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_err.h"
#include "remote_log_udp.h"
#include "remote_log_uart.h"


static uint8_t active_logs = 0;
remote_log_register_t remote_logs[MAX_LOGS];
TaskHandle_t udp_task_handle = NULL;
remote_log_transport_type log_transport = 0;

const char *RMLOG_TAG = "Remote Log";


static void remote_logs_cb(void* arg);


esp_err_t remote_log_init(uint32_t log_frequency_ms, remote_log_config *cfg)
{

    if(cfg->transport_type == REMOTE_LOG_UDP) {
        log_transport = REMOTE_LOG_UDP;
        set_udp_ip_port(cfg->ip, cfg->port);
        get_udp_ip_port();

        // Start UDP tasks
        xTaskCreatePinnedToCore(udp_client_task,"udp_client",4096,NULL,5,&udp_task_handle,0);
    }

    else if(cfg->transport_type) {
        log_transport = REMOTE_LOG_UART;
        configure_uart(cfg->uart_num, cfg->baud_rate);
    }

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = remote_logs_cb,
            .name = "remote log timer"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, log_frequency_ms*1000));
    ESP_LOGI(RMLOG_TAG, "Periodic timer started: %d ms", log_frequency_ms);
    ESP_LOGI(RMLOG_TAG, "Remote logging started");
    
    return ESP_OK;
}



esp_err_t remote_log_send(remote_log_t *log)
{
    uint8_t log_packet[MAX_LOG_SIZE];

    int index = 0;
    log_packet[index++] = 0xA5;
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

    ESP_LOGD(RMLOG_TAG,"index: %d, log->total_len: %d", index, log->total_len);
    // ESP_LOG_BUFFER_HEX(RMLOG_TAG,log_packet,log->total_len);

    if (log_transport == REMOTE_LOG_UDP) {
        udp_write(log_packet, log->total_len);
    }
    else if (log_transport == REMOTE_LOG_UART)
    {
        remote_uart_send(log_packet, log->total_len);
    }
    else {
        ESP_LOGE(RMLOG_TAG, "Transport not initialized!");
    }
    
    return ESP_OK;
}

static void remote_logs_cb(void* arg)
{
    // Call all registered callbacks here
    if(active_logs == 0) {
        ESP_LOGI(RMLOG_TAG,"no logs registered");
        return;
    }

    for(int i=0;i<active_logs;i++) {
        ESP_LOGD(RMLOG_TAG,"creating log #%d",i);

        uint8_t logging_data[MAX_DATA_SIZE];
        size_t logging_data_len;
        uint32_t timestamp = (uint32_t)(esp_timer_get_time()/1000);
        esp_err_t err = remote_logs[i].data_log_cb(logging_data, &logging_data_len);
        if(err != ESP_OK) {
            ESP_LOGE(RMLOG_TAG, "Data logging callback error. Log ID: %i", remote_logs[i].log_id);
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

        if(remote_logs[i].called_counter++ > ID_SEND_INTERVAL) { // resend ID packet
            remote_logs[i].called_counter = 0;

            remote_log_t id_log = {
                .log_type = REMOTE_DATA_ID,
                .log_id = remote_logs[i].log_id,
                .timestamp = 0,
                .log_data_len = remote_logs[i].tag_len,
                .log_data = (uint8_t*)remote_logs[i].tag,
                .total_len = 9 + remote_logs[i].tag_len
            };
            remote_log_send(&id_log);
        }
    }
}

esp_err_t remote_log_register(remote_log_register_t log)
{
    if(log_transport == 0) {
        ESP_LOGE(RMLOG_TAG,"module not initialized!");
        return ESP_FAIL;
    }
    if(!(active_logs < MAX_LOGS)) {
        ESP_LOGE(RMLOG_TAG,"Can't register more logs - too many");
        return ESP_FAIL;
    }
    log.called_counter = 0; // reset call counter
    log.total_times_called = 0;
    remote_logs[active_logs++] = log;
    
    remote_log_t initial_log = {
            .log_type = REMOTE_DATA_ID,
            .log_id = log.log_id,
            .timestamp = 0,
            .log_data_len = log.tag_len,
            .log_data = (uint8_t*)log.tag,
            .total_len = 9 + log.tag_len
        };
    remote_log_send(&initial_log);

    return ESP_OK;
}

