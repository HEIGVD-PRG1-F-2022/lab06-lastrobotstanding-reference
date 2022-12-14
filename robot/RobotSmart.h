//
// Created by Linus Gasser on 14.12.22.
//

#ifndef REFERENCEGAME_ROBOTSMART_H
#define REFERENCEGAME_ROBOTSMART_H

#include <iostream>
#include <librobots/Direction.h>
#include <librobots/Robot.h>

class RobotSmart : public Robot {
    size_t width = 0, height = 0;
    unsigned energy = 0, power = 0;
    unsigned nearestCounter = 0;
    Direction nearestDirection;

    void updateNearest(Direction dir, bool bonus = false);

public:
    RobotSmart() = default;

    void setConfig(size_t init_width, size_t init_height, unsigned init_energy,
                   unsigned init_power) override;

    std::string action(std::vector<std::string> updates) override;

    [[nodiscard]] std::string name() const override;
};
#endif//REFERENCEGAME_ROBOTSMART_H
