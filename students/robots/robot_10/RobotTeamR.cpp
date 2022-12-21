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


    //if there were radar info, we use those instead of the board info
    robotsAutour.insert(robotsAutour.end(), radarRobots.begin(), radarRobots.end());
    bonusAround.insert(bonusAround.end(), radarBonus.begin(), radarBonus.end());

    //we sort the positions to have the closest on top
    sort(robotsAutour.begin(), robotsAutour.end(), [](const Direction &a, const Direction &b) {
        return a.mag() < b.mag();
    });
    robotsAutour.erase(unique(robotsAutour.begin(), robotsAutour.end()), robotsAutour.end());
    sort(bonusAround.begin(), bonusAround.end(), [](const Direction &a, const Direction &b) {
        return a.mag() < b.mag();
    });
    bonusAround.erase(unique(bonusAround.begin(), bonusAround.end()), bonusAround.end());
    bool robotFound = !robotsAutour.empty();
    bool bonusFound = !bonusAround.empty();
    //bool radarInfo = !radarRobots.empty() || !radarBonus.empty();

    bool huntingMode = power >= MIN_POWER_HUNTING && energy >= MIN_ENERGY_HUNTING;

    //if we have a robot at one move from us, we escape to avoid unwanted collisions

    if (robotFound && floor(robotsAutour.at(0).mag()) == 1) {
        responseAction = escapeMove(robotsAutour.at(0), robotsAutour);
        if (not responseAction.empty()) {
            return responseAction;
        }
    }

    if (robotFound and huntingMode) {
        if (robotsAutour.at(0).mag() >= VISIBILITY) {
            lastMove = Message::actionMove(robotsAutour.at(0).unitary());
            numRandom = 0;
            return lastMove;
        }
    }
    if (robotFound and energy >= MIN_ENERGY_ATTACK and power >= MIN_POWER_ATTACK) {
        if (robotsAutour.front().mag() < VISIBILITY) {
            responseAction = Message::actionAttack(robotsAutour.front());
            return responseAction;
        }
    }
    //if there is a bonus to take, we go for it
    if (bonusFound) {

        for (auto bonus: bonusAround) {
            bool tooRisky = false;
            if (robotFound) {
                for (const auto &i: robotsAutour) {
                    //if there is another robot closer to the bonus than the robot or a robot on the case we want to go --> too risky
                    if (floor((i - bonus).mag()) <= bonus.mag()) {
                        tooRisky = true;
                        break;
                    }
                }

            }
            if (!tooRisky) {
                responseAction = Message::actionMove(bonus.unitary());
            }
            if (bonus.mag() >= VISIBILITY) {
                lastMove = responseAction;
                numRandom = 0;
            }
            return responseAction;

        }
    }


    if (robotFound) {

        responseAction = escapeMove(robotsAutour.front(), robotsAutour);
        if (not responseAction.empty()) {
            return responseAction;
        }
    }


    return randomAction();


}

string RobotTeamR::escapeMove(Direction dirEnemy, vector<Direction> robotsAutour) {
    Direction escapeMove = dirEnemy.unitary();
    escapeMove = escapeMove.neg();

    /*if (find(robotsAutour.begin(), robotsAutour.end(), escapeMove) == robotsAutour.end()) {
        return Message::actionMove(escapeMove);
    } else if (find(robotsAutour.begin(), robotsAutour.end(), escapeMove.rotate(M_PI / 2)) ==
               robotsAutour.end()) {
        return Message::actionMove(escapeMove);
    } else if (find(robotsAutour.begin(), robotsAutour.end(), escapeMove.rotate(-M_PI)) == robotsAutour.end()) {
        return Message::actionMove(escapeMove);
    } else if (robotsAutour.front().mag() < 3) {

        return Message::actionAttack(robotsAutour.front());
    }
    return "";*/
    if (!isBeside(escapeMove, robotsAutour)) {

        escapeMove = rotateRounded(-M_PI / 4.0, escapeMove);
        if (!isBeside(escapeMove, robotsAutour)) {

            escapeMove = rotateRounded(M_PI / 2.0, escapeMove);
            if (!isBeside(escapeMove, robotsAutour)) {
                if (robotsAutour.front().mag() < 3) {

                    return Message::actionAttack(robotsAutour.front());
                } else {
                    return "";
                }
            }
        }
    }
    return Message::actionMove(escapeMove);

}


string RobotTeamR::name() const {
    return "RobotTeamR";
}


void RobotTeamR::setConfig(size_t width_, size_t height_, unsigned int energy_, unsigned int power_) {
    this->width = width_;
    this->height = height_;
    this->energy = energy_;
    this->power = power_;

}


std::string RobotTeamR::randomMove() {
    if (lastMove.empty() or numRandom > 5) {
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
    std::uniform_int_distribution<int> action(0, 8);
    //choose random action 0 --> radar, else randomMove (to avoid making using too much actionRadar)
    switch (action(mt)) {
        case 0 :
            return Message::actionRadar();
        default:
            return randomMove();
    }
}

bool RobotTeamR::isBeside(Direction dir, const vector<Direction> &robotsAutour) {
    for (const auto &i: robotsAutour) {
        if (floor((dir - i).mag()) <= 1) {
            return false;
        }
    }

    return true;
}

Direction RobotTeamR::rotateRounded(double angle, Direction dir) {
    double newX = dir.getdX() * cos(angle) - dir.getdY() * sin(angle);
    double newY = dir.getdX() * sin(angle) + dir.getdY() * cos(angle);

    return {int(round(newX)), int(round(newY))};

}