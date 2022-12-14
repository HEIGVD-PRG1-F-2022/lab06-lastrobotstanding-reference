/**
 * @author : Rachel Tranchida & Rafael Dousse
 * @date : 29.11.2022
 * @file : RobotTeamR
 * @project : Refactor
 */
#include "RobotTeamR.h"
#include "librobots/Message.h"
#include  <chrono>
#include <thread>
#include <algorithm>

using namespace std;
using namespace this_thread;
using namespace chrono;


string RobotTeamR::action(vector<string> updates) {


    string responseAction = "wait";
    vector<string> paramsDamage;
    Direction opposite;
    vector<Direction> robotsAutour;
    vector<double> robotMag;
    Direction moveDir(1, 1);
    for (const auto &info: updates) {

        Message mess(info);


        switch (mess.msg) {
            case MessageType::UpdateBoard:
                cout << "updateBoard" << endl;


                robotsAutour.insert(robotsAutour.begin(), mess.robots.begin(), mess.robots.end());

                break;
            case MessageType::UpdateDamage:
                cout << "updateDamage" << endl;
                energy -= mess.energy;
                if (energy < 5) {
                    opposite = mess.robots.at(0).neg();
                    responseAction = Message::actionMove(opposite);
                } else {
                    responseAction = Message::actionAttack(mess.robots.at(0));
                }

                break;
                //que pour le bonus:
            case MessageType::UpdateEnergy:
                cout << "updatEnergy" << endl;
                break;
            case MessageType::UpdatePower:
                cout << "UpdatePower" << endl;
                break;
            case MessageType::UpdateBonus:
                cout << "UpdateBonus" << endl;
                break;
            case MessageType::UpdateRobot:
                cout << "updateRobot" << endl;
                break;

        }

    }

    for (auto robot: robotsAutour) {
        cout << "Robot : " << robot << endl;
        cout << endl;
        robotMag.push_back(robot.mag());
        if ((energy > 5) and (robot.mag() < 3)) {

        }

    }
    for (auto mag: robotMag) {
        cout << "mag : " << mag << endl;
    }

    sort(robotMag.begin(), robotMag.end());

    for (auto mag: robotMag) {
        cout << "mag sorted : " << mag << endl;
    }
    if(robotMag.at(0) == 1 ){
        responseAction = Message::actionAttack(robotsAutour.at(0));
    }


return responseAction;


}

string RobotTeamR::name() const {
    return "RobotTeamR";
}


void RobotTeamR::setConfig(size_t width, size_t height, unsigned int energy, unsigned int power) {
    this->width = width;
    this->height = height;
    this->energy = energy;
    this->power = power;

}

size_t RobotTeamR::getEnergy() const {
    cout << "Energy du robot : ";
    return energy;
}

size_t RobotTeamR::getPower() const {
    cout << "Power du robot : ";
    return power;
}

