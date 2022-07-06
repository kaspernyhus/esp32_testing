#include <string.h>
#include "remote_log_udp.h"
#include "esp_log.h"


static const char *TAG = "Remote Log, udp";

static int sock = 0;
static struct sockaddr_in dest_addr;
char host_ip[13] = "";
uint32_t host_port = 0;


void remote_log_set_udp_ip_port(char *ip, uint32_t port)
{
    memcpy(host_ip,ip,13);
    host_port = port;
}

void remote_log_get_udp_ip_port(void)
{
    ESP_LOGI(TAG,"IP: %s : %d", host_ip, host_port);
}

esp_err_t remote_log_udp_write(const void *payload, size_t len) {
    int err = sendto(sock, payload, len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        return ESP_FAIL;
    }
    return ESP_OK;
}

void remote_log_udp_client_task(void *pvParameters)
{
    int addr_family = 0;
    int ip_protocol = 0;

    while (1) {
        // struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(host_ip);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(host_port);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        int ttl = 64;
        setsockopt(sock, ip_protocol, IP_TTL, &ttl, sizeof(ttl));

    
        ESP_LOGI(TAG, "Socket created (%d), sending to %s:%d", sock, host_ip, host_port);


        while (1) {
            if(ulTaskNotifyTake(pdTRUE,portMAX_DELAY)) {
                ESP_LOGE(TAG,"Error occurred in udp client task");
                break;
            }
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}