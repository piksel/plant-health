#include "stdint.h"

#ifndef PLANT_HEALTH_FIRMWARE_STATE_H
#define PLANT_HEALTH_FIRMWARE_STATE_H

struct State {
    uint16_t soil_cap = 0;
    uint16_t soil_cap_thresh = 700;
    float soil_temp = 0;
    bool floater = false;
    float flow_rate = 0;
};

#endif //PLANT_HEALTH_FIRMWARE_STATE_H
