#include "LEDManager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "events.h"

#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"

LEDManager* ledManager;

void led_manager_animate(void* arg) {
    while(true) {
        ledManager->animate_internal();
        vTaskDelay(40 / portTICK_PERIOD_MS);
    }
}

void ledmanager_setup() {
    LEDManager* manager = ledmanager_create();
    if(ledManager == NULL) {
        xTaskCreate(led_manager_animate, "led_manager_animate", 2048, NULL, 10, NULL);
    }
    ledManager = manager;
}

void ledmanager_event_handler(void* handler_arg, esp_event_base_t base, int id, void* data) {
    LEDManager* manager = static_cast<LEDManager*>(handler_arg);
    ESP_LOGD("Buzzer-LEDManager", "handling event with base %s and id %d", base, id);
    switch(id) {
        case TRIGGER_DOWN_EVENT:
            manager->onBuzzerDown();
            break;
        case TRIGGER_UP_EVENT:
            manager->onBuzzerUp();
            break;
        case CONNECTED:
            manager->onClientConnected();
            break;
        case DISCONNECTED:
            manager->onClientDisconnected();
            break;
        case REMOTE_EVENT:
            buzzer_remote_event_data_t* remote_event_data = (buzzer_remote_event_data_t*)data;
            manager->onExternalEvent(remote_event_data);
            break;
    }
}

LEDManager::LEDManager() {
    FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5,500);

    ESP_ERROR_CHECK(esp_event_handler_register(BUZZER_EVENT, ESP_EVENT_ANY_ID, ledmanager_event_handler, this));
}

void LEDManager::animate_internal() {
    int64_t current_time = esp_timer_get_time();
    animate((current_time - last_frame) / 1000);
    FastLED.show();
    last_frame = current_time;
}