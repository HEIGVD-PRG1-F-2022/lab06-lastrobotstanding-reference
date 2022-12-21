/*
-----------------------------------------------------------------------------------
Nom du fichier  : MonRobot.h
Auteur(s)       : Guillaume Dunant, David Berger
Date creation   : 29.11.2022

Description     : LastRobotStanding

Remarque(s)     : <a completer>

-----------------------------------------------------------------------------------
*/

#ifndef MONROBOT_H
#define MONROBOT_H

#include <vector>
#include <string>
#include "librobots/Robot.h"

class MonRobot: public Robot {
public:
    // Créer un nouveau robot et indique la taille d1u terrain de jeu ainsi que
    // l’énergie initiale du robot et la puissance de la frappe.
    MonRobot();
    // Une étape dans le jeu: les ‘updates’ sont des informations sur 
    // l’environnement du robot. Le robot retourne un ou plusieurs string
    // pour indiquer ses actions.
    std::string action(std::vector<std::string> updates) override;
    // Retourne le nom de ce robot.
    [[nodiscard]] std::string name() const override;

    void setConfig(size_t width_init, size_t height_init, unsigned energy_init,
                   unsigned power_init) override;

private:
    std::string str_name;
    size_t dx;
    size_t dy;
    int pow;
    int life;

    static std::vector<std::vector<char>> boardRobot;

    static std::string attack(int xMove, int yMove);
    std::string move(int xMove, int yMove);
    static std::vector<std::vector<int>> objectNear(const std::vector<std::vector<char>>& board, char objectSymbol);

    std::string goForward();
    std::string goBackward();
    std::string goLeft();
    std::string goRight();
    std::string goUpLeft();
    std::string goUpRight();
    std::string goBackLeft();
    std::string goBackRight();

    static std::string fightFirstTarget(const std::vector<std::vector<int>>& enemyCoordinates);

    static std::vector<std::vector<char>> boardStringToVectors(const std:: string&);

    std::string fleeRobot(const std::vector<std::vector<int>>& enemyCoordinates);


};


#endif //MONROBOT_H
