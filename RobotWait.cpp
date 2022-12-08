#include <iostream>
#include <librobots/Robot.h>
#include <librobots/Message.h>

using namespace std;

class RobotWait : public Robot {
    size_t width = 0, height = 0, energy = 0, power = 0;

public:
    void setConfig(size_t width_init, size_t height_init, unsigned energy_init,
                   unsigned power_init) override {
        width = width_init;
        height = height_init;
        energy = energy_init;
        power = power_init;
    };

    string action(vector<string> updates) override {
        for (const auto &update : updates) {
            vector<string> params = split(update, " ", 2);
        }
        return "wait";
    }

    [[nodiscard]] string name() const override { return "Waiting Robot"; }
};
