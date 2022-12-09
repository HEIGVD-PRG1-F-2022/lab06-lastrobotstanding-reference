#include <algorithm>
#include <libdio/display.h>
#include <librobots/Bonus.h>
#include <librobots/Message.h>
#include <librobots/RobotState.h>
#include <random>
#include <thread>
#include <vector>

using namespace std;

class Game {
    const unsigned ENERGY_INIT = 10;
    const unsigned POWER_INIT = 1;
    const unsigned BONUS_RECURRENCE = 100;
    const unsigned IDLE_LIMIT = 200;

    vector<Robot *> waitList;
    vector<RobotState> robots;
    vector<Position> radar;
    vector<Position> positions;
    vector<Bonus> boni;
    unsigned bonusTimeout = 1;
    size_t side = 0;
    unsigned round = 0, idle = 0;

    friend void testGame();

    long robotsAlive() {
        return count_if(robots.begin(), robots.end(), [](const RobotState &r) -> bool { return !r.isDead(); });
    }

    void waitListToArena() {
        if (waitList.empty()) {
            throw runtime_error("No robot waiting for the arena");
        }
        side = size_t(10 * sqrt(waitList.size()));
        for (auto robot: waitList) {
            robots.emplace_back(robot, Position::random(unsigned(side), unsigned(side), side, side), side, ENERGY_INIT, POWER_INIT);
        }
    }

    void actionAttack() {
        for (const auto &attacker: robots) {
            if (attacker.isDead()) {
                continue;
            }
            Message act = attacker.getAction();
            if (act.msg == MessageType::ActionAttack) {
                auto destination = attacker.getPosition() + act.robots.at(0);
                for (auto &defender: robots) {
                    if (defender.isDead()) {
                        continue;
                    }
                    if (destination == defender.getPosition()) {
                        defender.actionAttack(attacker, destination);
                        idle = 0;
                    }
                }
            }
        }
    }

    void actionMove() {
        for (auto &robot: robots) {
            if (robot.isDead()) {
                continue;
            }
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
                if (robots.at(i).isDead() || robots.at(j).isDead()) {
                    continue;
                }
                robots.at(i).checkCollision(robots.at(j));
            }
        }
        positions.clear();
        for (const auto &robot: robots) {
            if (robot.isDead()) {
                continue;
            }
            positions.push_back(robot.getPosition());
        }
    }

    void actionRadar() {
        radar.clear();
        for (auto &robot: robots) {
            if (robot.isDead()) {
                continue;
            }
            Message act = robot.getAction();
            if (act.msg == MessageType::ActionRadar) {
                robot.actionRadar(positions);
                radar.push_back(robot.getPosition());
            }
        }
    }

    void createBonus() {
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
                if (robot.isDead()) {
                    continue;
                }
                robot.actionBonus(newBonus->pos);
            }
            boni.push_back(*newBonus);
        }
    }

    void checkBonus() {
        for (auto &robot: robots) {
            if (robot.isDead()) {
                continue;
            }
            auto b = find_if(boni.begin(), boni.end(), [robot](Bonus b) -> bool { return b.pos == robot.getPosition(); });
            if (b != boni.end()) {
                idle = 0;
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
        vector<Position> boniPos(boni.size());
        transform(boni.begin(), boni.end(), boniPos.begin(), [](Bonus b) -> Position { return b.pos; });
        auto updateBoards = Message::updateBoard(positions, boniPos);
        for (auto &robot: robots) {
            if (robot.isDead()) {
                continue;
            }
            robot.sendUpdate(updateBoards.front());
            updateBoards.erase(updateBoards.begin());
        }
    }

    void display_debug() {
        cout << endl
             << "Robots: ";
        for (const auto &robot: robots) {
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
        vector<vector<Display::DString>> grid(side, vector<Display::DString>(side));
        for (char robotNbr = '0'; const auto &robot: robots) {
            robotNbr++;
            if (robot.isDead()) {
                continue;
            }
            auto pos = robot.getPosition();
            grid.at(size_t(pos.getY())).at(size_t(pos.getX())) = Display::DString(Display::Color::GREEN) << string(&robotNbr);
        }
        for (const auto &bonus: boni) {
            grid.at(size_t(bonus.pos.getY())).at(size_t(bonus.pos.getX())) = Display::DString(Display::Color::YELLOW) << "B";
        }
        Display::clearScreen();
        Display::displayGrid(grid, false).print();
        cout << "Round: " << round++ << " Idle for: " << idle++ << endl;
        for (int i = 1; const auto &robot: robots) {
            if (robot.isDead()) {
                cout << (Display::DString(Display::Color::RED) << i++ << " - Robot: " << robot.getName() << " - RIP: " << robot.getDeathCause()) << endl;
            } else {
                cout << (Display::DString(Display::Color::GREEN) << i++ << " - Robot: " << robot.getName() << " - Energy: " << robot.getEnergy()
                                                                 << " - Power: " << robot.getPower() << " - action: " << robot.getAction().getMessageType())
                     << endl;
            }
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
        do {
            actionAttack();
            actionMove();
            actionRadar();
            createBonus();
            checkBonus();
            display();
            sendUpdates();
            this_thread::sleep_for(100ms / log(round + 2));
        } while (robotsAlive() > 1 && idle < IDLE_LIMIT);

        if (robotsAlive() == 1) {
            return &*find_if(robots.begin(), robots.end(), [](const RobotState &r) -> bool { return !r.isDead(); });
        }
        return nullptr;
    }
};
