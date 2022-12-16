/*
-----------------------------------------------------------------------------------
Nom du fichier  : RobotFB.cpp
Auteur(s)       : Ewan Bloechle, Robin Forestier
Date creation   : 29.11.2022

Description     :

Remarque(s)     :TODO: ne pas devenir fou (impossible)

Compilateur     : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
 */

#include "RobotFB.h"

RobotFB::RobotFB() {

}

void traitementAction(std::vector<std::string> &updates, int &posRX, int &posRY, int &flagR,
                      std::vector<std::vector<std::string>> &alentour) {
    for (std::string lignes: updates) {

        //permet de récupérer la string jusqu'à l'espace (donc l'action à effectuer)
        std::string effet = lignes.substr(0, lignes.find_first_of(' '));
        lignes = lignes.substr(lignes.find_first_of(' ')+1);

        //ici différent if en fonction de l'action à effectuer (board pour maj du tableau ou damage)
        if (effet == "board") {
            //itérateur pour la chaine
            unsigned entite = 0;

            alentour.resize(5, std::vector<std::string>(5));

            //parcours-les 5x5 cases entourant le robot
            for (int i = 0; i <= 4; i++) {//i et j allant de 0 à 4 au lieu de -2 à 2 à cause de la matrice alentour
                for (int j = 0; j <= 4; j++) {
                    if (i != 2 || j != 2) {//pour toutes les cases sauf celle où se situe notre robot

                        //place les éléments de la chaine dans la "Vision" du robot (les 5x5 case autour de lui)
                        alentour.at(i).at(j) = lignes.at(entite);

                        //retiens la position du 1er robot repérer, utiliser pour stratégie d'attaque
                        if (lignes.at(entite) == 'R' && !flagR) {
                            flagR = 1;
                            posRX = i - 2;
                            posRY = j - 2;
                        }

                    } else {//position de notre robot
                        alentour.at(i).at(j) = 'X';
                    }
                    entite++;//passe à l'élément suivant de la ligne d'updates
                }
            }
        } else if (effet == "damage") {//permet de faire une stratégie de contre-attaque

        }
    }

}

//return action à effectuer (attaquer, bouger, attendre)
std::string realisationAction(const int &flagR, const int &posRX, const int &posRY) {
    //strategie 1, la violence : s'il y a un robot à porter, on lui pète la gueule
    if (flagR) {
        std::string retour = "attack " + std::to_string(posRX) + "," + std::to_string(posRY);
        return retour;
    }
    //strategie 2, patience: Aucune possibilitée d'attaque car aucun robot autour
    else {
        return "wait";
    }
}

std::string RobotFB::action(std::vector<std::string> updates) {
    int flagR = 0, distR = 0;
    int posRX = 0, posRY = 0;

    //traite les actions reçu dans l'update
    traitementAction(updates, posRX, posRY, flagR, alentour);

    //"décide" de l'action à effectuer
    std::string action = realisationAction(flagR, posRX, posRY);

    return action;
}

std::string RobotFB::name() const {
    std::string nom = "RobotFB";
    return nom;
}
