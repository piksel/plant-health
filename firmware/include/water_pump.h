#ifndef PLANT_HEALTH_FIRMWARE_AUTO_WATERING_H
#define PLANT_HEALTH_FIRMWARE_AUTO_WATERING_H

#include "module.h"


class WaterPump: public Module {
public:
    WaterPump(int pin): Module(1000), _pin{pin} {}
    void start_watering();
    void stop_watering();
protected:
    void on_update(State *state) override;
    bool on_init() override;
private:
    const int _pin;
    uint16_t _last_water;
    const int _cooldown = 10000;
    int _pour_interval = 1000;
    uint16_t _pour_end = 0;
};

#endif //PLANT_HEALTH_FIRMWARE_AUTO_WATERING_H
