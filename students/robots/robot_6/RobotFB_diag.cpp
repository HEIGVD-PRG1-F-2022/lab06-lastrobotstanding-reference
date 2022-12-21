/*
-----------------------------------------------------------------------------------
Nom du fichier  : RobotFB_diag.cpp
Auteur(s)       : Ewan Bloechle, Robin Forestier
Date creation   : 07.12.2022

Description     : Robot Statique, attaque tout robot passant à proximité

Remarque(s)     :

Compilateur     : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
 */

#include "RobotFB_diag.h"
#include <string>
#include <cmath>

RobotFB_diag::RobotFB_diag() {

}


void traitementAction(std::vector<std::string> &updates, int &posRX, int &posRY, int &flagR, int &posBX, int &posBY, int &flagB,std::vector<std::vector<std::string>> &alentour){
    for(std::string lignes : updates){

        //permet de récupérer la string jusqu'à l'espace (donc l'action à effectuer)
        std::string effet = lignes.substr(0,lignes.find_first_of(' '));
        lignes = lignes.substr(lignes.find_first_of(' ')+1);

        //ici différent if en fonction de l'action à effectuer (board pour maj du tableau ou damage)
        if(effet == "board"){
            //itérateur pour la chaine
            unsigned entite = 0;

            alentour.resize(5, std::vector<std::string>(5));

            //parcours-les 5x5 cases entourant le robot
            for(int i=0;i <= 4;i++){
                for(int j=0;j <=4;j++){
                    if(i != 2 || j != 2) {//pour toutes les cases sauf celle où se situe notre robot

                        //place les éléments de la chaine dans la "Vision" du robot
                        alentour.at(i).at(j) = lignes.at(entite);

                        //retiens la position du 1er robot repérer, utiliser pour stratégie d'attaque
                        if (lignes.at(entite) == 'R' && !flagR) {
                            flagR = 1;
                            posRX = i - 2;
                            posRY = j - 2;
                        }

                        /*
                        //retiens la position du 1er bonus repérer pour foncer dessus
                        if (lignes.at(entite) == 'B' && !flagB) {
                            flagB = 1;
                            posBX = i - 2;
                            posBY = j - 2;
                        }
                        */

                    }else{
                        alentour.at(i).at(j) = 'X';
                    }
                    entite++;
                }

            }


        }
        else if(effet == "damage"){//éventuel stratégie de contre attaque

        }
        else if(effet == "bonus"){

            std::string dX = lignes.substr(0,lignes.find_first_of(','));
            lignes = lignes.substr(lignes.find_first_of(',')+1);
            std::string dY= lignes.substr(0);

            int distance = (int)sqrt((pow(posBX,2)+ pow(posBY,2)));

            static int distanceMin = 100;

            if ( distance < distanceMin){

                posBX = std::stoi(dX);
                posBY = std::stoi(dY);
                distanceMin = distance;

            }

            if (distance > 15){
                flagB = 1;
            }else{
                flagB = 0;
            }


        }
    }

}

//return action à effectuer (attaquer, bouger, attendre)
std::string realisationAction(const int &flagR, const int &posRX, const int &posRY, int &posBX, int &posBY, const int &flagB, size_t energy){
    int dX,dY;
    std::string retour;

    if(flagB){//si il y a un bonus détecter et qu'il n'est pas trop loin, va le prendre
        //sens déplacement sur axe Y
        if(posBY > 0){
            dY = 1;
        }else if(posBY< 0){
            dY = -1;
        }else{
            dY = 0;
        }
        posBY--;

        //sens déplacement sur axe X
        if(posBX > 0){
            dX = 1;
        }else if(posBX< 0){
            dX = -1;
        }else{
            dX = 0;
        }
        posBX--;

        retour = "attack " + std::to_string(dX) + "," + std::to_string(dY);


    }
    else if (flagR) {//si il y a un robot
        if(energy>5){//on l'attaque si on a assez d'énergie
            retour = "attack " + std::to_string(posRX) + "," + std::to_string(posRY);
        }else{//sinon on fuit
            retour = "move" + std::to_string(-posRX) + "," + std::to_string(-posRY);
        }

    }else{//pas de bonus ni de robot -> avance en diagonal
        retour = "move 1,1";
    }


    return retour;
}

std::string RobotFB_diag::action(std::vector<std::string> updates){
    int flagR = 0;
    int posRX = 0, posRY = 0;
    int flagB = 0;
    int posBX = 0, posBY = 0;

    //traite les action reçu dans l'update
    traitementAction(updates, posRX, posRY, flagR,posBX, posBY, flagB, alentour);

    //"décide" de l'action à effectuer
    std::string action = realisationAction(flagR,posRX,posRY,flagB,posBX,posBY,energy);

    return action;
}

std::string RobotFB_diag::name() const {
    std::string nom = "RobotFB diagonal";
    return nom;
}
