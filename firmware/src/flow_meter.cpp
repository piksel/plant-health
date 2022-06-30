#include "flow_meter.h"
#include "log.h"
#include "Arduino.h"

void handle_interrupt(void *arg) {
    static_cast<FlowMeter*>(arg)->tick();
}

void FlowMeter::tick() {
    _fan_ticks++;
}

bool FlowMeter::on_init() {

    pinMode(_pin, INPUT);
    attachInterruptArg(_pin, handle_interrupt, this, FALLING);

    return true;
}


void FlowMeter::on_update(State *state) {

    // TODO: Account for update delays!
    //(Pulse frequency x 60) / 7.5Q, = flow rate
    state->flow_rate = (_fan_ticks * 60 / 7.5);

    _fan_ticks = 0;

    log("Flow: ");
    log(_fan_ticks);
    log("ticks, ");

    log (state->flow_rate, DEC);
    logln (" L/hour");
}
