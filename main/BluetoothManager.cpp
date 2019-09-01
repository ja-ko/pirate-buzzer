
#include "BluetoothManager.h"
#include "BLE2902.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "events.h"

#define BLM_LOG_TAG "Buzzer-BluetoothManager"

void blm_trigger_up_event_handler(void* handler_arg, esp_event_base_t base, int id, void* data) {
    ESP_LOGD(BLM_LOG_TAG, "trigger_up_event_handler called");
    BluetoothManager* blm = (BluetoothManager*)handler_arg;
    blm->triggerPressed();
}

BluetoothManager::BluetoothManager() {
    ESP_LOGI(BLM_LOG_TAG, "initializing BluetoothManager");
    BLEDevice::init("Pirate Buzzer");
    //BLEDevice::setSecurityCallbacks(new SecurityCallbackHandler());
    server_ = BLEDevice::createServer();
    server_->setCallbacks(this);
    service_ = server_->createService(kServiceUUID);

    trigger_ = new TriggerCharacteristic(kTriggerCharacteristicUUID);
    service_->addCharacteristic(trigger_);
    trigger_->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED);
    BLE2902* descriptor = new BLE2902();
    descriptor->setNotifications(true);
    trigger_->addDescriptor(descriptor);

    remote_event_ = service_->createCharacteristic(kRemoteEventCharacteristicUUID, BLECharacteristic::PROPERTY_WRITE);
    remote_event_->setAccessPermissions(ESP_GATT_PERM_WRITE_ENCRYPTED);

    service_->start();

    advertising_ = server_->getAdvertising();
    BLEAdvertisementData advertisement_data;
    advertisement_data.setName("Pirate Buzzer");
    advertising_->setAdvertisementData(advertisement_data);
    advertising_->addServiceUUID(kServiceUUID);

    security_ = new BLESecurity();
    security_->setStaticPIN(123456); // TODO change to bonding
    ESP_LOGI(BLM_LOG_TAG, "server listening on %s", BLEDevice::getAddress().toString().c_str());

    ESP_ERROR_CHECK(esp_event_handler_register(BUZZER_EVENT, TRIGGER_UP_EVENT, blm_trigger_up_event_handler, this));
}

void BluetoothManager::onConnect(BLEServer* server) {
    ESP_LOGI(BLM_LOG_TAG, "client connected");
    advertising_->stop();
    ESP_ERROR_CHECK(esp_event_post(BUZZER_EVENT, CONNECTED, nullptr, 0, 10 / portTICK_PERIOD_MS ));
}

void BluetoothManager::onDisconnect(BLEServer* server) {
    ESP_LOGI(BLM_LOG_TAG, "client disconnected");
    advertising_->start();
    ESP_ERROR_CHECK(esp_event_post(BUZZER_EVENT, DISCONNECTED, nullptr, 0, 10 / portTICK_PERIOD_MS ));
}

void BluetoothManager::triggerPressed() {
    trigger_->triggerPressed();
}

void TriggerCharacteristic::triggerPressed() {
    ESP_LOGD(BLM_LOG_TAG, "setting trigger characteristic");
    int64_t current_time = esp_timer_get_time();
    setValue((uint8_t*)&current_time, sizeof(int64_t));
    notify();
}

void RemoteEventCharacteristicCallbacks::onWrite(BLECharacteristic* pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    ESP_LOGI(BLM_LOG_TAG, "remote event called.");
    if(value.size() >= 2) {
        const char* data = value.c_str();
        buzzer_remote_event_data_t event_data = {
            .major = static_cast<RemoteEventMajorCode>(*data),
            .minor = static_cast<RemoteEventMinorCode>(*(++data)),
        };
        ESP_LOGI(BLM_LOG_TAG, "major: %02x", event_data.major);
        ESP_LOGI(BLM_LOG_TAG, "minor: %02x", event_data.minor);
        ESP_ERROR_CHECK(esp_event_post(BUZZER_EVENT, REMOTE_EVENT, &event_data, sizeof(event_data), 10 / portTICK_PERIOD_MS ));
    } 
}

