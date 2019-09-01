#pragma once

#include "esp_event_loop.h"

ESP_EVENT_DECLARE_BASE(BUZZER_EVENT);

enum BuzzerEvent {
    TRIGGER_DOWN_EVENT,
    TRIGGER_UP_EVENT,
    CONNECTED,
    DISCONNECTED,
    REMOTE_EVENT,
};

enum RemoteEventMajorCode {
    RELEASE = 0x01,
    ROLLBACK = 0x02
};

enum RemoteEventMinorCode {
    STARTED = 0x01,
    CONFIRMED = 0x02,
    DONE = 0x03,

};

typedef struct {
    RemoteEventMajorCode major;
    RemoteEventMinorCode minor;
} buzzer_remote_event_data_t;
