#include "iot.h"
#include "log.h"

const int iot_update_freq = 10000;

// io.adafruit.com root CA
const char* adafruitio_root_ca = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
    "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
    "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
    "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
    "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
    "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
    "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
    "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
    "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
    "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
    "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
    "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
    "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
    "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
    "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
    "-----END CERTIFICATE-----\n";

const char* wl_status_name(wl_status_t status) {
    switch(status) {
        case WL_NO_SHIELD: return "NO_SHIELD";
        case WL_IDLE_STATUS: return "IDLE_STATUS";
        case WL_NO_SSID_AVAIL: return "NO_SSID_AVAIL";
        case WL_SCAN_COMPLETED: return "SCAN_COMPLETED";
        case WL_CONNECTED: return "CONNECTED";
        case WL_CONNECT_FAILED: return "CONNECT_FAILED";
        case WL_CONNECTION_LOST: return "CONNECTION_LOST";
        case WL_DISCONNECTED: return "DISCONNECTED";
        default: return "UNKNOWN";
    }
}

#define publish(f, v) \
    logi("Sending val %s to feed %s... %s", iot_fmt(v), feeds[f], feed_publishers[f]->publish(v) ? "OK" : "Failed!")

char iot_value_buf[10];
char* iot_fmt(uint16_t v) { snprintf(iot_value_buf, 10, "%d",    v); return iot_value_buf; }
char* iot_fmt(float v)    { snprintf(iot_value_buf, 10, "%2.2f", v); return iot_value_buf; }
char* iot_fmt(bool v)     { snprintf(iot_value_buf, 10, "%d",    v); return iot_value_buf; }

void IoT::publish_event(eventid_t eventid, State *state) {
    auto event = (1 << eventid);
    if (state->events & event) {
        publish(FEED_EVENT, event_names[eventid]);
        state->events &= ~event;
    }
}

void IoT::on_update(State *state) {

    // Ensure the connection to the MQTT server is alive (this will make the first
    // connection and automatically reconnect when disconnected).  See the MQTT_connect
    // function definition further below.
    if(!MQTT_connect()) {
        logw("Not connected! Skipping MQTT publishing.");
        return;
    }

    publish(FEED_CAPACITY, state->soil_cap);
    publish(FEED_TEMP, state->soil_temp);
    publish(FEED_FLOAT, state->floater);

    publish_events(state);


    delay(1000);

    Adafruit_MQTT_Subscribe *sub;
    while ((sub = _mqtt->readSubscription(1000))) {
        if(sub == feed_subcribers[FEED_WTR_TSH]) {
            state->soil_cap_thresh = strtol((char *)sub->lastread, nullptr, 10);
            logi("Updated auto watering threshold to %d", state->soil_cap_thresh);
        }
        else if(sub == feed_subcribers[FEED_EVENT]) {
            if (strcmp((char *)sub->lastread, event_names[EVENT_MAN_WATER]) == 0) {
                logi("Got manual watering event");
                state->events |= EVENT_MAN_WATER;
            }
            else {
                logw("Got unknown event \"%s\"", sub->lastread);
            }
        }
        else {
            logw("Got unknown subscription from \"%s\"", sub->topic);
        }
    }

}

bool IoT::on_init() {
    logi("Connecting to %s", WLAN_SSID);

    delay(1000);
    WiFi.disconnect();
    delay(600);
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    delay(2000);

    auto wl_status = WiFiClass::status();

    byte retries = 10;
    while (wl_status != WL_CONNECTED) {
        logd("Waiting for connection (%d s left). Status: %s", retries, wl_status_name(wl_status));

        delay(1000);
        wl_status = WiFiClass::status();
        if (retries-- == 0) return false;
    }
    // Add a small delay here to prevent logging issues
    delay(100);
    logi("WiFi connected!");
    logd("IP address: %s", WiFi.localIP().toString().c_str());

    return true;
}

IoT::IoT() : Module(iot_update_freq, "IoT") {
    logd("Setting WiFi cert");
    // Set Adafruit IO's root CA
    _wifi_client.setCACert(adafruitio_root_ca);

    logd("Initializing MQTT Client...");
    _mqtt = new Adafruit_MQTT_Client(&_wifi_client, AIO_SERVER_HOST, AIO_SERVER_PORT, AIO_USERNAME, AIO_KEY);

    logd("Initializing feed publishers...");
    for (int i = 0; i < NUM_FEEDS; i++) {
        if (feed_modes[i] & FEED_SUBSCRIBE) {
            feed_subcribers[i] = new Adafruit_MQTT_Subscribe(_mqtt, feeds[i]);
            _mqtt->subscribe(feed_subcribers[i]);
        }
        if (feed_modes[i] & FEED_PUBLISH) {
            feed_publishers[i] = new Adafruit_MQTT_Publish(_mqtt, feeds[i]);
        }
    }

    this->init();
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
boolean IoT::MQTT_connect() {
    int8_t ret;

    // Stop if already connected.
    if (_mqtt->connected()) {
        return true;
    }

    logi("Connecting to MQTT... ");

    uint8_t retries = 3;
    while ((ret = _mqtt->connect()) != 0) { // connect will return 0 for connected
        logw("Failed to connect to MQTT: %s", _mqtt->connectErrorString(ret));
        logi("Retrying MQTT connection in 5 seconds...");
        _mqtt->disconnect();
        delay(5000);  // wait 5 seconds
        retries--;
        if (retries == 0) {
            return false;
        }
    }

    logi("MQTT Connected!");

    return true;
}

void IoT::publish_events(State *state) {
    if(!MQTT_connect()) {
        logw("Not connected! Skipping MQTT publishing.");
        return;
    }
    publish_event(EVENTID_WATER_ON, state);
    publish_event(EVENTID_WATER_OFF, state);
}

