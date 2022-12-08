/**
 * @author : Rachel Tranchida
 * @date : 29.11.2022
 * @file : RobotTeamR
 * @project : Refactor
 */
#ifndef REFACTOR_ROBOTTEAMR_H
#define REFACTOR_ROBOTTEAMR_H

#include <string>
#include <vector>
#include <iostream>
#include <librobots/Robot.h>
#include <string>

class RobotTeamR : public Robot {


public:

    /*RobotTeamR(size_t width, size_t height, unsigned energy, unsigned power) : width(0), height(0), energy(energy),
                                                                               power(power) {
        std::cout << "Un robot " << name() << " with energy : " << this->energy << " and power : " << this->power
                  << std::endl;

    };*/

    std::string action(std::vector<std::string> updates) override;

    std::string name() const override;

    void setConfig(size_t width, size_t height, unsigned energy, unsigned power) override;

private:
    enum Action {
        NOTHING, BOARD, DAMAGE, BONUS
    }; //Tu peux supprimer les enum et les struct si tu trouves un truc mieux
    struct ActionValue { //tu peux supprimer si tu veux et faire différemment
        Action action;
        std::string actionName;
    };
    const std::vector<ActionValue> ACTIONS = {{BOARD,  "board"},
                                              {DAMAGE, "damage"},
                                              {BONUS,  "bonus"}};

    size_t width;
    size_t height;
    unsigned energy;
    unsigned power;

    Action findAction(std::string action);
};


#endif //REFACTOR_ROBOTTEAMR_H
