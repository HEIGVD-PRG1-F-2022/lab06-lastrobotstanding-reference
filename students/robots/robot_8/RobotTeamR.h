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

    /*RobotTeamR(size_t width, size_t height, unsigned energy, unsigned power) : width(0), height(0), energy(energy),
                                                                               power(power) {
// << "Un robot " << name() << " with energy : " << this->energy << " and power : " << this->power
                  << std::endl;

    };*/

    std::string action(std::vector<std::string> updates) override;

    std::string name() const override;

    void setConfig(size_t width, size_t height, unsigned energy, unsigned power) override;

    size_t getEnergy() const;

    size_t getPower() const;

    std::string randomAction();

private:

    size_t width;
    size_t height;
    unsigned energy;
    unsigned power;

};


#endif //REFACTOR_ROBOTTEAMR_H
