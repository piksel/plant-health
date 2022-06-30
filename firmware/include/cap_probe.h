#include "module.h"
#include "state.h"
#include "Adafruit_seesaw.h"
#include "log.h"

class CapProbe: public Module {
public:
    explicit CapProbe(int addr);
protected:
    void on_update(State *state) override;
    bool on_init() override;
private:
    int _addr;
    Adafruit_seesaw _ss;
};