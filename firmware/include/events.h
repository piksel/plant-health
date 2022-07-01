#ifndef FIRMWARE_EVENTS_H
#define FIRMWARE_EVENTS_H

typedef enum {
    EVENTID_MAN_WATER,
    EVENTID_WATER_ON,
    EVENTID_WATER_OFF,
    EVENT_NUM = 3
} eventid_t;

enum {
    EVENT_MAN_WATER = 1 << EVENTID_MAN_WATER,
    EVENT_WATER_ON  = 1 << EVENTID_WATER_ON,
    EVENT_WATER_OFF = 1 << EVENTID_WATER_OFF,
};

extern const char* event_names[EVENT_NUM];

#endif //FIRMWARE_EVENTS_H
