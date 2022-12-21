/*
-----------------------------------------------------------------------------------
Nom du fichier  : RobotFB_diag.cpp
Auteur(s)       : Ewan Bloechle, Robin Forestier
Date creation   : 07.12.2022

Description     :Startégie du robot: dès qu'il détecte un bonus assez proche va le chercher
                 Sinon si il y a un robot à proximité et qu'il lui reste assez de pv va se battre sinon fui
                 Si rien il n'y a ni bonus ni robot à proximité, avance en diagonal.

Remarque(s)     :todo: le robot ne vas pas vers un bonus lorsqu'il en détecte un

Compilateur     : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
 */

#include "RobotFB_diag.h"
#include <string>
#include <cmath>

RobotFB_diag::RobotFB_diag() = default;


void traitementAction(std::vector<std::string> &updates, int &posRX, int &posRY, bool &flagR, int &posBX, int &posBY, bool &flagB,std::vector<std::vector<std::string>> &alentour){
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
            for(size_t i=0; i <= 4; i++){
                for(size_t j=0;j <=4;j++){
                    if(i != 2 || j != 2) {//pour toutes les cases sauf celle où se situe notre robot

                        //place les éléments de la chaine dans la "Vision" du robot
                        alentour.at(i).at(j) = lignes.at(entite);

                        if (lignes.at(entite) == 'B') {
                            flagB = true;
                            posBX = (int)i - 2;//cast en int pour réduire les warnings
                            posBY = (int)j - 2;
                        }  //retiens la position du 1er robot repérer, utiliser pour stratégie d'attaque
                        else if (lignes.at(entite) == 'R' && !flagR) {
                            flagR = true;
                            posRX = (int)i - 2;//cast en int pour réduire les warnings
                            posRY = (int)j - 2;
                        }


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

            std::string BonusX = lignes.substr(0,lignes.find_first_of(','));
            lignes = lignes.substr(lignes.find_first_of(',')+1);
            std::string BonusY= lignes.substr(0);

            int dX = stoi(BonusX);
            int dY = std::stoi(BonusY);


            //distance entre le bonus et notre robot
            int distance = (int)sqrt((pow(dX,2)+ pow(dY,2)));


            //retient la distance du bonus le plus proche
            static int distanceMin = 100;


            //si le nouveau bonus est plus près que le précédent plus proche bonus
            if ( distance < distanceMin){

                //modifie la nouvelle distance ainsi que les coordonée x et y du plus proche bonus
                posBX = dX;
                posBY = dY;
                distanceMin = distance;

            }

            if (distanceMin < 15){//si le bonus est assez proche
                flagB = true;
            }


        }
    }

}

//return action à effectuer (attaquer, bouger, attendre)
std::string realisationAction(const bool &flagR, const int &posRX, const int &posRY, int &posBX, int &posBY, const bool &flagB, size_t energy){
    int dX,dY;
    std::string retour;

    if(flagB){//si il y a un bonus détecter et qu'il n'est pas trop loin, va le prendre
        //sens déplacement sur axe Y
        if(posBY > 0){//si le bonus est en dessus, monte
            dY = 1;
        }else if(posBY < 0){//sinon si le bonus est en dessous, descend
            dY = -1;
        }else{//sinon ne se déplace pas sur axe Y
            dY = 0;
        }


        //sens déplacement sur axe X, idem que pour l'axe Y
        if(posBX > 0){
            dX = 1;
        }else if(posBX < 0){
            dX = -1;
        }else{
            dX = 0;
        }


        retour = "move " + std::to_string(dX) + "," + std::to_string(dY);
    }
    else if (flagR) {//si il y a un robot
        if(energy>5){//on l'attaque si on a assez d'énergie
            retour = "attack " + std::to_string(posRX) + "," + std::to_string(posRY);
        }else{//sinon on fuit
            retour = "move " + std::to_string(-posRX) + "," + std::to_string(-posRY);
        }

    }else{//pas de bonus ni de robot -> avance en diagonal
        retour = "move 1,1";
    }


    return retour;
}

std::string RobotFB_diag::action(std::vector<std::string> updates){
    bool flagR = false;
    int posRX = 0, posRY = 0;
    bool flagB = false;


    //traite les action reçu dans l'update
    traitementAction(updates, posRX, posRY, flagR,posBX, posBY, flagB, alentour);

    //"décide" de l'action à effectuer
    std::string action = realisationAction(flagR,posRX,posRY,posBX,posBY,flagB,energy);

    return action;
}

std::string RobotFB_diag::name() const {
    std::string nom = "RobotFB diagonal";
    return nom;
}
