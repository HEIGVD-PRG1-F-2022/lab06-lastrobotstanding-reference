/*
-----------------------------------------------------------------------------------
Nom du fichier  : RobotFB.cpp
Auteur(s)       : Ewan Bloechle, Robin Forestion
Date creation   : 29.11.2022

Description     :

Remarque(s)     :TODO: optimiser stratégie 2 dans le cas ou il y a plusieurs robot au alentour (ou la supprimer)
                 TODO: ne pas devenir fou

Compilateur     : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
 */

#include "RobotFB.h"

RobotFB::RobotFB() {

}

void traitementAction(std::vector<std::string>& updates,int& posRX,int& posRY,int& flagR,int& distR,std::vector<std::vector<char>>& alentour,size_t& energy){
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

                        //retiens la position du 1er robot repérer, utiliser pour stratégie d'attaque
                        if (lignes.at(entite)== 'R' && flagR == 0){
                            flagR = 1;
                            posRX = i;
                            posRY = j;
                            if((i == 0 || i == 4)&&(j == 0 || j == 4)){
                                distR = 2; //robot ennemi distant
                            }else{
                                distR = 1; //robot ennemi proche
                            }
                        }
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

            //energy -= damage;//todo: demander à quel moment enlever energy (ici ou dans games)
        }
    }

}

//return action à effectuer (attaquer, bouger, attendre)
std::string realisationAction(const int& flagR,int& distR,const int& posRX,const int& posRY,const size_t& energy){
    int dX = 0,dY = 0;

    //strategie 1: violence (scan tout les cases alentours, si trouve un R va lui péter la gueule)
    if(flagR && energy > 3){ //s'il lui reste assez d'energy pour combatre
        //le robot ennemi est dans notre périmètre direct
        if(distR == 1){
            //si le robot est à droite, avance vers la droite. Sinon si le robot est à gauche, va vers la gauche. Sinon
            //robot sur même axe X que nous, ne se déplace pas en X
            if(posRX > 2){
                dX = 1;
            }else if(posRX < 2){
                dX = -1;
            }

            //si le robot est devant, avance vers l'avant. Sinon si le robot est derrière, va vers l'arrière. Sinon
            //robot sur même axe Y que nous, ne se déplace pas en Y
            if(posRY > 2){
                dY = 1;
            }else if(posRY < 2){
                dY = -1;
            }
        }
        //même chose que précédement mais le robot ennemi est dans notre rayon d'attaque à distance
        else if (distR == 2){
            if(posRX > 2){
                dX = 2;
            }else if(posRX < 2){
                dX = -2;
            }
            if(posRY > 2){
                dY = 2;
            }else if(posRY < 2){
                dY = -2;
            }
        }

        std::string retour = "attack "+ std::to_string(dX)+","+std::to_string(dY);
        return retour;
    }

    //strategie 2(si peu d'energy):retraite tactique (scan tout les cases alentours, si trouve un R va dans direction oposée)
    else if(flagR && energy <= 3){//si l'energy est trop faible

        //si le robot est à droite, avance vers la gauche. Sinon si le robot est à gauche, va vers la droite. Sinon
        //robot sur même axe X que nous, ne se déplace pas en X
        if(posRX > 2){
            dX = -1;
        }else if(posRX < 2){
            dX = 1;
        }

        //si le robot est devant, avance vers l'arrière. Sinon si le robot est derrière, va vers l'avant. Sinon
        //robot sur même axe Y que nous, ne se déplace pas en Y
        if(posRY > 2){
            dX = -1;
        }else if(posRY < 2){
            dX = 1;
        }

        std::string retour = "move "+ std::to_string(dX)+","+std::to_string(dY);
        return retour;
    }

    //strategie 3: patience (Aucune possibilitée d'attaque car compteurR = 0 ou de fuite car entourer de robot, reste sur place)
    else{
        return "wait";
    }
}

std::string RobotFB::action(std::vector<std::string> updates){
    int flagR = 0, distR = 0;
    int posRX = 0, posRY = 0;

    //traite les action reçu dans l'update
    traitementAction(updates,posRX,posRY,flagR,distR,alentour,energy);

    //"décide" de l'action à effectuer
    std::string action = realisationAction(flagR,distR,posRX,posRY,energy);

    return action;
}

std::string RobotFB::name() const {
    std::string nom = "RobotFB";
    return nom;
}
