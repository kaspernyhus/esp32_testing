#include "remote_log.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "remote_log_udp.h"


static uint8_t active_logs = 0;
remote_log_register_t remote_logs[MAX_LOGS];
TaskHandle_t udp_task_handle = NULL;

const char *RMLOG_TAG = "Remote Log";


esp_err_t remote_log_send(remote_log_t *log)
{
    uint8_t udp_packet[MAX_DATA_SIZE+1];

    int index = 0;
    udp_packet[index++] = 0xA5;
    udp_packet[index++] = log->total_len;
    udp_packet[index++] = log->log_id.log_id;
    udp_packet[index++] = log->log_id.tag_len;
    memcpy(udp_packet+index,log->log_id.tag,log->log_id.tag_len);
    index += log->log_id.tag_len;
    udp_packet[index++] = (log->timestamp) & 0xff;
    udp_packet[index++] = (log->timestamp >> 8) & 0xff;
    udp_packet[index++] = (log->timestamp >> 16) & 0xff;
    udp_packet[index++] = (log->timestamp >> 24) & 0xff;
    udp_packet[index++] = log->data_len;
    memcpy(udp_packet+index,log->data,log->data_len);
    index += log->data_len;

    ESP_LOGD(RMLOG_TAG,"index: %d, log->total_len: %d", index, log->total_len);

    udp_write(udp_packet, log->total_len);

    return ESP_OK;
}

static void remote_logs_cb(void* arg)
{   
    // Call all registered callbacks here
    if(active_logs == 0) {
        ESP_LOGI(RMLOG_TAG,"No logs registered");
        return;
    }

    for(int i=0;i<active_logs;i++) {
        uint8_t data[MAX_DATA_SIZE];
        size_t data_len = 0;
        uint32_t timestamp = (uint32_t)(esp_timer_get_time()/1000);
        uint8_t err = remote_logs[i].cb(data, &data_len);
        if(err != 0) {
            ESP_LOGE(RMLOG_TAG,"Callback error");
        }

        ESP_LOGD(RMLOG_TAG,"logged");

        size_t total_len = 9 + remote_logs[i].id.tag_len + data_len;

        remote_log_t new_log = {
            .total_len = total_len,
            .log_id = remote_logs[i].id,
            .timestamp = timestamp,
            .data_len = data_len,
            .data = data
        };
        remote_log_send(&new_log);
    }
}


esp_err_t remote_log_init(uint32_t log_frequency_ms, char *log_ip, uint32_t log_port)
{
    set_udp_ip_port(log_ip, log_port);
    get_udp_ip_port();

    // Start UDP tasks
    xTaskCreatePinnedToCore(udp_client_task,"udp_client",4096,NULL,5,&udp_task_handle,0);

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = remote_logs_cb,
            .name = "remote log timer"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, log_frequency_ms*1000));
    ESP_LOGI(RMLOG_TAG, "Periodic timer started: %d ms", log_frequency_ms);
    ESP_LOGI(RMLOG_TAG,"Remote logging started");
    
    return ESP_OK;
}

esp_err_t remote_log_register(remote_log_register_t log)
{
    if(!(active_logs < MAX_LOGS)) {
        ESP_LOGE(RMLOG_TAG,"Too many logs");
        return ESP_FAIL;
    }
    remote_logs[active_logs++] = log;

    return ESP_OK;
}

