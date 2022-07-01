#ifndef MODULE_H
#define MODULE_H

#include "Arduino.h"
#include "state.h"
#include "log.h"

class Module {
  public:
    bool init() {
        logd("Initializing");
        this->_last_init = millis();
        return this->_available = on_init();
    }

    void update(State *state) {
        auto now = millis();
        if (!_available && now - this->_last_init <= this->_retry_frequency) {
            if(!init()) return;
        }
        if (now - this->_last_updated <= this->_update_frequency) {
            logd("Skipping update");
            return;
        }
        logd("Running update");
        on_update(state);
        this->_last_updated = millis();
    }

protected:
    explicit Module(int update_frequency, const char* log_tag)
        : _update_frequency{update_frequency}, _log_tag{log_tag} {

        this->_last_updated = 0;

    }
//    void logv(const char* format, va_list argp) { ESP_LOGV(_log_tag, format, argp); }
//    void logd(const char* format, va_list argp) { ESP_LOGD(_log_tag, format, argp); }
//    void logi(const char* format, va_list argp) { ESP_LOGI(_log_tag, format, argp); }
//    void logw(const char* format, va_list argp) { ESP_LOGW(_log_tag, format, argp); }
//    void loge(const char* format, va_list argp) { ESP_LOGE(_log_tag, format, argp); }
    virtual void on_update(State *state) { logw("on_update not overriden!"); }
    virtual bool on_init() { logw("on_init not overriden!"); return true;}
    boolean _available;
    int _retry_frequency = 600000; // 10 minutes
    const char* _log_tag;
private:
    const int _update_frequency;
    int _last_updated;
    int _last_init;
};
#endif