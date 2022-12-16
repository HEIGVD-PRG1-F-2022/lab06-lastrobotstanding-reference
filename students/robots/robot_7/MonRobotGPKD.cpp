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

void MonRobotGPKD::setConfig(size_t widthInit, size_t heightInit,
                             unsigned energyInit, unsigned powerInit) {
   this->width = widthInit;
   this->height= heightInit;
   this->energy = energyInit;
   this->power = powerInit;
}

string MonRobotGPKD::action(vector<string> updates){
   const string ESPACE = " ";
//   size_t finEntete = 0;
   vector<string> command;
   for (const string& update: updates) {
//      finEntete = update.find(ESPACE,finEntete);
//      selectionUpdate(finEntete, update);
      command = split(update, ESPACE, 2);
      //selectionUpdate(command);
   }

}

string MonRobotGPKD::name()const{

}

void MonRobotGPKD::selectionUpdate(const vector<string> &
command) {
//   string enTete;
//   enTete =s.substr(0,finEntete + 1);
   string enTete;
   enTete = command.at(0);

   if(enTete == "board"){
      board(command.at(1));
   }else if (enTete == "damage"){
      damage(command.at(1));
   }else if (enTete == "energy"){
      energyUpdate(command.at(1));
   }else if(enTete == "power"){
      powerUpdate(command.at(1));
   }else if(enTete == "bonus"){
      bonus(command.at(1));
   }else if (enTete == "robot"){
      robot(command.at(1));
   } else{

   }
}

vector<vector<char>> MonRobotGPKD::board(const string &s) {
   size_t NB_COLONNES = 5, NB_LIGNE = 5;
   vector<vector<char>> visionRobot(NB_LIGNE, vector<char>(NB_COLONNES));

   // Vérification que la chaîne soit de la bonne taille
   if (NB_COLONNES * NB_LIGNE == s.length()) {
      size_t j = 0;
      for (size_t i = 0; i < s.length(); ++i) {
         visionRobot.at(j).at(i % NB_COLONNES) = s.at(i);
         if (i % NB_COLONNES == NB_COLONNES - 1) {
            ++j;
         }
      }
   }
   return visionRobot;
}

vector<int> MonRobotGPKD::damage(const string &s) {
   const string VIRGULE = ",";
   vector<string> cmdDamage = split(s, VIRGULE, 3);

   vector<int> vDamage(3);

   // Initialise le Vector vDamage [int posX, int posY, int degats]
   for (size_t i = 0; int &val : vDamage) {
       val = stoi(cmdDamage.at(i));
       ++i;
   }
   energy -= (unsigned)vDamage.at(2);

   return vDamage;
}

void MonRobotGPKD::energyUpdate(const string &s){
   energy += (unsigned)stoi(s);
}

void MonRobotGPKD::powerUpdate(const string &s){
   power += (unsigned)stoi(s);
}

Direction MonRobotGPKD::bonus(const string &s){
   
}

Direction MonRobotGPKD::robot(const string &s){

}


