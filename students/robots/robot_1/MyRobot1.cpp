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

        /*vector<string> params = split(update, " ", 2);
// << "Command: " << params.at(0) << endl;
// << "Parameters: " << params.at(1) << endl;*/

        vector<Direction> robots, boni;

        Message msg(update);
        robots.insert(msg.robots.end(), msg.robots.begin(), msg.robots.end());
        boni.insert(msg.boni.end(), msg.boni.begin(), msg.boni.end());
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
            //Go for bonus
        }
        if (energy <= 5) {
            //run away
        }
    }
}

/**
 * Returns robot's name
 * @return
 */
string MyRobot1::name() const {
    return "Maschallatron 3000";
}
