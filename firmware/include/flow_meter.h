#ifndef PLANT_HEALTH_FIRMWARE_FLOW_METER_H
#define PLANT_HEALTH_FIRMWARE_FLOW_METER_H
#include "module.h"


class FlowMeter: public Module {
public:
    FlowMeter(int pin): Module(1000, "FlowMeter"), _pin{pin} {
        this->init();
    }
    void tick();
protected:
    void on_update(State *state) override;
    bool on_init() override;
private:
    const int _pin;
    volatile int _fan_ticks = 0;
};


#endif //PLANT_HEALTH_FIRMWARE_FLOW_METER_H
