/*
-----------------------------------------------------------------------------------
Nom du fichier  : MonRobotGPKD.h
Auteur(s)       : Guilherme Pinto, Keya Dessasa
Date creation   : 29.11.2022

Description     : <à compléter>

Remarque(s)     : <à compléter>

Compilateur     : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
*/

#ifndef REFACTOR_MONROBOTGPKD_H
#define REFACTOR_MONROBOTGPKD_H

#include <cstdlib>
#include <string>
#include <vector>
#include "librobots/Position.h"
#include "librobots/Robot.h"
#include "librobots/Direction.h"

class MonRobotGPKD : public Robot{
public:

   void setConfig(size_t width, size_t height, unsigned energy, unsigned power);

   std::string action(std::vector<std::string> updates);

   std::string name()const;

private:
   void selectionUpdate(const std::vector<std::string> &command);

   static std::vector<std::vector<char>> board(const std::string& s);
   std::vector<int> damage(const std::string &s);
   void energyUpdate(const std::string &s);
   void powerUpdate(const std::string &s);
   Direction bonus(const std::string &s);
   Direction robot(const std::string &s);

   size_t width, height;
   unsigned energy, power;
};
#endif //REFACTOR_MONROBOTGPKD_H
