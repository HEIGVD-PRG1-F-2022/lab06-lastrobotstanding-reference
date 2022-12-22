/*
-----------------------------------------------------------------------------------
Nom du fichier  : RobotFB_diag.h
Auteur(s)       : Ewan Bloechle, Robin Forestier
Date creation   : 07.12.2022

Description     : Robot allant constamment en diagonal en haut à droite

Remarque(s)     : sert principalement pour les tests

Compilateur     : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
 */

#ifndef LABO6_ROBOTFB_DIAG_H
#define LABO6_ROBOTFB_DIAG_H

#include <iostream>
#include <vector>
#include <string>

#include "librobots/Robot.h"

class RobotFB_diag : public Robot {

    friend void traitementAction(std::vector<std::string> &updates, int &posRX, int &posRY, int &flagR, int &posBX, int &posBY, bool &flagB,std::vector<std::vector<std::string>> &alentour);
    friend std::string realisationAction(const int &flagR, const int &posRX, const int &posRY, int &posBX, int &posBY, const bool &flagB);

    friend int calculDistance(int dX, int dY);

public:

    RobotFB_diag();

    [[nodiscard]] int getEnergy() const {
        return energy;
    }

    [[nodiscard]] int getPower() const {
        return power;
    }

    // Créer un nouveau robot et indique la taille du terrain de jeu ainsi que
    // l’énergie initiale du robot et la puissance de la frappe.

    void setConfig(size_t width_init = 0, size_t height_init = 0, unsigned energy_init = 10,
                   unsigned power_init = 1) {
        width = width_init;
        height = height_init;
        energy = energy_init;
        power = power_init;
    };

    // Une étape dans le jeu : les ‘updates’ sont des informations sur
    // l’environnement du robot. Le robot retourne un ou plusieurs string
    // pour indiquer ses actions.
    std::string action(std::vector<std::string> updates) override;

    // Retourne le nom de ce robot.
    std::string name() const override;

private:
    std::vector<std::vector<std::string>> alentour;
    int width = 0, height = 0, energy = 0, power = 0;
    int posBX = 0, posBY = 0;
    bool flagB = false;
};


#endif //LABO6_ROBOTFB_DIAG_H
