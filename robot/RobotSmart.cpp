#define _USE_MATH_DEFINES
#include "RobotSmart.h"
#include <algorithm>
#include <cmath>
#include <librobots/Direction.h>
#include <librobots/Message.h>
#include <librobots/Robot.h>
#include <random>

using namespace std;

constexpr unsigned START_TRACE_ROBOT = 20;

void RobotSmart::updateNearest(Direction dir, bool bonus) {
    double distanceNearest = nearestDirection.mag(), distance = dir.mag();
    if (distance <= 0.5) { return; }
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

string RobotSmart::fleeFrom(Direction dir) {
    random_device rd;
    uniform_int_distribution<int> val(-10, 10);
    return Message::actionMove(dir.neg().rotate(M_PI_2 * val(rd) / 10));
}

void normalize(vector<Direction> &list) {
    sort(list.begin(), list.end(), [](Direction a, Direction b) { return a.mag() < b.mag(); });
    list.erase(unique(list.begin(), list.end()), list.end());
    while (!list.empty() && list.front().mag() == 0) { list.erase(list.begin()); }
}

string RobotSmart::action(std::vector<std::string> updates) {
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

    normalize(robots);
    normalize(boni);

    if (!boni.empty()) { updateNearest(boni.at(0), true); }
    if (!robots.empty()) {
        for (auto robot: robots) {
            if (robot.mag() < 2) {
                lastAttack = 0;
                return fleeFrom(robot);
            } else if (robot.mag() <= 3) {
                if (energy >= 10) {
                    lastAttack = 0;
                    return Message::actionAttack(robot);
                } else {
                    lastAttack = 0;
                    return fleeFrom(robot);
                }
            }
        }
        if (nearestCounter == 0) { updateNearest(robots.at(0)); }
    }

    if (lastAttack >= START_TRACE_ROBOT && energy > 10) {
        switch (lastAttack) {
            case START_TRACE_ROBOT:
                traceRobots.clear();
                return Message::actionRadar();
            case START_TRACE_ROBOT + 1:
                traceRobots.push_back(robots);
                return Message::actionRadar();
            case START_TRACE_ROBOT + 2:
                traceRobots.push_back(robots);
                {
                    Direction pos = traceRobots.at(1).at(0);
                    Direction move = pos - traceRobots.at(0).at(0);
                    unsigned steps = 0;
                    while (pos.mag() > double(steps++)) {
                        pos += move;
                        pos = Direction(int((size_t(pos.getdX()) + width) % width), int((size_t(pos.getdY()) + height) % height));
                    }
                    updateNearest(pos);
                }
                break;
            default:
                break;
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

[[nodiscard]] string RobotSmart::name() const { return "Smart one"; }