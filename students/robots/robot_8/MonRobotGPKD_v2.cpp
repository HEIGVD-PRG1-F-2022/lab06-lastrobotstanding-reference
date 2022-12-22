/*
-----------------------------------------------------------------------------------
Nom du fichier  : MonRobotGPKD_v2_v2.cpp
Auteur(s)       : Guilherme Pinto da Cunha da Mata
Date creation   : 20.12.2022

Description     : <à compléter>

Remarque(s)     : <à compléter>

Compilateur     : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
*/

#include "MonRobotGPKD_v2.h"
#include "librobots/Message.h"
#include <iostream>

using namespace std;

// Initialisation de la variable static
unsigned MonRobotGPKD_v2::nbRobot = 0;


//enum class UPDATE {
//   BOARD, DAMAGE, ENERGY, POWER, BONUS, ROBOT
//};

void MonRobotGPKD_v2::setConfig(size_t widthInit, size_t heightInit,
                                unsigned energyInit, unsigned powerInit) {
   this->width = widthInit;
   this->height = heightInit;
   this->energy = energyInit;
   this->power = powerInit;
   vector<vector<char>> resize(NB_LIGNE, vector<char>(NB_COLONNES));
   board = resize;
   ++nbRobot;
}

string MonRobotGPKD_v2::action(vector<string> updates) {
   const string ESPACE = " ";
//   size_t finEntete = 0;
   vector<string> command;

   for (const string &update: updates) {
//      finEntete = update.find(ESPACE,finEntete);
//      selectionUpdate(finEntete, update);
      command = split(update, ESPACE, 2);
      selectionUpdate(command);
   }
   return strategy();
}

string MonRobotGPKD_v2::name() const {
   return "MonRobotGPKD_v2 " ;
}

void MonRobotGPKD_v2::selectionUpdate(const vector<string> &
command) {
//   string enTete;
//   enTete =s.substr(0,finEntete + 1);

   string enTete;
   enTete = command.at(0);

   if (enTete == "board") {
      boardUpdate(command.at(1));
   } else if (enTete == "damage") {
      damageUpdate(command.at(1));
   } else if (enTete == "energy") {
      energyUpdate(command.at(1));
   } else if (enTete == "power") {
      powerUpdate(command.at(1));
   } else if (enTete == "bonus") {
      bonusUpdate(command.at(1));
   } else if (enTete == "robot") {
      robot(command.at(1));
   }
}

void MonRobotGPKD_v2::boardUpdate(const string &s) {

   // Vérification que la chaîne soit de la bonne taille
   if (NB_COLONNES * NB_LIGNE == s.length()) {
      size_t ligne = 0;
      for (size_t i = 0; i < s.length(); ++i) {
         board.at(ligne).at(i % NB_COLONNES) = s.at(i);

         // Si on atteint la dernière colonne on incrémente la ligne
         if (i % NB_COLONNES == NB_COLONNES - 1) {
            ++ligne;
         }
      }
   }
}

void MonRobotGPKD_v2::damageUpdate(const string &s) {
    const string VIRGULE = ",";
    vector<string> cmdDamage = split(s, VIRGULE, 3);
    vector<int> vDamage(3);
    // Initialise le Vector vDamage [int posX, int posY, int degats]
    for (size_t i = 0; int &val: vDamage) {
        val = stoi(cmdDamage.at(i));
        ++i;
    }
    degats = (unsigned) vDamage.back();
    energy -= degats;
}

void MonRobotGPKD_v2::energyUpdate(const string &s) {
   energy += (unsigned) stoi(s);
}

void MonRobotGPKD_v2::powerUpdate(const string &s) {
   power += (unsigned) stoi(s);
}

void MonRobotGPKD_v2::bonusUpdate(const string &s) {
    vector<string> cmdBonus = split(s, ",", 2);

    bonus.emplace_back(stoi(cmdBonus.at(0)), stoi(cmdBonus.at(1))) ;
}

void MonRobotGPKD_v2::robot(const string &s) {
//    vector<string> command = split(s, " ", 2);
    vector<string> cmdRobot = split(s, ",", 2);

    radarR.emplace_back(stoi(cmdRobot.at(0)), stoi(cmdRobot.at(1))) ;
}

