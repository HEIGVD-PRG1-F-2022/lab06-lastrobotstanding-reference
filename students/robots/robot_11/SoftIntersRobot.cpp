//
// Created by bioan on 29.11.2022.
//

#include "SoftIntersRobot.h"

using namespace std;

string SoftIntersRobot::action(vector<string> updates) {

    // return Message::actionWait();
    return Message::actionMove(Direction(0, 1));
    // return Message::actionAttack(Direction(-1, 1));
}

string SoftIntersRobot::name() const {
    return NAME_ROBOT;
}

void SoftIntersRobot::setConfig(size_t width, size_t height, unsigned energy, unsigned power) {
    largeurBoard = width;
    hauteurBoard = height;
    hp = energy;
    dmgPower = power;
}
