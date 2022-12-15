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
                cout << "updateBoard" << endl;


                robotsAutour.insert(robotsAutour.begin(), mess.robots.begin(), mess.robots.end());
                bonusAround.insert(bonusAround.begin(), mess.boni.begin(), mess.boni.end());


                break;
            case MessageType::UpdateDamage:
                cout << "updateDamage" << endl;

                damegeReceived.insert(damegeReceived.begin(), mess.robots.begin(), mess.robots.end());

                energy -= mess.energy;
                cout << "energy : " << energy << endl;



                break;
                //que pour le bonus:
            case MessageType::UpdateEnergy:
                cout << "updateEnergy" << endl;
                energy += mess.energy;
                break;
            case MessageType::UpdatePower:
                cout << "UpdatePower" << endl;
                power += mess.power;
                break;
            case MessageType::UpdateBonus:
                cout << "UpdateBonus" << endl;
                break;
            case MessageType::UpdateRobot:
                cout << "updateRobot" << endl;
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
        cout << "There's a bonus!! en position :" << closeBonus << endl;
        responseAction = Message::actionMove(closeBonus.unitary());
    }

    if(!damegeReceived.empty()){
        Direction closeRobot = *min_element(robotsAutour.begin(), robotsAutour.end(), [](Direction a, Direction b) {
            return a.mag() < b.mag();
        });

        for(auto &d : damegeReceived){
            cout << "Damage received autour: " << d << endl;
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
            cout << "Robot autour: " << r << endl;
        }
        Direction closeRobot = *min_element(robotsAutour.begin(), robotsAutour.end(), [](Direction a, Direction b) {
            return a.mag() < b.mag();
        });


        for (auto robot: robotsAutour) {
            cout << "Robot : " << robot << endl;
            cout << endl;


            if ((energy > 5) and (robot.mag() < 3.0)) {
                cout << "I'm attacking : " << closeRobot << endl;
                responseAction = Message::actionAttack(closeRobot);


            } else {
                cout << "close " << closeRobot << endl;
                //  cout << "close robot with unitary and neg " << closeRobot.neg().unitary() << endl;
                responseAction = Message::actionMove(closeRobot.unitary().neg());
                // cout << "close robot with unitary and neg " << closeRobot.neg().unitary() << endl;
                cout << "Response " << responseAction << endl;
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
    cout << "Energy du robot : ";
    return energy;
}

size_t RobotTeamR::getPower() const {
    cout << "Power du robot : ";
    return power;
}

