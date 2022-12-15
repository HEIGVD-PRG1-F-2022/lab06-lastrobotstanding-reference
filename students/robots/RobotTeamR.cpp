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


    //string responseAction = "move 1,1";
    string responseAction = "move 1,1";
    vector<string> paramsDamage;
    Direction opposite;
    vector<Direction> robotsAutour;
    Direction moveDir(1, 1);
    for (const auto &info: updates) {
        Message mess(info);

        robotsAutour.clear();
        switch (mess.msg) {
            case MessageType::UpdateBoard:
//                cout << "updateBoard" << endl;


                robotsAutour.insert(robotsAutour.begin(), mess.robots.begin(), mess.robots.end());


                break;
            case MessageType::UpdateDamage:
//                cout << "updateDamage" << endl;

                energy -= mess.energy;
//                cout << "energy : " << energy << endl;
                if (energy < 5) {
                    opposite = mess.robots.at(0).neg();
                    responseAction = Message::actionMove(opposite);
                } else {
                    responseAction = Message::actionAttack(mess.robots.at(0));
                }
                break;
                //que pour le bonus:
            case MessageType::UpdateEnergy:
//                cout << "updateEnergy" << endl;
                break;
            case MessageType::UpdatePower:
//                cout << "UpdatePower" << endl;
                break;
            case MessageType::UpdateBonus:
//                cout << "UpdateBonus" << endl;
                break;
            case MessageType::UpdateRobot:
//                cout << "updateRobot" << endl;
                break;
            default:
                break;

        }

    }
    if (!robotsAutour.empty()) {
//        for(auto &r : robotsAutour){
//            cout << "Robot autour: " << r << endl;
//        }
        Direction closeRobot = *min_element(robotsAutour.begin(), robotsAutour.end(), [](Direction a, Direction b) {
            return a.mag() < b.mag();
        });


        for (auto robot: robotsAutour) {
//            cout << "Robot : " << robot << endl;
//            cout << endl;


            if ((energy > 5) and (robot.mag() < 3.0)) {
//                cout << "I'm attacking : " << closeRobot << endl;
                responseAction = Message::actionAttack(closeRobot);


            } else {
//                cout << "close " << closeRobot << endl;
                //  cout << "close robot with unitary and neg " << closeRobot.neg().unitary() << endl;
                responseAction = Message::actionMove(closeRobot.unitary().neg());
                // cout << "close robot with unitary and neg " << closeRobot.neg().unitary() << endl;
//                cout << "Response " << responseAction << endl;
            }
        }

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
//    cout << "Energy du robot : ";
    return energy;
}

size_t RobotTeamR::getPower() const {
//    cout << "Power du robot : ";
    return power;
}

