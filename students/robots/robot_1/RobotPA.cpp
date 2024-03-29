//
// Created by PatrickMaillard on 29.11.2022.
//

#include "RobotPA.h"
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>

using namespace std;

unsigned RobotPA::numberOfRobot = 1;

string RobotPA::action(vector<string> updates) {
    //notre stratégie (Exemple: si un robot est dans un rayon de 1 alors retourner
    //attaquer)

    // traiter la perte d'energie
    auto iterr = find(updates.begin(), updates.end(), "board");
    size_t positionDamage = static_cast<size_t>(iterr - updates.begin() - 1);

    if(iterr != updates.end()){

        // à commenter et expliquer
        string damageTaken = updates.at(positionDamage);
        vector<string> damages;
        string e;
        stringstream ssDamages(damageTaken);

        while (getline(ssDamages, e, ',')){
            damages.push_back(e);
        }

        // a tester
        int energyToRemove = stoi(damages[2]);
        energy -= energyToRemove;
    }

    vector<vector<char>> board2D(5, vector<char>(5, ' '));

    auto iter = find(updates.begin(), updates.end(), "board");

    size_t positionBoard = static_cast<size_t>(iter - updates.begin() - 1);
    //cout << updates.at(positionBoard);
    //le probèle est à cet ligne (voir l'iteration du find plus haut qui est
    // bizzare) essayé d'affiché le updateMemory pour comprendre
    string board = updates.at(positionBoard).substr(6,25);

    //on met le board sous forme de matrice afin de simplifier la recherche pour
    // la position ou l'on attaque un robot
    for(int i = 0, k = 0; i<5;i++){
        for(int j=0; j<5; j++, k++){
            board2D[i][j] = board[k];
        }
    }

    //si un robot est dans notre champ de vision alors on l'attaque
    for(vector<char> v : board2D) {
        for(char c : v){
            if(c != ' ' && c != char(robotNumber + 48) && c != 'B'){
                int posXRobotToAttack = int(find(v.begin(), v.end(), c) - v.begin() - 2);
                int posYRobotToAttack = int(find(board2D.begin(), board2D.end(),v) - board2D.begin() - 2); //-2 car on cherche la position du
                // robot a attaquer par rapport à l'attanquant
                return "attack " + to_string(posXRobotToAttack) + "," +
                        to_string(posYRobotToAttack);
            }
        }
    }
    return "wait";
}


void RobotPA::setConfig(size_t width_init, size_t height_init, unsigned int
energy_init, unsigned int power_init){
    robotNumber = numberOfRobot++;
    width = width_init;
    height = height_init;
    energy = energy_init;
    power = power_init;
}
