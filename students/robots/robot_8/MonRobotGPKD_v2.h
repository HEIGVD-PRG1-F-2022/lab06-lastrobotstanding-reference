/*
-----------------------------------------------------------------------------------
Nom du fichier  : MonRobotGPKD_v2.h
Auteur(s)       : Guilherme Pinto da Cunha da Mata
Date creation   : 20.12.2022

Description     : <à compléter>

Remarque(s)     : <à compléter>

Compilateur     : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
*/

#ifndef LABO6_ROBOTS_V2_MONROBOTGPKD_V2_H
#define LABO6_ROBOTS_V2_MONROBOTGPKD_V2_H

#include <cstdlib>
#include <string>
#include <vector>
#include "librobots/Position.h"
#include "librobots/Robot.h"
#include "librobots/Direction.h"

class MonRobotGPKD_v2 : public Robot{
public:

   void setConfig(size_t width, size_t height, unsigned energy, unsigned power);

   std::string action(std::vector<std::string> updates);

   std::string name()const;

private:
   void selectionUpdate(const std::vector<std::string> &command);

   void boardUpdate(const std::string& s);
   void damageUpdate(const std::string &s);
   void energyUpdate(const std::string &s);
   void powerUpdate(const std::string &s);
   void bonusUpdate(const std::string &s);
   void robot(const std::string &s);
   std::string strategy();
   static Position positionPlusProche(std::vector<Position> vPosition);

   size_t width, height;
   unsigned energy, power, degats;
   std::vector<Direction> bonus, radarR;
   Position mouvDefaut = {-2,-2};
   static unsigned nbRobot;
   std::vector<std::vector<char>> board;
   std::string derniereAction;

   const size_t NB_COLONNES = 5, NB_LIGNE = 5;

};


#endif //LABO6_ROBOTS_V2_MONROBOTGPKD_V2_H
