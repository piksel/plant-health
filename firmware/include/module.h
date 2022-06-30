#ifndef MODULE_H
#define MODULE_H

#include "Arduino.h"
#include "state.h"

class Module {
  public:
    bool init() {
        this->_last_init = millis();
        return this->_available = this->on_init();
    }

    void update(State *state) {
        auto now = millis();
        if (!_available && now - this->_last_init <= this->_retry_frequency) {
            if(!init()) return;
        }
        if (now - this->_last_updated <= this->_update_frequency) {
            return;
        }
        this->on_update(state);
        this->_last_updated = millis();
    }

protected:
    explicit Module(int update_frequency) {
        this->_update_frequency = update_frequency;
        this->_last_updated = 0;
        this->init();
    }
    virtual void on_update(State *state) {}
    virtual bool on_init() {return true;}
    boolean _available;
  private:
    int _update_frequency;
    int _retry_frequency;
    int _last_updated;
    int _last_init;
};
#endif