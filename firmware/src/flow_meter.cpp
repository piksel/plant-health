#include "flow_meter.h"
#include "../include/flow_meter.h"
#include "log.h"
#include "Arduino.h"

bool FlowMeter::on_init() {


    return true;
}

void FlowMeter::on_update(State *state) {
    _fan_ticks = 0;
    sei();      //Enables interrupts
    delay (1000); //Wait 1 second
    cli();        //Disable interrupts

    Serial.print("Flow: ");
    Serial.print(_fan_ticks);
    Serial.print("ticks, ");

    //(Pulse frequency x 60) / 7.5Q, = flow rate
    state->flow_rate = (_fan_ticks * 60 / 7.5);
    Serial.print (state->flow_rate, DEC);
    Serial.println (" L/hour");
}
