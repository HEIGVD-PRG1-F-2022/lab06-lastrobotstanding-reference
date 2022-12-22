/*
-----------------------------------------------------------------------------------
Nom du fichier  : MonRobotGPKD.cpp
Auteur(s)       : Guilherme Pinto, Keya Dessasa
Date creation   : 29.11.2022

Description     : <à compléter>

Remarque(s)     : <à compléter>

Compilateur     : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
*/
#include "MonRobotGPKD.h"
#include "librobots/Message.h"
#include <iostream>

using namespace std;

// Initialisation de la variable static
unsigned MonRobotGPKD::nbRobot = 0;


enum class UPDATE{BOARD, DAMAGE, ENERGY, POWER, BONUS, ROBOT};

void MonRobotGPKD::setConfig(size_t widthInit, size_t heightInit,
                             unsigned energyInit, unsigned powerInit) {
   this->width = widthInit;
   this->height= heightInit;
   this->energy = energyInit;
   this->power = powerInit;
   vector<vector<char>> resize(NB_LIGNE,vector<char>(NB_COLONNES));
   board = resize;
   ++nbRobot;
}

string MonRobotGPKD::action(vector<string> updates) {
   const string ESPACE = " ";
//   size_t finEntete = 0;
   vector<vector<int>> vUpdates;
   vector<string> command;

   for (const string &update: updates) {
//      finEntete = update.find(ESPACE,finEntete);
//      selectionUpdate(finEntete, update);
      command = split(update, ESPACE, 2);
      vUpdates.push_back(selectionUpdate(command));
   }
   return strategy(vUpdates);
}

string MonRobotGPKD::name() const {
   return "MonRobotGPKD ";
}

vector<int> MonRobotGPKD::selectionUpdate(const vector<string> &
command) {
//   string enTete;
//   enTete =s.substr(0,finEntete + 1);
   vector<int> vUpdate;
   string enTete;
   enTete = command.at(0);

   if (enTete == "board") {
      vUpdate.push_back((int) UPDATE::BOARD);
      boardUpdate(command.at(1));
   } else if (enTete == "damage") {
      vUpdate.push_back((int) UPDATE::DAMAGE);
      return damageUpdate(command.at(1), vUpdate);
   } else if (enTete == "energy") {
      vUpdate.push_back((int) UPDATE::ENERGY);
      energyUpdate(command.at(1));
   } else if (enTete == "power") {
      vUpdate.push_back((int) UPDATE::POWER);
      powerUpdate(command.at(1));
   } else if (enTete == "bonus") {
      vUpdate.push_back((int) UPDATE::BONUS);
      bonusUpdate(command.at(1));
   } else if (enTete == "robot") {
      vUpdate.push_back((int) UPDATE::ROBOT);
      robot(command.at(1));
   } else {

   }
   return vUpdate;
}

void MonRobotGPKD::boardUpdate(const string &s) {

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

vector<int> MonRobotGPKD::damageUpdate(const string &s, std::vector<int> vDamage) {
   const string VIRGULE = ",";
   vector<string> cmdDamage = split(s, VIRGULE, 3);

   // Initialise le Vector vDamage [int posX, int posY, int degats]
   for (size_t i = 0; int &val: vDamage) {
      vDamage.push_back(stoi(cmdDamage.at(i)));
      ++i;
   }
   energy -= (unsigned) vDamage.back();

   return vDamage;
}

void MonRobotGPKD::energyUpdate(const string &s) {
   energy += (unsigned) stoi(s);
}

void MonRobotGPKD::powerUpdate(const string &s) {
   power += (unsigned) stoi(s);
}

/*Direction MonRobotGPKD::bonusUpdate(const string &s) {

}

Direction MonRobotGPKD::robot(const string &s) {

}*/

string MonRobotGPKD::strategy(vector<vector<int>> vUpdates) {
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

   } else if (vRobot.size() > 3) {
      // Attack
      posPlusProche = positionPlusProche(vRobot);
      return Message::actionAttack(maPosition.directionTo(posPlusProche));

   } else {
      // Fuir
      if (derniereAction.find("move") != string::npos) {
         return derniereAction;
      } else {
         if (vRobot.empty()) {
            return Message::actionMove(maPosition.directionTo({-2, -2}));
         } else if (vRobot.size() == 2) {
            int dx, dy;
            dx = vRobot.at(0).getX() - vRobot.at(1).getX();
            dy = vRobot.at(0).getY() - vRobot.at(1).getY();
            return Message::actionMove(maPosition.directionTo({dy, dx * (-1)}));

         } else {
            return Message::actionMove(maPosition.directionTo(vRobot.at(0).neg()));
         }
      }
   }

}

Position MonRobotGPKD::positionPlusProche(vector<Position> vPosition) {
   Position maPosition(0, 0);
   // On place le premier élément du tableau dans la variable du bonus le plus
   // proche
   Position posPlusProche = vPosition.at(0);
   double distPlusProche = maPosition.dist(vPosition.at(0));


   // Si il y a plusieur Bonus on va comparer les distances et on stocke
   // toujours la plus petite distance dans la variable posPlusProche
   if (vPosition.size() > 1) {
      for (size_t pos = 1; pos < vPosition.size(); pos++) {
         posPlusProche = distPlusProche < maPosition.dist(vPosition.at(pos)) ?
                         posPlusProche : vPosition.at(pos);
      }
   }
   return posPlusProche;
}