/*
 *  HEIG-VD
 *  Description: PRG1 Labo6 - Last Robot Standing
 *  Author(s): Bruno Manuel Vieira Rosas & Ahmad Jano
 *  Date: 29.11.2022
*/

#ifndef REFACTOR_MONROBOT_H
#define REFACTOR_MONROBOT_H
#include <iostream>
#include <librobots/Direction.h>
#include <librobots/Message.h>
#include <librobots/Position.h>
#include <librobots/Robot.h>
#include <librobots/RobotState.h>
#include <vector>

class MyRobot1: public Robot {
private:
    size_t width = 0, height = 0, energy = 0, power = 0;
public:
    // Une étape dans le jeu: les ‘updates’ sont des informations sur
    // l’environnement du robot. Le robot retourne un ou plusieurs string
    // pour indiquer ses actions.
    std::string action(std::vector<std::string> updates);

    void setConfig(size_t width_init, size_t height_init, unsigned int energy_init, unsigned int power_init) override;

    std::string name() const;
};


#endif //REFACTOR_MONROBOT_H
