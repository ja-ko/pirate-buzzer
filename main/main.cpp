#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_err.h"

#include "events.h"
#include "BluetoothManager.h"
#include "LEDManager.h"

#include "BLEDevice.h"
#include "BLE2902.h"
#include "GPIO.h"

#define TRIGGER_PIN GPIO_NUM_27

#define LOG_TAG "Buzzer-Main"


extern "C" {
  void app_main();
}

using namespace ESP32CPP;

static bool current_trigger_state = false;
static xQueueHandle gpio_event_queue = NULL;

ESP_EVENT_DEFINE_BASE(BUZZER_EVENT);

class SecurityCallbackHandler: public BLESecurityCallbacks {
  uint32_t onPassKeyRequest() {
    ESP_LOGI(LOG_TAG, "OnPassKeyRequest");
    return 123456;
  }

  void onPassKeyNotify(uint32_t pass_key) {
    ESP_LOGI(LOG_TAG, "OnPassKeyNotify with passkey: %d", pass_key);
  }

  bool onSecurityRequest() {
    ESP_LOGI(LOG_TAG, "OnSecurityRequest");
    return true;
  }

  void onAuthenticationComplete(esp_ble_auth_cmpl_t) {
    ESP_LOGI(LOG_TAG, "OnAuthenticationComplete ");
  }

  bool onConfirmPIN(uint32_t pin) {
    ESP_LOGI(LOG_TAG, "PinConfirm for pin: %d", pin);
    return true;
  }

};

void init_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
}

void handle_trigger_events(void* args) {
  bool current_state;
  bool isr_state;
  while(true) {
    if(xQueueReceive(gpio_event_queue, &isr_state, portMAX_DELAY)) {
      ESP_LOGD(LOG_TAG, "TRIGGER INTERRUPT");
      current_state = !GPIO::read(TRIGGER_PIN);
      if(current_trigger_state != current_state) {
        if(current_state) {
          ESP_ERROR_CHECK(esp_event_post(BUZZER_EVENT, TRIGGER_DOWN_EVENT, nullptr, 0, 10 / portTICK_PERIOD_MS ));
        } else {
          ESP_ERROR_CHECK(esp_event_post(BUZZER_EVENT, TRIGGER_UP_EVENT, nullptr, 0, 10 / portTICK_PERIOD_MS ));
        }
        current_trigger_state = current_state;
      }
    }
  }
}

void trigger_isr_handler(void* args) {
  xQueueSendFromISR(gpio_event_queue, &current_trigger_state, NULL);
}

void setup_gpio() {
  gpio_event_queue = xQueueCreate(8, sizeof(bool));

  GPIO::setInput(TRIGGER_PIN);
  gpio_pullup_en(TRIGGER_PIN);
  GPIO::setInterruptType(TRIGGER_PIN, GPIO_INTR_ANYEDGE);
  GPIO::addISRHandler(TRIGGER_PIN, trigger_isr_handler, nullptr);
  current_trigger_state = !GPIO::read(TRIGGER_PIN);
}


void app_main()
{
  ESP_LOGI(LOG_TAG, "This is PirateBuzzer, starting up!");

  init_nvs();
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  setup_gpio();
  xTaskCreate(handle_trigger_events, "handle_trigger_events", 2048, NULL, 10, NULL);

  BluetoothManager* blm = new BluetoothManager();
  ledmanager_setup();
}

