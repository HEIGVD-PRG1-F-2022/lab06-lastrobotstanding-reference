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
};

/**
 *
 * @param updates
 * @return
 */
string MyRobot1::action(vector<string> updates){
    for (const auto &update: updates) {
        vector<string> params = split(update, " ", 2);
// << "Command: " << params.at(0) << endl;
// << "Parameters: " << params.at(1) << endl;

        Message msg(update);
        switch (msg.msg) {
            case MessageType::UpdateBoard:

            case MessageType::UpdateDamage:
            case MessageType::UpdateEnergy:
            case MessageType::UpdatePower:
            case MessageType::UpdateBonus:
            case MessageType::UpdateRobot:
            case MessageType::ActionMove:
            case MessageType::ActionAttack:
            case MessageType::ActionWait:
            case MessageType::ActionRadar:
                return "wait";
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