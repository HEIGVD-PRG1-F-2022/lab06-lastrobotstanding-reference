/**
 * @author : Rachel Tranchida & Rafael Dousse
 * @date : 29.11.2022
 * @file : RobotTeamR
 * @project : Refactor
 */
#include "RobotTeamR.h"
#include "librobots/Message.h"
#include <random>

using namespace std;

std::ostream &operator<<(std::ostream &os, const std::vector<Direction> &positions) {
    for (const auto &i: positions) {
        os << i << ", ";
    }
    return os;
}

string RobotTeamR::action(vector<string> updates) {


    //cout<< endl << "Début de robot" << endl;

    string responseAction = "move 1,1";
    vector<string> paramsDamage;

    //Direction opposite;
    // Direction moveDir(1, 1);

    vector<Direction> robotsAutour;
    vector<Direction> damageReceived;
    vector<Direction> bonusAround;
    vector<Direction> robotAround;
    vector<Direction> radarRobots;
    vector<Direction> radarBonus;


    for (const auto &info: updates) {
        Message mess(info);


        switch (mess.msg) {
            case MessageType::UpdateBoard:


                robotsAutour.insert(robotsAutour.end(), mess.robots.begin(), mess.robots.end());
                bonusAround.insert(bonusAround.end(), mess.boni.begin(), mess.boni.end());


                break;
            case MessageType::UpdateDamage:

                damageReceived.insert(damageReceived.end(), mess.robots.begin(), mess.robots.end());
                energy -= mess.energy;

                break;
            case MessageType::UpdateEnergy:
                energy += mess.energy;
                break;
            case MessageType::UpdatePower:
                power += mess.power;
                break;
            case MessageType::UpdateBonus:
                radarBonus.insert(radarBonus.end(), mess.boni.begin(), mess.boni.end());
                break;
            case MessageType::UpdateRobot:
                radarRobots.insert(radarRobots.end(), mess.robots.begin(), mess.robots.end());
                break;
            default:
                break;

        }

    }

    /*Proposition stratégie
     *
     * TODO
     * Priorité BON
     *
     *
     *
     */

    //if there were radar info, we use those instead of the board info
    if (!radarRobots.empty()) {
        robotsAutour = radarRobots;
    }
    if (!radarBonus.empty()) {
        bonusAround = radarBonus;
    }

    //we sort the positions to have the closest on top
    sort(robotsAutour.begin(), robotsAutour.end(), [](const Direction &a, const Direction &b) {
        return a.mag() < b.mag();
    });
    sort(bonusAround.begin(), bonusAround.end(), [](const Direction &a, const Direction &b) {
        return a.mag() < b.mag();
    });
    bool robotFound = !robotsAutour.empty();
    bool bonusFound = !bonusAround.empty();
    //bool radarInfo = !radarRobots.empty() || !radarBonus.empty();

    //if there is a bonus to take, we go for it
    if (bonusFound) {

        for (auto bonus: bonusAround) {
            bool tooRisky = false;
            if (robotFound) {
                for (const auto &i: robotsAutour) {
                    //if there is another robot closer to the bonus than the robot or a robot on the case we want to go --> too risky
                    if ((i - bonus).mag() <= bonus.mag()) {
                        tooRisky = true;
                        break;
                    }
                }

            }
            if (!tooRisky) {
                responseAction = Message::actionMove(bonus.unitary());
                return responseAction;
            }
        }
    }
    //cout << "closestBonus : " << closestBonus << endl;

    if (energy >= 12 and power >= 3 and robotFound) {
        if (robotsAutour.front().mag() < 3) {
            responseAction = Message::actionAttack(robotsAutour.front());
            return responseAction;
        }


    } else {
        if (robotFound) {
            Direction escapeMove = robotsAutour.front().unitary();
            escapeMove = escapeMove.neg();

            if (find(robotsAutour.begin(), robotsAutour.end(), escapeMove) == robotsAutour.end()) {
                return Message::actionMove(escapeMove);
            } else if (find(robotsAutour.begin(), robotsAutour.end(), escapeMove.rotate(M_PI / 2)) ==
                       robotsAutour.end()) {
                return Message::actionMove(escapeMove);
            } else if (find(robotsAutour.begin(), robotsAutour.end(), escapeMove.rotate(-M_PI)) == robotsAutour.end()) {
                return Message::actionMove(escapeMove);
            } else if (robotsAutour.front().mag() < 3) {

                return Message::actionAttack(robotsAutour.front());
            }


        }
    }

    return randomAction();


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


std::string RobotTeamR::randomMove() {
    if (lastMove.empty() or numRandom > 3) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dir(-1, 1);
        lastMove = Message::actionMove(Direction(dir(mt), dir(mt)));
        if (lastMove == Message::actionMove(Direction(0, 0))) {
            numRandom = 4;
            lastMove = Message::actionWait();
        } else {
            numRandom = 0;
        }

        return lastMove;
    }
    ++numRandom;
    return lastMove;


}

std::string RobotTeamR::randomAction() {

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> action(0, 5);
    //choose random action 0 --> radar, else randomMove (to avoid making using too much actionRadar)
    switch (action(mt)) {
        case 0 :
            return Message::actionRadar();
        default:
            return randomMove();
    }
}
