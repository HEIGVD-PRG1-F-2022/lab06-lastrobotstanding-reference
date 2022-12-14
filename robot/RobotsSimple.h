//
// Created by Linus Gasser on 14.12.22.
//

#ifndef REFERENCEGAME_ROBOTSSIMPLE_H
#define REFERENCEGAME_ROBOTSSIMPLE_H

#include <iostream>
#include <librobots/Direction.h>
#include <librobots/Robot.h>

class RobotDiagonal : public Robot {
public:
    RobotDiagonal() = default;

    void setConfig(size_t init_width, size_t init_height, unsigned init_energy,
                   unsigned init_power) override;

    std::string action(std::vector<std::string> updates) override;

    [[nodiscard]] std::string name() const override;
};

class RobotWait : public Robot {
public:
    RobotWait() = default;

    void setConfig(size_t init_width, size_t init_height, unsigned init_energy,
                   unsigned init_power) override;

    std::string action(std::vector<std::string> updates) override;

    [[nodiscard]] std::string name() const override;
};

#endif//REFERENCEGAME_ROBOTSSIMPLE_H
