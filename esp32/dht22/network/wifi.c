/**
     *  from https://github.com/espressif/esp-idf/blob/master/examples/wifi/getting_started/station/main/station_example_main.c
     */

#include "nvs_flash.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "string.h"

#include "freertos/event_groups.h"

static const char *TAG = "wifi station";

static int s_retry_num = 0;

static bool nvs_initialized = false;

static bool connected = false;

static EventGroupHandle_t s_wifi_event_group;
#define EXAMPLE_ESP_MAXIMUM_RETRY 5

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static uint16_t g_scan_ap_num;
static wifi_ap_record_t *g_ap_list_buffer;

void initialise_nvs()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    nvs_initialized = true;
}

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "connect to the AP fail");
        connected = false;
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        connected = true;
    }
}

void connect_wifi(const char *ssid, const char *wifi_pw)
{
    if (!nvs_initialized)
    {
        initialise_nvs();
    }

    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        }};
    memcpy(wifi_config.sta.ssid, ssid, strlen(ssid) * sizeof(char));
    memcpy(wifi_config.sta.password, wifi_pw, strlen(wifi_pw) * sizeof(char));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

int scan_wifi_ap(const char *ssid)
{
    if (connected == false)
    {
        ESP_LOGI(TAG, "Not connected, skipping scan");
        return false;
    }

    wifi_scan_config_t scan_config = {0};
    scan_config.ssid = (uint8_t *)ssid;
    uint8_t i;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));

    esp_wifi_scan_get_ap_num(&g_scan_ap_num);
    if (g_scan_ap_num == 0)
    {
        ESP_LOGI(TAG, "No matching AP found");
        return false;
    }

    g_ap_list_buffer = malloc(g_scan_ap_num * sizeof(wifi_ap_record_t));

    if (g_ap_list_buffer == NULL)
    {
        ESP_LOGE(TAG, "Failed to malloc buffer to print scan results");
        return false;
    }
    int bestRSSI = -9999999;
    if (esp_wifi_scan_get_ap_records(&g_scan_ap_num, (wifi_ap_record_t *)g_ap_list_buffer) == ESP_OK)
    {

        for (i = 0; i < g_scan_ap_num; i++)
        {
            if(g_ap_list_buffer[i].bssid > bestRSSI) {
                bestRSSI = g_ap_list_buffer[i].bssid;
            }
            ESP_LOGI(TAG, "[%d][%s][rssi=%d][bssid=%d][channel=%d]"
                          "%s", i, 
                     g_ap_list_buffer[i].ssid,
                     g_ap_list_buffer[i].rssi,
                     g_ap_list_buffer[i].bssid,
                     g_ap_list_buffer[i].primary,
                     g_ap_list_buffer[i].ftm_responder ? "[FTM Responder]" : "");
        }
    }

    ESP_LOGI(TAG, "sta scan done");
    free(g_ap_list_buffer);

    return bestRSSI;
}