#ifndef PLANT_HEALTH_FIRMWARE_IOT_H
#define PLANT_HEALTH_FIRMWARE_IOT_H

#include <WiFi.h>
#include "WiFiClientSecure.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "module.h"
#include "state.h"
#include "config.h"

#define AIO_FEED_PFX  AIO_USERNAME "/feeds/"


typedef enum {
    FEED_CAPACITY,
    FEED_TEMP,
    FEED_FLOAT,

    NUM_FEEDS
} feed_t;

class IoT: public Module {
public:
    IoT();
protected:
    void on_update(State *state) override;
    bool on_init() override;
private:
    // WiFiFlientSecure for SSL/TLS support
    WiFiClientSecure _wifi_client;

    Adafruit_MQTT_Client* _mqtt;
    const char* feeds[NUM_FEEDS] {
            AIO_FEED_PFX "test",
            AIO_FEED_PFX "soil_temp",
            AIO_FEED_PFX "soil_floater"
    };
    Adafruit_MQTT_Publish* feed_publishers[NUM_FEEDS]{};

    boolean MQTT_connect();
};

#endif //PLANT_HEALTH_FIRMWARE_IOT_H