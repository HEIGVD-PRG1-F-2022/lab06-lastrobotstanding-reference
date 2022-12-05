#include <librobots.h>

class RobotRandom : public Robot {
    size_t width = 0, height = 0;
    unsigned energy = 0, power = 0;

public:
    RobotRandom()= default;

    void setConfig(size_t init_width, size_t init_height, unsigned init_energy,
                   unsigned init_power) override {
        width = init_width;
        height = init_height;
        energy = init_energy;
        power = init_power;
    }

    std::string action(std::vector<std::string> updates) override {
        if (updates.size() > 1){
            return "move 1,-1";
        } else {
            return "move 1,1";
        }
    };

    [[nodiscard]] std::string name() const override {
        return "Random one";
    }
};