/*
  ---------------------------------------------------------------------------
  Fichier     : SoftIntersRobot.h
  Nom du labo : lastRobotStanding
  Auteur(s)   : Alexander Bouriot et Arthur Junod
  Date        : 23.12.2022

  Le robot que nous avons fait.
  ---------------------------------------------------------------------------
*/

#ifndef LAB06_LASTROBOTSTANDING_SOFT_INTERS_SOFTINTERSROBOT_H
#define LAB06_LASTROBOTSTANDING_SOFT_INTERS_SOFTINTERSROBOT_H

#include <string>
#include <vector>
#include "librobots/Robot.h"
#include "librobots/Message.h"
// Classe de notre robot à nous
class SoftIntersRobot : public Robot{
public:

    /**
     * Le constructeur par défaut de notre robot.
     */
    SoftIntersRobot() : hauteurBoard(0), largeurBoard(0), hp(0), dmgPower(0) {};

    /**
     * Permet de changer la configuration de notre robot.
     *
     * @param width Largeur du tableau de jeu de notre robot.
     * @param height Hauteur du tableau de jeu de notre robot.
     * @param energy Les hps de notre robot.
     * @param power Les dégats que peut mettre notre robot au autres.
     */
    void setConfig(size_t width, size_t height, unsigned energy,
                   unsigned power) override;

    /**
     * Définit la logique de notre robot.
     *
     * @param updates Les informations envoyées par le jeu à notre robot.
     * @return Les actions que notre robot a décidés de faire.
     */
    std::string action(std::vector<std::string> updates) override;

    /**
     * Permet de récupérer le nom de notre robot.
     *
     * @return Le nom du robot.
     */
    [[nodiscard]] std::string name() const override;

private:
    //Les dimensions du tableau de jeu
    size_t hauteurBoard;
    size_t largeurBoard;
    //Les statistique de notre robot.
    unsigned hp;
    unsigned dmgPower;
    //Le nom du robot
    const std::string NAME_ROBOT = "SoftInters";

};


#endif //LAB06_LASTROBOTSTANDING_SOFT_INTERS_SOFTINTERSROBOT_H
