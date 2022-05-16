#include "remote_log.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_timer.h"

static uint8_t active_logs = 0;
remote_log_t remote_logs[MAX_LOGS];

const char *RMLOG_TAG = "Remote Log";


static void remote_logs_cb(void* arg)
{   
    ESP_LOGI(RMLOG_TAG,"----- remote logging -----");
    // Call all registered callbacks here
    for(int i=0;i<active_logs;i++) {
        int64_t esp_timer_get_time();
        remote_logs[i].cb();
        ESP_LOGI(RMLOG_TAG,"logged");
    }
    // Send data to wifi stack maybe
}


esp_err_t remote_log_init(uint32_t log_frequency_ms)
{
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

esp_err_t remote_log_register(remote_log_t log)
{
    if(!(active_logs < MAX_LOGS)) {
        ESP_LOGE(RMLOG_TAG,"Too many logs");
        return ESP_FAIL;
    }
    active_logs++;
    remote_logs[active_logs] = log;

    return ESP_OK;
}

