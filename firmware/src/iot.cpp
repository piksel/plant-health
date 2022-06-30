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

void log_result(bool success) {
    if (!success) logln(F("Failed"));
    else logln(F("OK!"));
}
#define publish(f, v) \
    log(F("Sending val \"")); Serial.print(v); log(F("\" to feed ")); log(feeds[f]); \
    log_result(feed_publishers[f]->publish(v))

void IoT::on_update(State *state) {


    // Ensure the connection to the MQTT server is alive (this will make the first
    // connection and automatically reconnect when disconnected).  See the MQTT_connect
    // function definition further below.
    if(!MQTT_connect()) {
        log("Not connected! Skipping MQTT publishing.");
        return;
    }

    publish(FEED_CAPACITY, state->soil_cap);


}



IoT::IoT() : Module(iot_update_freq) {
    // Set Adafruit IO's root CA
    _wifi_client.setCACert(adafruitio_root_ca);

    _mqtt = new Adafruit_MQTT_Client(&_wifi_client, AIO_SERVER_HOST, AIO_SERVER_PORT, AIO_USERNAME, AIO_KEY);

    for (int i = 0; i < NUM_FEEDS; i++) {
        feed_publishers[i] = new Adafruit_MQTT_Publish(_mqtt, feeds[i]);
    }


    log("Connecting to "); logln(WLAN_SSID);

    delay(1000);
    WiFi.disconnect();
    delay(600);
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    delay(5000);

    auto wl_status = WiFiClass::status();

    while (wl_status != WL_CONNECTED) {

        log("WiFi Status: "); logln(wl_status_name(wl_status));
        delay(500);
        wl_status = WiFiClass::status();
    }
    logln();

    logln("WiFi connected!");
    log("IP address: "); logln(WiFi.localIP());


}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
boolean IoT::MQTT_connect() {
    int8_t ret;

    // Stop if already connected.
    if (_mqtt->connected()) {
        return true;
    }

    Serial.print("Connecting to MQTT... ");

    uint8_t retries = 3;
    while ((ret = _mqtt->connect()) != 0) { // connect will return 0 for connected
        logln(_mqtt->connectErrorString(ret));
        logln("Retrying MQTT connection in 5 seconds...");
        _mqtt->disconnect();
        delay(5000);  // wait 5 seconds
        retries--;
        if (retries == 0) {
            return false;
        }
    }

    logln("MQTT Connected!");

    return true;
}

