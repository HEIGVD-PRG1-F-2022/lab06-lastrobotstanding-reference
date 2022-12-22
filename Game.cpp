#include "Game.h"
#include <algorithm>
#include <libdio/display.h>
#include <librobots/Bonus.h>
#include <librobots/Message.h>
#include <librobots/RobotState.h>
#include <random>
#include <thread>
#include <vector>

using namespace std;

long Game::robotsAlive() {
    return count_if(robots.begin(), robots.end(), [](const RobotState &r) -> bool { return !r.isDead(); });
}

size_t Game::largestRobotAlive() {
    vector<size_t> alive;
    for (size_t i = 1; const auto &robot: robots) {
        if (!robot.isDead()) { alive.push_back(i); }
        ++i;
    }
    return *max_element(alive.begin(), alive.end());
}

void Game::waitListToArena() {
    if (waitList.empty()) { throw runtime_error("No robot waiting for the arena"); }
    side = size_t(10 * sqrt(waitList.size()));
    for (auto robot: waitList) {
        robots.emplace_back(robot, Position::random(unsigned(side), unsigned(side), side, side), side, ENERGY_INIT, POWER_INIT);
    }
}

void Game::actionAttack() {
    for (const auto &attacker: robots) {
        if (attacker.isDead()) { continue; }
        Message act = attacker.getAction();
        if (act.msg == MessageType::ActionAttack) {
            auto destination = attacker.getPosition() + act.robots.at(0);
            for (auto &defender: robots) {
                if (defender.isDead()) { continue; }
                if (destination == defender.getPosition()) {
                    defender.actionAttack(attacker, destination);
                    idle = 0;
                }
            }
        }
    }
}

void Game::actionMove() {
    for (auto &robot: robots) {
        if (robot.isDead()) { continue; }
        Message act = robot.getAction();
        if (act.msg == MessageType::ActionMove) { robot.actionMove(act.robots.at(0).unitary()); }
    }
    for (size_t i = 0; i < robots.size(); i++) {
        for (size_t j = 0; j < robots.size(); j++) {
            if (i == j) { continue; }
            if (robots.at(i).isDead() || robots.at(j).isDead()) { continue; }
            robots.at(i).checkCollision(robots.at(j));
        }
    }
    positions.clear();
    for (const auto &robot: robots) {
        if (robot.isDead()) { continue; }
        positions.push_back(robot.getPosition());
    }
}

void Game::actionRadar() {
    radar.clear();
    for (auto &robot: robots) {
        if (robot.isDead()) { continue; }
        Message act = robot.getAction();
        if (act.msg == MessageType::ActionRadar) {
            if (standard) {
                robot.actionRadar(positions);
            } else {
                vector<Position> boniPos(boni.size());
                transform(boni.begin(), boni.end(), boniPos.begin(), [](const Bonus &b) { return b.pos; });
                robot.actionRadar(positions, boniPos);
            }
            radar.push_back(robot.getPosition());
        }
    }
}

void Game::createBonus() {
    if (bonusTimeout-- == 0) {
        bonusTimeout = BONUS_RECURRENCE / unsigned(robotsAlive());
        std::random_device rd;
        std::uniform_int_distribution<int> type(0, 1);
        int newType = type(rd);
        Bonus *newBonus;
        do {
            if (newType == 0) {
                newBonus = new Bonus(side, side, 10, BonusType::Energy);
            } else {
                newBonus = new Bonus(side, side, 3, BonusType::Power);
            }
        } while (find_if(boni.begin(), boni.end(), [newBonus](Bonus b) -> bool { return b.pos == newBonus->pos; }) != boni.end());
        for (auto &robot: robots) {
            if (robot.isDead()) { continue; }
            robot.actionBonus(newBonus->pos);
        }
        boni.push_back(*newBonus);
    }
}

void Game::checkBonus() {
    for (auto &robot: robots) {
        if (robot.isDead()) { continue; }
        auto b = find_if(boni.begin(), boni.end(), [robot](Bonus b) -> bool { return b.pos == robot.getPosition(); });
        if (b != boni.end()) {
            if (!standard) { idle = 0; }
            switch (b->type) {
                case BonusType::Energy:
                    robot.actionEnergy(b->value);
                    break;
                case BonusType::Power:
                    robot.actionPower(b->value);
                    break;
            }
            boni.erase(b);
        }
    }
}

