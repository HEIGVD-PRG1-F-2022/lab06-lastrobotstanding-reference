/*
 *  HEIG-VD
 *  Description: PRG1 Labo6 - Last Robot Standing
 *  Author(s): Bruno Manuel Vieira Rosas & Ahmad Jano
 *  Date: 29.11.2022
*/

#include "MyRobot1.h"

using namespace std;

void MyRobot1::setConfig(size_t width_init, size_t height_init, unsigned energy_init,
                         unsigned power_init) {
    width = width_init;
    height = height_init;
    energy = energy_init;
    power = power_init;
}

/**
 *
 * @param updates
 * @return
 */
string MyRobot1::action(vector<string> updates) {
    for (const auto &update: updates) {
        vector<Direction> robots, boni;

        Message msg(update);
        robots.insert(robots.end(), msg.robots.begin(), msg.robots.end());
        boni.insert(boni.end(), msg.boni.begin(), msg.boni.end());

        switch (msg.msg) {
            case MessageType::UpdateDamage:
                energy -= msg.energy;
                robots.push_back(msg.robots.at(0));
                break;
            case MessageType::UpdateEnergy:
                energy += msg.energy;
                break;
            case MessageType::UpdatePower:
                power += msg.power;
            case MessageType::UpdateBonus:
            case MessageType::UpdateBoard:
            case MessageType::UpdateRobot:
            default:
                break;
        }

        if (!boni.empty()) {
            // TODO: Choose the nearest bonus.
            return Message::actionMove(boni[0]);
        }
        else if (energy < 6) {
            if (!robots.empty()){
                return Message::actionMove(robots[0].neg());
            }
        }
        else if(!robots.empty()){
            return Message::actionAttack(robots[0]);
        }
        else{
            roundCounter++;
            if (!robots.empty() || power >= 3   )
            {
                changeDirection(false);
            }
            if (roundCounter % 10 == 0){
                changeDirection();
            }
            return Message::actionMove(dir);
        }
    }
}

void MyRobot1::changeDirection(bool waitAllowed) {
    if (waitAllowed){
        dir = Direction ((rand() % 3) - 1,(rand() % 3) - 1);
    }
    else {
        int x ,y;
        do {
            x = (rand() % 3) - 1;
            y = (rand() % 3) - 1;
        } while (x == 0 && y == 0);
        dir = Direction (x, y);
    }
}

/**
 * Returns robot's name
 * @return
 */
string MyRobot1::name() const {
    return "Maschallatron 3000";
}
