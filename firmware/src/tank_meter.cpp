#include "tank_meter.h"
#include "log.h"

void TankMeter::on_update(State *state) {

    state->floater = digitalRead(_pin);

    logd("Floater: %d", state->floater);
}

bool TankMeter::on_init() {
    pinMode(_pin, INPUT_PULLUP);

    return true;
}