void Game::sendUpdates(bool debug) {
    vector<Position> boniPos(boni.size());
    transform(boni.begin(), boni.end(), boniPos.begin(), [](Bonus b) -> Position { return b.pos; });
    auto updateBoards = Message::updateBoard(positions, boniPos);
    for (auto &robot: robots) {
        if (robot.isDead()) { continue; }
        robot.actionRadar(radar);
        debug && cout << "Sending updates to " << robot.getName() << endl;
        robot.sendUpdate(updateBoards.front());
        debug && cout << "Done with " << robot.getName() << endl;
        updateBoards.erase(updateBoards.begin());
    }
}

void Game::display() {
    vector<vector<Display::DString>> grid(side, vector<Display::DString>(side));
    for (int robotNbr = 0; const auto &robot: robots) {
        robotNbr++;
        if (robot.isDead()) { continue; }
        auto pos = robot.getPosition();
        Display::DString robotStr;
        switch (robot.getAction().msg) {
            case MessageType::ActionMove:
                robotStr.setColor(Display::Color::GREEN);
                break;
            case MessageType::ActionAttack:
                robotStr.setColor(Display::Color::RED);
                break;
            case MessageType::ActionRadar:
                robotStr.setColor(Display::Color::YELLOW);
                break;
            case MessageType::ActionWait:
                robotStr.setColor(Display::Color::BLUE);
                break;
            default:
                break;
        }
        grid.at(size_t(pos.getY())).at(size_t(pos.getX())) = robotStr << robotNbr;
    }
    for (const auto &bonus: boni) {
        grid.at(size_t(bonus.pos.getY())).at(size_t(bonus.pos.getX())) = Display::DString(Display::Color::YELLOW) << "B";
    }

    Display::DString().cursorHome().print();
    Display::displayGrid(grid, false).print();

    displayRobotsStats((to_string(largestRobotAlive()).size() + 1) * side + 5, 1);
}

void Game::displayRobotsStats(size_t x, size_t y) {
    cout << (Display::DString().cursorPosition(x, y) << "Round: " << round << " Idle for: " << idle)
                    .cursorDelete(Display::DString::LineDelete::TO_END)
         << endl;
    for (size_t i = 1; const auto &robot: robots) {
        Display::DString str;
        str.cursorPosition(x, y + i + 1);
        if (robot.isDead()) {
            cout << (str.setColor(Display::Color::RED) << i++ << " - Robot: " << robot.getName() << " - RIP: " << robot.getDeathCause());
        } else {
            cout << (str.setColor(Display::Color::GREEN)
                     << i++ << " - Robot: " << robot.getName() << " - Energy: " << robot.getEnergy() << " - Power: " << robot.getPower()
                     << " - action: " << robot.getAction().getMessageType());
        }
        cout << Display::DString().cursorDelete(Display::DString::LineDelete::TO_END) << endl;
    }
}

void Game::addRobot(Robot *r) {
    if (!robots.empty()) { throw runtime_error("Play already started!"); }
    waitList.push_back(r);
}

RobotState *Game::play(bool show, bool debug) {
    if (show) {
        Display::init();
        Display::DString().clearScreen().cursorVisible(false).print();
    }
    waitListToArena();
    do {
        size_t largestAlive = largestRobotAlive();
        actionAttack();
        actionMove();
        actionRadar();
        createBonus();
        checkBonus();
        if (show) {
            if (largestRobotAlive() < largestAlive) { Display::DString().clearScreen().print(); }
            display();
            this_thread::sleep_for(1000ms / pow(log(round + 2), 2));
        }
        sendUpdates(debug);
        if (standard) { idle_limit = unsigned(100 * robotsAlive()); }
    } while (robotsAlive() > 1 && ++idle < idle_limit && ++round < MAX_ROUNDS);

    if (debug) {
        for (const auto &robot: robots) { cout << robot.getName() << ": " << robot.getDeathCause() << endl; }
    }

    if (robotsAlive() == 1) {
        return &*find_if(robots.begin(), robots.end(), [](const RobotState &r) -> bool { return !r.isDead(); });
    }
    return nullptr;
}