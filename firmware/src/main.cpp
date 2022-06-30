#include <Arduino.h>
#include "version.h"

#include "iot.h"
#include "cap_probe.h"
#include "flow_meter.h"
#include "log.h"



int relay = 21; //Pin 21
int float_switch = 14;
int flow_pin = 32;

const int COOLDOWN = 10000;
int pour_interval = 1000;
int pour_end = 0;
uint16_t last_water;


// Modules
IoT* iot;
CapProbe *cap_probe;
FlowMeter *flow_meter;

State state;






volatile int NbTopsFan = 0; //measuring the rising edges of the signal
int Calc;

void ARDUINO_ISR_ATTR flow_interrupt() {
    NbTopsFan++;

}

void setup() {
    Serial.begin(115200);

    log("Plant Health Controller v"); logln(FIRMWARE_VERSION);

    pinMode(relay, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(float_switch, INPUT_PULLUP);
    pinMode(flow_pin, INPUT);
    attachInterrupt(flow_pin, flow_interrupt, FALLING);

    iot = new IoT();
    cap_probe = new CapProbe(0x36);




    last_water = millis();
}

void loop() {



    log("Temperature: "); log(state.tempC); logln("*C");
    log("Capacitive: "); logln(state.capread);

    int now = millis();
    int diff = now - last_water;
    Serial.print("Cooldown: "); Serial.print(diff); Serial.print(" < "); Serial.print(last_water); Serial.print(" < "); Serial.println(COOLDOWN);

    state.floater = digitalRead(float_switch);

    Serial.print("Floater: "); Serial.println(state.floater);

    if (state.capread > 700) {

        digitalWrite(LED_BUILTIN, HIGH);

        if (diff > COOLDOWN && pour_end == 0) {
            pour_end = now + pour_interval;
            digitalWrite(relay, HIGH);
        }


    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }


    if (pour_end != 0 && now > pour_end) {
        digitalWrite(relay, LOW);
        last_water = millis();
        pour_end = 0;
    }

    flow_meter->update(&state);

    cap_probe->update(&state);
    iot->update(&state);

    Serial.println();
}




