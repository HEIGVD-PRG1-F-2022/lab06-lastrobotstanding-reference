/**
 * @author : Rachel Tranchida
 * @date : 29.11.2022
 * @file : RobotTeamR
 * @project : Refactor
 */
#ifndef REFACTOR_ROBOTTEAMR_H
#define REFACTOR_ROBOTTEAMR_H

#include <string>
#include <vector>
#include <iostream>
#include <librobots/Robot.h>
#include <string>
#include "librobots/Message.h"

class RobotTeamR : public Robot {


public:


    std::string action(std::vector<std::string> updates) override;

    [[nodiscard]] std::string name() const override;

    void setConfig(size_t width, size_t height, unsigned energy, unsigned power) override;


private:

    [[maybe_unused]] size_t width;
    [[maybe_unused]] size_t height;
    unsigned energy;
    unsigned power;
    const unsigned MIN_ENERGY_HUNTING = 15;
    const unsigned MIN_POWER_HUNTING = 5;
    const unsigned MIN_ENERGY_ATTACK = 12;
    const unsigned MIN_POWER_ATTACK = 3;
    const unsigned VISIBILITY = 3;


    std::string randomMove();

    int numRandom;
    std::string lastMove;

    std::string randomAction();

    static std::string escapeMove(Direction dirEnemy, std::vector<Direction> robotsAutour);

    static bool isBeside(Direction dir, const std::vector<Direction> &robotsAutour);

    static Direction rotateRounded(double angle, Direction dir);
};


#endif //REFACTOR_ROBOTTEAMR_H
