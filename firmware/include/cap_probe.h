#include "module.h"
#include "state.h"
#include "Adafruit_seesaw.h"
#include "log.h"

class CapProbe: public Module {
public:
    explicit CapProbe(uint8_t addr) : Module(1000), _addr{addr} {}
protected:
    void on_update(State *state) override;
    bool on_init() override;
private:
    uint8_t _addr;
    Adafruit_seesaw _ss;
};