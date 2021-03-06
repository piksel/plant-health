#include "module.h"
#include "state.h"
#include "Adafruit_seesaw.h"
#include "log.h"

class TankMeter: public Module {
public:
    explicit TankMeter(int pin) : Module(1000, "TankMeter"), _pin{pin} {
        this->init();
    }
protected:
    void on_update(State *state) override;
    bool on_init() override;
private:
    int _pin;
};