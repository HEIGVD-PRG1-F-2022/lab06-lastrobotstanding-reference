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

    size_t width;
    size_t height;
    unsigned energy;
    unsigned power;
    std::string randomMove();
    int numRandom;
    std::string lastMove;

    std::string randomAction();
};


#endif //REFACTOR_ROBOTTEAMR_H
