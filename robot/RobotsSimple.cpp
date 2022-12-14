//
// Created by Linus Gasser on 14.12.22.
//

#include "RobotsSimple.h"

void RobotDiagonal::setConfig(size_t init_width, size_t init_height, unsigned init_energy,
                              unsigned init_power) {
    (void) init_width;
    (void) init_height;
    (void) init_energy;
    (void) init_power;
}

std::string RobotDiagonal::action(std::vector<std::string> updates) {
    (void) updates;
    return "move 1,1";
};

[[nodiscard]] std::string RobotDiagonal::name() const {
    return "Bishop robot";
}


void RobotWait::setConfig(size_t init_width, size_t init_height, unsigned init_energy,
                              unsigned init_power) {
    (void) init_width;
    (void) init_height;
    (void) init_energy;
    (void) init_power;
}

std::string RobotWait::action(std::vector<std::string> updates) {
    (void) updates;
    return "wait";
};

[[nodiscard]] std::string RobotWait::name() const {
    return "Zen robot";
}

