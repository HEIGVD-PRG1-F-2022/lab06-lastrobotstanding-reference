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


    string responseAction = "move 1,1";
    vector<string> paramsDamage;

    Direction opposite;
    Direction moveDir(1, 1);

    vector<Direction> robotsAutour;
    vector<Direction> damegeReceived;
    vector<Direction> bonusAround;


    for (const auto &info: updates) {
        Message mess(info);

        robotsAutour.clear();
        switch (mess.msg) {
            case MessageType::UpdateBoard:
// << "updateBoard" << endl;


                robotsAutour.insert(robotsAutour.begin(), mess.robots.begin(), mess.robots.end());
                bonusAround.insert(bonusAround.begin(), mess.boni.begin(), mess.boni.end());


                break;
            case MessageType::UpdateDamage:
// << "updateDamage" << endl;

                damegeReceived.insert(damegeReceived.begin(), mess.robots.begin(), mess.robots.end());

                energy -= mess.energy;
// << "energy : " << energy << endl;



                break;
                //que pour le bonus:
            case MessageType::UpdateEnergy:
// << "updateEnergy" << endl;
                energy += mess.energy;
                break;
            case MessageType::UpdatePower:
// << "UpdatePower" << endl;
                power += mess.power;
                break;
            case MessageType::UpdateBonus:
// << "UpdateBonus" << endl;
                break;
            case MessageType::UpdateRobot:
// << "updateRobot" << endl;
                break;
            default:
                break;

        }

    }

    /*
     *
     *
     *
     */

    if(!bonusAround.empty()){
        Direction closeBonus = *min_element(bonusAround.begin(), bonusAround.end(), [](Direction a, Direction b) {
            return a.mag() < b.mag();
        });
// << "There's a bonus!! en position :" << closeBonus << endl;
        responseAction = Message::actionMove(closeBonus.unitary());
    }

    if(!damegeReceived.empty()){
        Direction closeRobot = *min_element(robotsAutour.begin(), robotsAutour.end(), [](Direction a, Direction b) {
            return a.mag() < b.mag();
        });

        for(auto &d : damegeReceived){
// << "Damage received autour: " << d << endl;
        }

      /*  if (energy < 5) {
            opposite = mess.robots.at(0).neg();
            responseAction = Message::actionMove(opposite);
        } else {
            responseAction = Message::actionAttack(mess.robots.at(0));
        }*/
    }


    if (!robotsAutour.empty()) {
        for(auto &r : robotsAutour){
// << "Robot autour: " << r << endl;
        }
        Direction closeRobot = *min_element(robotsAutour.begin(), robotsAutour.end(), [](Direction a, Direction b) {
            return a.mag() < b.mag();
        });


        for (auto robot: robotsAutour) {
// << "Robot : " << robot << endl;
// << endl;


            if ((energy > 5) and (robot.mag() < 3.0)) {
// << "I'm attacking : " << closeRobot << endl;
                responseAction = Message::actionAttack(closeRobot);


            } else {
// << "close " << closeRobot << endl;
// << "close robot with unitary and neg " << closeRobot.neg().unitary() << endl;
                responseAction = Message::actionMove(closeRobot.unitary().neg());
// << "close robot with unitary and neg " << closeRobot.neg().unitary() << endl;
// << "Response " << responseAction << endl;
            }
        }

    }


    return responseAction;


}


void responseDamage(){

}
void responseRobot(){

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
// << "Energy du robot : ";
    return energy;
}

size_t RobotTeamR::getPower() const {
// << "Power du robot : ";
    return power;
}

