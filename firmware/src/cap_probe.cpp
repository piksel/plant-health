#include "cap_probe.h"

void CapProbe::on_update(State *state) {
    logd("Reading values...");

    state->soil_cap = _ss.touchRead(0);
    state->soil_temp = _ss.getTemp();

    logd("Temperature: %2.2f'C", state->soil_temp);
    logd("Capacitive: %d", state->soil_cap);
}

bool CapProbe::on_init() {
    if (!_ss.begin(_addr)) {
        loge("Seesaw not found!");
        return false;
    } else {
        logi("seesaw started! version: %x", _ss.getVersion());
        return true;
    }
}