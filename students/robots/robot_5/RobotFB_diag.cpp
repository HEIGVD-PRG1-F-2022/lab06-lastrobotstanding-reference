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

RobotFB_diag::RobotFB_diag() {

}


void traitementAction(std::vector<std::string>& updates,std::vector<std::vector<std::string>>& alentour){
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


                    }else{
                        alentour.at(i).at(j) = 'X';
                    }
                    entite++;
                }

            }


        }
        else if(effet == "damage"){

        }
    }

}

//return action à effectuer (attaquer, bouger, attendre)
std::string realisationAction(){
    int dX = 1,dY = 1;

    std::string retour = "move "+ std::to_string(dX)+","+std::to_string(dY);
    return retour;
    //avance uniquement en diagonal
}

std::string RobotFB_diag::action(std::vector<std::string> updates){

    //traite les action reçu dans l'update
    traitementAction(updates,alentour);

    //"décide" de l'action à effectuer
    std::string action = realisationAction();

    return action;
}

std::string RobotFB_diag::name() const {
    std::string nom = "RobotFB diagonal";
    return nom;
}
