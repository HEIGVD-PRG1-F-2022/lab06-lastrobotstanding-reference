/*
-----------------------------------------------------------------------------------
Nom du fichier  : RobotFB.h
Auteur(s)       : Ewan Bloechle, Robin Forestier
Date creation   : 29.11.2022

Description     :

Remarque(s)     :

Compilateur     : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
 */


#ifndef REFACTOR_ROBOTFB_H
#define REFACTOR_ROBOTFB_H

#include <iostream>
#include <vector>
#include <string>

#include "librobots/Robot.h"

class RobotFB {

    friend void traitementAction(std::vector<std::string> &updates, int &posRX, int &posRY, int &flagR,std::vector<std::vector<std::string>> &alentour);
    friend std::string realisationAction(const int &flagR, const int &posRX, const int &posRY);

public:

    RobotFB();

    [[nodiscard]] double getEnergy() const {
        return energy;
    }

    [[nodiscard]] double getPower() const {
        return power;
    }

    void setEnergy(double nrj) {
        energy = nrj;
    }
    void setPower(double pow) {
        power = pow;
    }

    // Créer un nouveau robot et indique la taille du terrain de jeu ainsi que
    // l’énergie initiale du robot et la puissance de la frappe.

    void setConfig(size_t width_init = 0, size_t height_init = 0, unsigned energy_init = 10,
                   unsigned power_init = 1)  {
        width = width_init;
        height = height_init;
        energy = energy_init;
        power = power_init;
    };

    // Une étape dans le jeu : les ‘updates’ sont des informations sur
    // l’environnement du robot. Le robot retourne un ou plusieurs string
    // pour indiquer ses actions.
    std::string action(std::vector<std::string> updates) ;

    // Retourne le nom de ce robot.
    std::string name() const ;

private:
    std::vector<std::vector<std::string>> alentour;
    size_t width = 0, height = 0, energy = 0, power = 0;

};



#endif //REFACTOR_ROBOTFB_H
