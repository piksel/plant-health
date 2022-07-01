#ifndef PLANT_HEALTH_FIRMWARE_AUTO_WATERING_H
#define PLANT_HEALTH_FIRMWARE_AUTO_WATERING_H

#include "module.h"
#include "iot.h"


class WaterPump: public Module {
public:
    WaterPump(int pin): Module(1000, "WaterPump"), _pin{pin} {
        this->init();
    }
    void start_watering();
    void stop_watering();
    void set_iot(IoT *iot);
protected:
    void on_update(State *state) override;
    bool on_init() override;
private:
    const int _pin;
    uint32_t _last_water;
    const int _cooldown = 10000;
    int _pour_interval = 1000;
    uint32_t _pour_end = 0;
    IoT *_iot_module;
};

#endif //PLANT_HEALTH_FIRMWARE_AUTO_WATERING_H
