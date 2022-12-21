//
// Created by emily on 08.12.2022.
//

#ifndef REFACTOR_MYROBOT_H
#define REFACTOR_MYROBOT_H

#include "librobots/Direction.h"
#include "librobots/Message.h"
#include "librobots/Position.h"
#include "librobots/Robot.h"
#include "librobots/RobotState.h"
#include <string>
#include <vector>


class MyRobot : public Robot {
    size_t width = 0, height = 0, energy = 0, power = 0;
    Direction target = Direction(0, 0);
    int countRound;
public:
    Direction setTarget(int x, int y){

        return target = Direction(x, y);
    }
    Direction getTarget(){
        return target;
    }
    void setConfig(size_t width_init, size_t height_init, unsigned energy_init,
                   unsigned power_init) override;

    std::string action(std::vector<std::string> updates) override;

    [[nodiscard]] std::string name() const override;

    bool isDuplicate(Direction, std::vector<Direction>);

    Direction getClosest(std::vector<Direction>);

    std::string moveTo(Direction target);

    // all good :)
    std::string wander();
};


#endif //REFACTOR_MYROBOT_H
