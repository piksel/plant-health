#include "cap_probe.h"

void CapProbe::on_update(State *state) {
    auto tempC = _ss.getTemp();
    auto cap = _ss.touchRead(0);

    state->capread = cap;
    state->tempC = tempC;
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

CapProbe::CapProbe(int addr) : Module(1000), _addr{addr} {

}
