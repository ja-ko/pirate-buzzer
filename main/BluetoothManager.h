#pragma once

#include "BLEDevice.h"

class TriggerCharacteristic: public BLECharacteristic {
public:
    TriggerCharacteristic(const char* uuid): BLECharacteristic(BLEUUID(uuid), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY) {
        int64_t zero = 0;
        setValue((uint8_t*)&zero, sizeof(int64_t));
    }
    void triggerPressed();
};

class RemoteEventCharacteristicCallbacks: public BLECharacteristicCallbacks {
	virtual void onRead(BLECharacteristic* pCharacteristic);
	virtual void onWrite(BLECharacteristic* pCharacteristic);
};

class BluetoothManager: public BLEServerCallbacks {
public:
    BluetoothManager();
    virtual void onConnect(BLEServer* pServer);
	virtual void onDisconnect(BLEServer* pServer);
    virtual void triggerPressed();

private:
    const char* kServiceUUID = "01ce20d7-8264-4245-91e4-3f51579bff65";
    const char* kTriggerCharacteristicUUID = "5d71708d-b707-4b71-a749-8aed19d545d9";
    const char* kRemoteEventCharacteristicUUID = "fb3d83a3-3d0e-4bb5-8f16-dc57c6f90a31";
    BLEServer* server_;
    BLEService* service_;
    TriggerCharacteristic* trigger_;
    BLECharacteristic* remote_event_;
    BLESecurity* security_;
    BLEAdvertising* advertising_;

}; // BluetoothManager


