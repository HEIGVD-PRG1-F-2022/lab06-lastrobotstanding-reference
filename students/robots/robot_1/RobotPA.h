//
// Created by PatrickMaillard on 29.11.2022.
//

#ifndef REFACTOR_ROBOT_H
#define REFACTOR_ROBOT_H

#include <iostream>
#include <vector>
#include "librobots/Robot.h"

using namespace std;

class RobotPA : public Robot{
public:

    void setConfig(size_t width_init, size_t height_init, unsigned energy_init,
                   unsigned power_init) override;

    string action(vector<string> updates) override;

    [[nodiscard]] string name() const override { return "RobotPA"; };
    [[nodiscard]] int getEnergy() const;
    void setEnergy(int newEnergy);
    [[nodiscard]] unsigned getPower() const;
    [[nodiscard]] unsigned getRobotNumber() const;

private:
    size_t width, height;
    static unsigned numberOfRobot; //pour l'initialisation de robotNumber
    int energy;
    unsigned power;
    string _name;
    unsigned robotNumber; // le numéro du robot qui sera utilisé pour le board
};


#endif //REFACTOR_ROBOT_H
