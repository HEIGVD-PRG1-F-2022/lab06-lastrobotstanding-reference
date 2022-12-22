//
// Created by emily on 08.12.2022.
//
#include "MyRobot.h"
#include <string>

using namespace std;

void MyRobot::setConfig(size_t width_init, size_t height_init, unsigned energy_init,
                        unsigned power_init) {
    width = width_init;
    height = height_init;
    energy = energy_init;
    power = power_init;
    lastPower = power;
    lastEnergy = energy;
    countRound = 0;
}

[[nodiscard]] string MyRobot::name() const {
    return "MCR";
}

string MyRobot::action(std::vector<std::string> updates) {
    countRound++;
    std::vector<Direction> robots, boni;
    Direction directionDamage;
    for (const auto &update: updates) {
        Message m(update);
        switch (m.msg) {
            case MessageType::UpdateBoard:
                setMemoryList(m.robots, robotsList);
                setMemoryList(m.boni, boniList);
                break;
            case MessageType::UpdateDamage:
                energy -= m.energy;
                break;
            case MessageType::UpdateEnergy:
                energy += m.energy;
                break;
            case MessageType::UpdatePower:
                power += m.power;
                break;
            case MessageType::UpdateBonus:
                setMemoryList(m.boni, boniList);
                break;
            case MessageType::UpdateRobot:
                setMemoryList(m.robots, robotsList);
                break;
            default :
                break;
        }
    }
    Direction ClosestRobot = getClosest(robotsList);
    Direction ClosestRobotInverse = ClosestRobot.neg();
    Direction ClosestBonus = getClosest(boniList);
    double dClosestRobot = ClosestRobot.mag();
    double dClosestBonus = ClosestBonus.mag();

    int radarThreshold = 0;
    switch (energy) {
        case 1 ... 3:
            radarThreshold = 32;
            break;
        case 4 ... 7:
            radarThreshold = 16;
            break;
        default:
            radarThreshold = 8;
            break;
    }
    if (countRound >= radarThreshold and robots.empty()) {
        countRound = 0;
        robotsList.clear();
        return Message::actionRadar();
    }

    if (power != lastPower and energy > lastEnergy) {
        boniList.clear();
        lastEnergy = energy;
        lastPower = power;
    }
    if (robotsList.empty()) {
        return Message::actionRadar();
    } else {
        if (dClosestRobot * 2.0 < dClosestBonus) {
            if (ClosestRobot.mag() <= 2.82843) {
                lastMove = Direction(0, 0);
                return Message::actionAttack(ClosestRobot.neg());
            }
            //Chase the robot
            if (energy < 6) {
                lastMove = ClosestRobot.unitary();
                return Message::actionMove(lastMove);
            }
            lastMove = ClosestRobot.unitary().neg();
            return Message::actionMove(lastMove);
        } else {
            lastMove = ClosestBonus.unitary();
            return Message::actionMove(ClosestBonus.unitary());
        }
    }
}

void MyRobot::setMemoryList(vector<Direction> listShortTerm, vector<Direction> &listLongTerm) {
    for (auto i: listShortTerm) {
        if (!isDuplicate(i, listLongTerm)) {
            // robot list of the current turn contains an element if that is already present in the robot's long term memory
            listLongTerm.push_back(i);
        }
    }
    for (auto i: listLongTerm) {
        if (isMissing(i, listShortTerm) and (i.mag() <= 2.82843)) {
            std::remove(listLongTerm.begin(), listLongTerm.end(), i);
        }
    }
}

string MyRobot::moveTowards(Direction target) {
    return Message::actionMove(target.unitary());
}

Direction MyRobot::getRandomDirection() {
    int randX = 0;
    int randY = 0;
    do {
        randX = -1 + (std::rand() % (1 - (-1) + 1));
        randY = -1 + (std::rand() % (1 - (-1) + 1));
    } while (randX == 0 and randY == 0);
    Direction d(randX * 10, randY * 10);
    return d;
}

Direction MyRobot::getClosest(vector<Direction> directions) {
    if (directions.empty()) {
        return getRandomDirection();
    } else {
        vector<double> magnitudes = {};
        Direction min = directions.at(0);
        for (auto i: directions) {
            magnitudes.push_back(i.mag());
        }
        double smallestMag = *min_element(magnitudes.begin(), magnitudes.end());
        return directions.at(getIndex(magnitudes, smallestMag));
    }
}

int MyRobot::getIndex(vector<double> v, double K) {
    auto it = find(v.begin(), v.end(), K);

    int index = it - v.begin();
    return index;
}

//si duplicate, remove, else, keep
bool MyRobot::isDuplicate(Direction direction, vector<Direction> directions) {
    for (auto i: directions) {
        if (direction == i) {
            return true;
        }
    }
    return false;
}

bool MyRobot::isMissing(Direction direction, vector<Direction> directions) {
    if (std::find(directions.begin(), directions.end(), direction) != directions.end()) {
        return false;
    } else {
        return true;
    }
}

void MyRobot::updatePositionObject(vector<Direction> &objectType, Direction lastMove) {
    for (auto i: objectType) {
        i = Direction(i.getdX() - lastMove.getdX(), i.getdY() - lastMove.getdY());
    }
}