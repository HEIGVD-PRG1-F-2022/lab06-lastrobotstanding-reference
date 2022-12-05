#include <algorithm>
#include <librobots.h>

class RobotSmart : public Robot {
    size_t width = 0, height = 0;
    unsigned energy = 0, power = 0;
    unsigned nearestCounter = 0;
    Direction nearestDirection;

    void updateNearest(Direction dir, bool bonus = false) {
        double distanceNearest = nearestDirection.mag(),
               distance = dir.mag();
        if (distance < distanceNearest ||
            distance + nearestCounter < distanceNearest ||
            nearestCounter == 0 ||
            bonus) {
            nearestDirection = dir;
            nearestCounter = bonus ? 100 : 10;
        }
    }

public:
    RobotSmart() = default;

    void setConfig(size_t init_width, size_t init_height, unsigned init_energy,
                   unsigned init_power) override {
        width = init_width;
        height = init_height;
        energy = init_energy;
        power = init_power;
    }

    std::string action(std::vector<std::string> updates) override {
        Position p;
        for (const auto &update: updates) {
            //            cout << "Update is: " << update << endl;
            Message u(update);
            switch (u.msg) {
                case MessageType::UpdateBoard:
                    if (!u.boni.empty()) {
                        sort(u.boni.begin(), u.boni.end(), [](Direction a, Direction b) -> bool { return a.mag() < b.mag(); });
                        return Message::actionMove(u.boni.at(0).unitary());
                    }
                    if (!u.robots.empty()) {
                        for (auto robot: u.robots) {
                            if (robot.mag() < 2) {
                                return Message::actionAttack(robot);
                            }
                            updateNearest(u.robots.at(0));
                        }
                    }
                    break;
                case MessageType::UpdateDamage:
                    energy -= u.energy;
                    return Message::actionAttack(u.robots.at(0));
                    break;
                case MessageType::UpdateEnergy:
                    energy += u.energy;
                    nearestCounter = 0;
                    break;
                case MessageType::UpdatePower:
                    power += u.power;
                    nearestCounter = 0;
                    break;
                case MessageType::UpdateBonus:
                    updateNearest(u.boni.at(0), true);
                    break;
                case MessageType::UpdateRobot:
                    updateNearest(u.robots.at(0));
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

    [[nodiscard]] std::string name() const override {
        return "Smart one";
    }
};