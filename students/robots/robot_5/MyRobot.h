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
    Direction lastRobot = Direction(0, 0);
    int countRound;
    std::vector<Direction> boniList;
    std::vector<Direction> robotsList;
    size_t lastPower = 0, lastEnergy = 0;
    Direction lastMove = Direction(0, 0);
public:
    void setMemoryList(std::vector<Direction>, std::vector<Direction>&);
    void updatePosition(std::vector<Direction>);

    void setConfig(size_t width_init, size_t height_init, unsigned energy_init,
                   unsigned power_init) override;

    std::string action(std::vector<std::string> updates) override;

    [[nodiscard]] std::string name() const override;

    bool isDuplicate(Direction, std::vector<Direction>);
    bool isMissing(Direction, std::vector<Direction>);

    Direction getClosest(std::vector<Direction>);

    Direction getRandomDirection();

    std::string moveTowards(Direction target);

    int getIndex(std::vector<double> v, double K);
    void updatePositionObject(std::vector<Direction>& objectType, Direction lastMove);
};


#endif //REFACTOR_MYROBOT_H