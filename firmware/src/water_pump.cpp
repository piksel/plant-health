#include "water_pump.h"
#include "log.h"

bool WaterPump::on_init() {
    pinMode(_pin, OUTPUT);
    _last_water = millis();
    _pour_end = 0;

    return true;
}

void WaterPump::start_watering() {
    // Make sure we are not already watering
    if (_pour_end != 0) return;

    logi("Watering started");

    _pour_end = millis() + _pour_interval;
    logd("Ending at %d, (now: %d)", _pour_end, millis());
    digitalWrite(_pin, HIGH);
}

void WaterPump::stop_watering() {
    if (_pour_end == 0) return;

    logi("Watering stopped");

    digitalWrite(_pin, LOW);
    _last_water = millis();
    _pour_end = 0;
}

void WaterPump::on_update(State *state) {

    uint32_t now = millis();
    uint32_t diff = now - _last_water;
    logd("Cooldown: %d < %d < %d", diff, _cooldown, _last_water);

    if (state->soil_cap > state->soil_cap_thresh) {

        digitalWrite(LED_BUILTIN, HIGH);


        if (diff > _cooldown && _pour_end == 0) {
            start_watering();
            state->events |= EVENT_WATER_ON;
            if (_iot_module) _iot_module->publish_events(state);
        }


    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }


    if (_pour_end != 0 && now > _pour_end) {
        stop_watering();
        state->events |= EVENT_WATER_OFF;
        if (_iot_module) _iot_module->publish_events(state);
    }

}

void WaterPump::set_iot(IoT *iot) {
    _iot_module = iot;
}