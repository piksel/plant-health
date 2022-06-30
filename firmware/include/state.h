#include "stdint.h"

#ifndef PLANT_HEALTH_FIRMWARE_STATE_H
#define PLANT_HEALTH_FIRMWARE_STATE_H

struct State {
    uint16_t capread = 0;
    float tempC = 0;
    bool floater = false;
    float flow_rate = 0;
};

#endif //PLANT_HEALTH_FIRMWARE_STATE_H
