#include "Bonus.h"
#include "RobotState.h"
#include <algorithm>
#include <libdio/display.h>
#include <librobots.h>
#include <random>
#include <thread>
#include <vector>

using namespace std;

class Game {
    const unsigned ENERGY_INIT = 10;
    const unsigned POWER_INIT = 1;
    const unsigned BONUS_RECURRENCE = 100;

    vector<Robot *> waitList;
    vector<RobotState> robots;
    vector<Position> radar;
    vector<Position> positions;
    vector<Bonus> boni;
    unsigned bonusTimeout = 1;
    size_t side = 0;
    unsigned round = 0;

    friend void testGame();

    void waitListToArena() {
        if (waitList.empty()) {
            throw runtime_error("No robot waiting for the arena");
        }
        side = size_t(10 * sqrt(waitList.size()));
        for (auto robot: waitList) {
            robots.emplace_back(robot, Position::random(unsigned(side), unsigned(side), side, side), side, ENERGY_INIT, POWER_INIT);
        }
    }

    void buryRobots() {
        robots.erase(remove_if(robots.begin(), robots.end(), [](const RobotState &rs) { return rs.isDead(); }), robots.end());
    }

    void actionAttack() {
        for (auto attacker: robots) {
            Message act = attacker.getAction();
            if (act.msg == MessageType::ActionAttack) {
                auto destination = attacker.getPosition() + act.robots.at(0);
                for (auto &defender: robots) {
                    defender.actionAttack(defender, destination);
                }
            }
        }
        buryRobots();
    }

    void actionMove() {
        for (auto &robot: robots) {
            Message act = robot.getAction();
            if (act.msg == MessageType::ActionMove) {
                robot.actionMove(act.robots.at(0).unitary());
            }
        }
        for (size_t i = 0; i < robots.size(); i++) {
            for (size_t j = 0; j < robots.size(); j++) {
                if (i == j) {
                    continue;
                }
                robots.at(i).checkCollision(robots.at(j));
            }
        }
        positions.resize(robots.size());
        transform(robots.begin(), robots.end(), positions.begin(), [](const RobotState &s) { return s.getPosition(); });

        buryRobots();
    }

    void actionRadar() {
        radar.clear();
        for (auto &robot: robots) {
            Message act = robot.getAction();
            if (act.msg == MessageType::ActionRadar) {
                robot.actionRadar(positions);
                radar.push_back(robot.getPosition());
            }
        }
    }

    void createBonus() {
        if (bonusTimeout-- == 0) {
            bonusTimeout = BONUS_RECURRENCE;
            std::random_device rd;
            std::uniform_int_distribution<int> type(0, 1);
            int newType = type(rd);
            Bonus newBonus;
            do {
                if (newType == 0) {
                    newBonus = Bonus(side, side, BonusType::Energy, uniform_int_distribution<unsigned>(0, 9)(rd));
                } else {
                    newBonus = Bonus(side, side, BonusType::Power, uniform_int_distribution<unsigned>(0, 3)(rd));
                }
            } while (find_if(boni.begin(), boni.end(), [newBonus](Bonus b) -> bool { return b.pos == newBonus.pos; }) != boni.end());
            for (auto &robot: robots) {
                robot.actionBonus(newBonus.pos);
            }
            boni.push_back(newBonus);
        }
    }

    void checkBonus() {
        for (auto &robot: robots) {
            auto b = find_if(boni.begin(), boni.end(), [robot](Bonus b) -> bool { return b.pos == robot.getPosition(); });
            if (b != boni.end()) {
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

    void sendUpdates() {
        auto updateBoards = Message::updateBoard(positions);
        for (size_t i = 0; i < robots.size(); i++) {
            robots.at(i).sendUpdate(updateBoards.at(i));
        }
    }

    void display_debug() {
        cout << endl
             << "Robots: ";
        for (auto robot: robots) {
            cout << robot.getPosition() << " :: ";
        }
        cout << endl
             << "Boni: ";
        for (auto bonus: boni) {
            cout << bonus.pos << " :: ";
        }
        cout << endl;
    }

    void display() {
        vector<vector<string>> grid(side, vector<string>(side));
        char robotNbr = '1';
        for (const auto &robot: robots) {
            auto pos = robot.getPosition();
            grid.at(size_t(pos.getY())).at(size_t(pos.getX())) = robotNbr++;
        }
        for (const auto &bonus: boni) {
            grid.at(size_t(bonus.pos.getY())).at(size_t(bonus.pos.getX())) = 'B';
        }
        Display::clearScreen();
        Display::displayGrid(grid, false).print();
        cout << "Round: " << round++ << endl;
        for (auto robot: robots) {
            cout << "Robot: " << robot.name() << " - Energy: " << robot.getEnergy() << " - Power: " << robot.getPower() << endl;
        }
    }

public:
    Game() = default;

    void addRobot(Robot *r) {
        if (!robots.empty()) {
            throw runtime_error("Play already started!");
        }
        waitList.push_back(r);
    }

    RobotState *play() {
        Display::init();
        waitListToArena();
        while (robots.size() > 1) {
            actionAttack();
            actionMove();
            actionRadar();
            createBonus();
            checkBonus();
            sendUpdates();
            display();
            this_thread::sleep_for(100ms);
        }
        if (robots.size() == 1) {
            return &robots.at(0);
        } else {
            return nullptr;
        }
    }
};
