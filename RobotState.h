//
// Created by Linus Gasser on 01.12.22.
//

#ifndef INEITI_ROBOTSTATE_H
#define INEITI_ROBOTSTATE_H

#include "Message.h"
#include "Position.h"
#include <librobots.h>
#include <string>

class RobotState {
    Robot *robot;
    Position pos;
    unsigned energy, power;
    std::vector<std::string> updates_cache;
    Message action = Message("wait");

public:
    explicit RobotState(Robot *robot, Position pos, size_t side, unsigned energy, unsigned power);
    std::string name();
    [[nodiscard]] unsigned getPower() const;
    Message getAction();
    [[nodiscard]] Position getPosition() const;
    [[nodiscard]] bool isDead() const;
    void actionAttack(const RobotState &attacker, const Position &dest);
    void actionMove(const Direction& relative);
    void actionRadar(const std::vector<Position> &positions);
    void actionBonus(const Position &bonus);
    void actionEnergy(unsigned bonusEnergy);
    void actionPower(unsigned bonusPower);
    void checkCollision(RobotState &other);
    void sendUpdate(const std::string& updateBoard);
    [[nodiscard]] unsigned int getEnergy() const;
};

void testRobotState();

#endif//INEITI_ROBOTSTATE_H