string MonRobotGPKD_v2::strategy() {
    vector<Position> vBonus;
    vector<Position> vRobot;
    for (int iLigne = -2; const vector<char> &ligne: board) {
        for (int iColonne = -2; char cellule: ligne) {
            if (cellule == 'B') {
                Position posBonus(iColonne, iLigne);
                vBonus.push_back(posBonus);
            }
            if (cellule == 'R' && (iLigne != 0 || iColonne != 0)) {
                Position posRobot(iColonne, iLigne);
                vRobot.push_back(posRobot);
            }
            iColonne++;
        }
        iLigne++;
    }

    Position maPosition(0, 0);
    Position posPlusProche;
    if (!vBonus.empty()) {

        posPlusProche = positionPlusProche(vBonus);
        return Message::actionMove(maPosition.directionTo(posPlusProche));
    }

    // Si le robot est en forme
    if (energy > 10 && (power > 3 && energy / 3 > degats)) {
        if (vRobot.empty()) {
            if (!bonus.empty()) {
                int min = bonus.at(0).getdX() + bonus.at(0).getdY();
                size_t posMin;
                // Le premier controle ne va jamais rentrer dans le int car est déjà dans le min
                for (size_t pos = 0; Direction b: bonus) {
                    if (b.getdX() + b.getdY() < min) {
                        min = b.getdX() + b.getdY();
                        posMin = pos;
                    }
                    pos++;
                }
                return Message::actionMove(maPosition.directionTo({bonus.at(posMin).getdX(), bonus.at(posMin).getdY()}));
            } else {
                return Message::actionMove(maPosition.directionTo(mouvDefaut));
            }

        } else {
            // Attack
            posPlusProche = positionPlusProche(vRobot);
            return Message::actionAttack(maPosition.directionTo(posPlusProche));
        }

    } else {
        if (vRobot.size() > 3) {
            // Attack
            posPlusProche = positionPlusProche(vRobot);
            return Message::actionAttack(maPosition.directionTo(posPlusProche));

        } else if (vRobot.empty()) {
            if (!radarR.empty()) {
                int min = radarR.at(0).getdX() + radarR.at(0).getdY();
                size_t posMin;
                // Le premier controle ne va jamais rentrer dans le int car est déjà dans le min
                for (size_t pos = 0; Direction rob: radarR) {
                    if (rob.getdX() + rob.getdY() < min) {
                        min = rob.getdX() + rob.getdY();
                        posMin = pos;
                    }
                    pos++;
                }
                return Message::actionMove(maPosition.directionTo({radarR.at(posMin).getdX() * -1, radarR.at(posMin).getdY() * -1}));
            }
            if (!bonus.empty()) {
                int min = bonus.at(0).getdX() + bonus.at(0).getdY();
                size_t posMin;
                // Le premier controle ne va jamais rentrer dans le int car est déjà dans le min
                for (size_t pos = 0; Direction b: bonus) {
                    if (b.getdX() + b.getdY() < min) {
                        min = b.getdX() + b.getdY();
                        posMin = pos;
                    }
                    pos++;
                }
                return Message::actionMove(maPosition.directionTo({bonus.at(posMin).getdX(), bonus.at(posMin).getdY()}));
            }
            return Message::actionMove(maPosition.directionTo(mouvDefaut));
        } else if (vRobot.size() == 2) {
            int dx, dy;
            dx = vRobot.at(0).getX() - vRobot.at(1).getX();
            dy = vRobot.at(0).getY() - vRobot.at(1).getY();
            mouvDefaut = {dy, dx * (-1)};
            return Message::actionMove(maPosition.directionTo(mouvDefaut));

        } else {
            Direction fuir = maPosition.directionTo(vRobot.at(0).neg());
            mouvDefaut = {fuir.getdX(),fuir.getdY()};
            return Message::actionMove(maPosition.directionTo(mouvDefaut));
        }
    }
}
/*if (derniereAction.find("move") != string::npos) {
            return derniereAction;
         } else {
            if (vRobot.empty()) {
                if (!radarR.empty()){
                    int min = radarR.at(0).getdX() + radarR.at(0).getdY();
                    size_t posMin;
                    // Le premier controle ne va jamais rentrer dans le int car est déjà dans le min
                    for (size_t pos = 0; Direction rob:radarR) {
                        if (rob.getdX() + rob.getdY() < min){
                            min = rob.getdX() + rob.getdY();
                            posMin = pos;
                        }
                        pos++;
                    }
                    return Message::actionMove(maPosition.directionTo({radarR.at(posMin).getdX() * -1, radarR.at(posMin).getdY() * - 1}));
                }
                if (!bonus.empty()) {
                    int min = bonus.at(0).getdX() + bonus.at(0).getdY();
                    size_t posMin;
                    // Le premier controle ne va jamais rentrer dans le int car est déjà dans le min
                    for (size_t pos = 0; Direction b: bonus) {
                        if (b.getdX() + b.getdY() < min) {
                            min = b.getdX() + b.getdY();
                            posMin = pos;
                        }
                        pos++;
                    }
                    return Message::actionMove(maPosition.directionTo({bonus.at(posMin).getdX(), bonus.at(posMin).getdY()}));
                }
               return Message::actionMove(maPosition.directionTo({-2, -2}));
            } else if (vRobot.size() == 2) {
               int dx, dy;
               dx = vRobot.at(0).getX() - vRobot.at(1).getX();
               dy = vRobot.at(0).getY() - vRobot.at(1).getY();
               return Message::actionMove(maPosition.directionTo({dy, dx * (-1)}));

            } else {
               return Message::actionMove(
                  maPosition.directionTo(vRobot.at(0).neg()));
            }
         }*/


Position MonRobotGPKD_v2::positionPlusProche(vector<Position> vPosition) {
    Position maPosition(0, 0);
    // On place le premier élément du tableau dans la variable du bonus le plus
    // proche
    Position posPlusProche = vPosition.at(0);
    double distPlusProche = maPosition.dist(vPosition.at(0));


    // Si il y a plusieur Bonus on va comparer les distances et on stocke
    // toujours la plus petite distance dans la variable posPlusProche
    if (vPosition.size() > 1) {
        for (size_t pos = 1; pos < vPosition.size(); pos++) {
            posPlusProche = distPlusProche < maPosition.dist(vPosition.at(pos)) ? posPlusProche : vPosition.at(pos);
        }
    }
    return posPlusProche;
}