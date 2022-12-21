//
// Created by trixi on 29/11/2022.
//

#ifndef LAB06_LASTROBOTSTANDING_TEAM_QUI_VA_GAGNER_PIPOU_H
#define LAB06_LASTROBOTSTANDING_TEAM_QUI_VA_GAGNER_PIPOU_H

#include "vector"
#include <string>
#include "librobots/Robot.h"

using namespace std;

class Pipou : public Robot {
private:
    size_t width;
    size_t height;
    //vector<vector<char>> board;
    unsigned energy;

public:
    unsigned int getEnergy() const;

    void setEnergy(unsigned int energy);

    unsigned int getPower() const;

    void setPower(unsigned int power);

    void setRobotName(const string &robotName);

private:
    unsigned power;
    string robotName = "Pipou Le Fumier";
    vector<int> checkPos(string pos);

public:
    // Créer un nouveau robot et indique la taille du terrain de jeu ainsi que
    // l’énergie initiale du robot et la puissance de la frappe.
    Pipou(size_t width, size_t height, unsigned energy, unsigned power);
    Pipou();

    // Une étape dans le jeu: les ‘updates’ sont des informations sur
    // l’environnement du robot. Le robot retourne un ou plusieurs string
    // pour indiquer ses actions.
    string action(vector<string> updates);

    // Retourne le nom de ce robot.
    string name();

    const string &getRobotName() const;
};


#endif//LAB06_LASTROBOTSTANDING_TEAM_QUI_VA_GAGNER_PIPOU_H
