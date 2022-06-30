#include "cap_probe.h"

void CapProbe::on_update(State *state) {
    auto tempC = _ss.getTemp();
    auto cap = _ss.touchRead(0);

    state->soil_cap = cap;
    state->soil_temp = tempC;

    log("Temperature: "); log(state->soil_temp); logln("*C");
    log("Capacitive: "); logln(state->soil_cap);
}

bool CapProbe::on_init() {
    if (!_ss.begin(_addr)) {
        logln("ERROR! seesaw not found");
        return false;
    } else {
        log("seesaw started! version: ");
        Serial.println(_ss.getVersion(), HEX);
        return true;
    }
}