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
    FEED_WTR_TSH,
    FEED_EVENT,

    NUM_FEEDS
} feed_t;

enum feed_mode_t {
    FEED_SUBSCRIBE = 1,
    FEED_PUBLISH = 2
};

class IoT: public Module {
public:
    IoT();
    void publish_events(State *state);
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
            AIO_FEED_PFX "soil_floater",
            AIO_FEED_PFX "soil-dot-watering-thresh",
            AIO_FEED_PFX "plant-health.event"
    };
    Adafruit_MQTT_Publish* feed_publishers[NUM_FEEDS]{};
    Adafruit_MQTT_Subscribe* feed_subcribers[NUM_FEEDS]{};
    int feed_modes[NUM_FEEDS]{
            FEED_PUBLISH,
            FEED_PUBLISH,
            FEED_PUBLISH,
            FEED_SUBSCRIBE,
            FEED_SUBSCRIBE | FEED_PUBLISH,
    };

    boolean MQTT_connect();

    void publish_event(eventid_t event, State *state);
};

#endif //PLANT_HEALTH_FIRMWARE_IOT_H