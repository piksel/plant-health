#include <Arduino.h>
#include "version.h"

#include "iot.h"
#include "cap_probe.h"
#include "flow_meter.h"
#include "log.h"
#include "water_pump.h"
#include "tank_meter.h"


const int pump_relay_pin = 21; //Pin 21
const int tank_float_switch_pin = 14;
const int water_flow_interrupt_pin = 32;
const uint8_t cap_probe_addr = 0x36;



// Modules
IoT* iot;
CapProbe *cap_probe;
FlowMeter *flow_meter;
WaterPump *water_pump;
TankMeter *tank_meter;

State state;


void setup() {
    Serial.begin(115200);

    log("Plant Health Controller v"); logln(FIRMWARE_VERSION);

    pinMode(LED_BUILTIN, OUTPUT);

    iot = new IoT();
    cap_probe = new CapProbe(cap_probe_addr);
    flow_meter = new FlowMeter(water_flow_interrupt_pin);
    water_pump = new WaterPump(pump_relay_pin);
    tank_meter = new TankMeter(tank_float_switch_pin);


}

void loop() {

    // Reading modules
    flow_meter->update(&state);
    cap_probe->update(&state);
    tank_meter->update(&state);

    // Acting modules
    water_pump->update(&state);
    iot->update(&state);

    Serial.println();
    sleep(1000);
}




