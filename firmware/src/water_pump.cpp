#include "water_pump.h"
#include "log.h"

bool WaterPump::on_init() {
    pinMode(_pin, OUTPUT);
    _last_water = millis();

    return true;
}

void WaterPump::start_watering() {
    // Make sure we are not already watering
    if (_pour_end != 0) return;

    _pour_end = millis() + _pour_interval;
    digitalWrite(_pin, HIGH);
}

void WaterPump::stop_watering() {
    if (_pour_end == 0) return;

    digitalWrite(_pin, LOW);
    _last_water = millis();
    _pour_end = 0;
}

void WaterPump::on_update(State *state) {

    int now = millis();
    int diff = now - _last_water;
    log("Cooldown: "); log(diff); log(" < "); log(_last_water); log(" < "); logln(_cooldown);

    if (state->soil_cap > state->soil_cap_thresh) {

        digitalWrite(LED_BUILTIN, HIGH);


        if (diff > _cooldown && _pour_end == 0) {
            start_watering();
        }


    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }


    if (_pour_end != 0 && now > _pour_end) {
        stop_watering();
    }

}