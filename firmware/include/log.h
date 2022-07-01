#ifndef LOG_H
#define LOG_H

#define logv(...) ESP_LOGV(_log_tag, __VA_ARGS__)
#define logd(...) ESP_LOGD(_log_tag, __VA_ARGS__)
#define logi(...) ESP_LOGI(_log_tag, __VA_ARGS__)
#define logw(...) ESP_LOGW(_log_tag, __VA_ARGS__)
#define loge(...) ESP_LOGE(_log_tag, __VA_ARGS__)

// #define log(...) Serial.print(__VA_ARGS__)
// #define logln(...) Serial.println(__VA_ARGS__)



#endif
