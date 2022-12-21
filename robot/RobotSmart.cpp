#define _USE_MATH_DEFINES
#include "RobotSmart.h"
#include <algorithm>
#include <cmath>
#include <librobots/Direction.h>
#include <librobots/Message.h>
#include <librobots/Robot.h>

using namespace std;

void RobotSmart::updateNearest(Direction dir, bool bonus) {
    double distanceNearest = nearestDirection.mag(), distance = dir.mag();
    if (distance <= 0.5) { throw runtime_error("Nearest with distance 0"); }
    if (distance + nearestCounter < distanceNearest || nearestCounter == 0 || bonus) {
        //            cout << "Updating distance with " << dir << endl;
        nearestDirection = dir;
        nearestCounter = bonus ? unsigned(width) : 10;
    }
}

void RobotSmart::setConfig(size_t init_width, size_t init_height, unsigned init_energy, unsigned init_power) {
    width = init_width;
    height = init_height;
    energy = init_energy;
    power = init_power;
}

std::string RobotSmart::action(std::vector<std::string> updates) {
    vector<Direction> robots, boni;
    ++lastAttack;
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

    std::sort(robots.begin(), robots.end(), [](Direction a, Direction b) { return a.mag() < b.mag(); });
    robots.erase(std::unique(robots.begin(), robots.end()), robots.end());
    if (robots.size() > 0 && robots.front().mag() == 0) { robots.erase(robots.begin()); }
    std::sort(boni.begin(), boni.end(), [](Direction a, Direction b) { return a.mag() < b.mag(); });
    boni.erase(std::unique(boni.begin(), boni.end()), boni.end());

    if (!boni.empty()) { updateNearest(boni.at(0), true); }
    if (!robots.empty()) {
        if (energy < 10) {
            //                cout << "Going away from " << robots.at(0) << endl;
            lastAttack = 0;
            return Message::actionMove(robots.at(0).neg().rotate(M_PI / 2));
        }
        for (auto robot: robots) {
            if (robot.mag() < 2) {
                lastAttack = 0;
                return Message::actionMove(robot.neg().rotate(M_PI / 2));
            } else if (robot.mag() <= 3) {
                lastAttack = 0;
                return Message::actionAttack(robot);
            }
        }
        if (nearestCounter == 0) {
//            if (lastAttack >= 50) {
//                if (lastAttack == 50) {
//                    traceRobots.clear();
//                } else if (lastAttack < 60) {
//                    traceRobots.push_back(robots);
//                    return Message::actionRadar();
//                }
//            }
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

[[nodiscard]] std::string RobotSmart::name() const { return "Smart one"; }