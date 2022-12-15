#define _USE_MATH_DEFINES
#include <cmath>
#include "RobotSmart.h"
#include <algorithm>
#include <librobots/Direction.h>
#include <librobots/Message.h>
#include <librobots/Robot.h>

using namespace std;

void RobotSmart::updateNearest(Direction dir, bool bonus) {
    double distanceNearest = nearestDirection.mag(),
           distance = dir.mag();
    if (distance <= 0.5) {
        throw runtime_error("Nearest with distance 0");
    }
    if (distance + nearestCounter < distanceNearest ||
        nearestCounter == 0 ||
        bonus) {
        //            cout << "Updating distance with " << dir << endl;
        nearestDirection = dir;
        nearestCounter = bonus ? unsigned(width) : 10;
    }
}

void RobotSmart::setConfig(size_t init_width, size_t init_height, unsigned init_energy,
                           unsigned init_power) {
    width = init_width;
    height = init_height;
    energy = init_energy;
    power = init_power;
}

std::string RobotSmart::action(std::vector<std::string> updates) {
    vector<Direction> robots, boni;
    //        for (const auto &update: updates){
    //            cout << name() << " Update is: " << update << endl;
    //        }
    for (const auto &update: updates) {
        Message u(update);
        robots.insert(robots.end(), u.robots.begin(), u.robots.end());
        boni.insert(boni.end(), u.boni.begin(), u.boni.end());
        switch (u.msg) {
            case MessageType::UpdateBoard:
                break;
            case MessageType::UpdateDamage:
                energy -= u.energy;
                robots.push_back(u.robots.at(0));
                break;
            case MessageType::UpdateEnergy:
                energy += u.energy;
                break;
            case MessageType::UpdatePower:
                power += u.power;
                break;
            case MessageType::UpdateBonus:
                break;
            case MessageType::UpdateRobot:
                break;
            default:
                break;
        }
    }

    if (!boni.empty()) {
        sort(boni.begin(), boni.end(), [](Direction a, Direction b) -> bool { return a.mag() < b.mag(); });
        updateNearest(boni.at(0), true);
    }
    if (!robots.empty()) {
        sort(robots.begin(), robots.end(), [](Direction a, Direction b) -> bool { return a.mag() < b.mag(); });
        if (energy < 10) {
            //                cout << "Going away from " << robots.at(0) << endl;
            return Message::actionMove(robots.at(0).neg());
        }
        for (auto robot: robots) {
            if (robot.mag() < 2) {
                return Message::actionMove(robot.neg().rotate(M_PI / 2));
            } else if (robot.mag() <= 3) {
                return Message::actionAttack(robot);
            }
        }
        if (nearestCounter == 0) {
            updateNearest(robots.at(0));
        }
    }

    if (nearestCounter > 0 && nearestDirection.mag() > 0) {
        --nearestCounter;
        Direction dir = nearestDirection.unitary();
        //            cout << "Following before " << nearestDirection << " - " << dir << endl;
        nearestDirection -= dir;
        //            cout << "Following after " << nearestDirection << " - " << dir << endl;
        return Message::actionMove(dir);
    } else {
        nearestCounter = 0;
        return Message::actionRadar();
    }
};

[[nodiscard]] std::string RobotSmart::name() const {
    return "Smart one";
}