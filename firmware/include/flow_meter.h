#ifndef PLANT_HEALTH_FIRMWARE_FLOW_METER_H
#define PLANT_HEALTH_FIRMWARE_FLOW_METER_H
#include "module.h"


class FlowMeter: public Module {
public:
    FlowMeter(int pin): Module(1000), _pin{pin} {}
protected:
    void on_update(State *state) override;
    bool on_init() override;
private:
    int _pin;
    int _fan_ticks;
};


#endif //PLANT_HEALTH_FIRMWARE_FLOW_METER_H
