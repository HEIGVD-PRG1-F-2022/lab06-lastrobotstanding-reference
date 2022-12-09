/*
-----------------------------------------------------------------------------------
Nom du fichier  : RobotFB_diag.cpp
Auteur(s)       : Ewan Bloechle, Robin Forestion
Date creation   : 07.12.2022

Description     :

Remarque(s)     :

Compilateur     : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
 */

#include "RobotFB_diag.h"

RobotFB_diag::RobotFB_diag() {

}


void traitementAction(std::vector<std::string>& updates,std::vector<std::vector<char>>& alentour,size_t& energy){
    for(std::string lignes : updates){

        //permet de récupérer la string jusqu'à l'espace (donc l'action à effectuer)
        std::string effet = lignes.substr(0,lignes.find_first_of(' '));
        lignes = lignes.substr(lignes.find_first_of(' '));

        //ici différent if en fonction de l'action à effectuer (board pour maj du tableau ou damage)
        if(effet == "board"){
            //itérateur pour la chaine
            unsigned entite = 0;

            //parcours-les 5x5 cases entourant le robot
            for(int i=0;i <= 4;i++){
                for(int j=0;j <=4;i++){
                    if(i != 2 && j != 2) {//pour toutes les cases sauf celle où se situe notre robot

                        //place les éléments de la chaine dans la "Vision" du robot
                        alentour.at(i).at(j) = lignes.at(entite);

                        entite++;
                    }else{
                        alentour.at(i).at(j) = 'X';
                    }
                }

            }


        }
        else if(effet == "damage"){

            //prend la première valeur, pour la coordoner X du robot nous attanquant
            std::string dX = lignes.substr(0,lignes.find_first_of(','));
            //converti en int
            int coordX = stoi(dX);
            //efface les données assimiler
            lignes = lignes.substr(lignes.find_first_of(',')+1);

            //même chose que avant mais pour la coordonée Y
            std::string dY = lignes.substr(0,lignes.find_first_of(','));
            int coordY = stoi(dY);
            lignes = lignes.substr(lignes.find_first_of(',')+1);


            //prend la dernière valeur restante, sensé être les dommages reçus
            std::string dam = lignes;
            int damage = stoi(dam);
        }
    }

}

//return action à effectuer (attaquer, bouger, attendre)
std::string realisationAction(const size_t& energy){
    int dX = 1,dY = 1;

    std::string retour = "move "+ std::to_string(dX)+","+std::to_string(dY);
    return retour;
    //avance uniquement en diagonal
}

std::string RobotFB_diag::action(std::vector<std::string> updates){

    //traite les action reçu dans l'update
    traitementAction(updates,alentour,energy);

    //"décide" de l'action à effectuer
    std::string action = realisationAction(energy);

    return action;
}

std::string RobotFB_diag::name() const{
    std::string nom = "RobotFB_diag";
    return nom;
}

