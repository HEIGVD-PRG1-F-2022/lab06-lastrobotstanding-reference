/*
-----------------------------------------------------------------------------------
Nom du fichier  : MonRobot.h
Auteur(s)       : Guillaume Dunant, David Berger
Date creation   : 29.11.2022

Description     : LastRobotStanding

Remarque(s)     : <a completer>

-----------------------------------------------------------------------------------
*/

#ifndef MONROBOT_H
#define MONROBOT_H

#include <vector>
#include <string>
#include <cstdlib>
#include "librobots/Robot.h"

class MonRobot : public Robot {
public:
    // Créer un nouveau robot et indique la taille d1u terrain de jeu ainsi que
    // l’énergie initiale du robot et la puissance de la frappe.
    MonRobot();
    // Une étape dans le jeu: les ‘updates’ sont des informations sur 
    // l’environnement du robot. Le robot retourne un ou plusieurs string
    // pour indiquer ses actions.
    std::string action(std::vector<std::string> updates) override;
    // Retourne le nom de ce robot.
    [[nodiscard]] std::string name() const override;
    /**
     * fonction qui permet la config du Robot
     * @param width_init largeur du terrain de jeu
     * @param height_init hauteur du terrain de jeu
     * @param energy_init energie/vie du robot
     * @param power_init puissance d'attaque du robot
     */
    void setConfig(size_t width_init, size_t height_init, unsigned energy_init,
                   unsigned power_init) override;

private:
    ////////////////////Variables membres////////////////////

    /**
     * nom du robot
     */
    std::string str_name;

    /**
     * largeur du terrain de jeu
     * variable non-utilisée car nous n'avons pas fait de radar
     */
    [[maybe_unused]] size_t dx;

    /**
     * hauteur du terrain de jeu
     * variable non-utilisée car nous n'avons pas fait de radar
     */
    [[maybe_unused]] size_t dy;

    /**
     * puissance d'attaque du robot
     */
    int pow;

    /**
     * energie/vie du robot
     */
    int life;

    /**
     * fonction d'attaque
     * @param xMove coordonnée x où attaquer
     * @param yMove coordonnée y où attaquer
     * @return le string "attack x, y"
     */
    static std::string attack(int xMove, int yMove);

    /**
     * fonction de déplacement
     * @param xMove coordonnée x
     * @param yMove coordonnée y
     * @return le string "move x, y"
     */
    static std::string move(int xMove, int yMove);

    /**
     * fonction qui détecte les robots et/ou bonus dans les 25 cases autour
     * @param board le tableau
     * @param objectSymbol un char 'R' ou 'B' pour savoir si on recherche un robot ou un bonus
     * @return un tableau vector<vector<int>> avec les coordonées des robots ou celles des bonus
     */
    static std::vector<std::vector<int>> objectNear(const std::vector<std::vector<char>>& board, char objectSymbol);

    /**
     * Toutes des fonctions qui permettent de move dans une certaine direction
     * @return le string "move x, y"
     *
     * les fonctions pas utilisées sont gardées au cas où on changerait de stratégies
     */
    [[maybe_unused]] static std::string goForward();

    [[maybe_unused]] static std::string goBackward();

    [[maybe_unused]] static std::string goLeft();

    [[maybe_unused]] static std::string goRight();

    static std::string goUpLeft();

    [[maybe_unused]] static std::string goUpRight();

    static std::string goBackLeft();

    static std::string goBackRight();

    /**
     * fonction qui permet d'attaquer le robot le plus proche
     * @param enemyCoordinates un vector<vector<int>> qui ont les coordonnées des autres robot dans les 25 cases autour
     * @return le string "attack x, y"
     */
    static std::string fightFirstTarget(const std::vector<std::vector<int>> &enemyCoordinates);

    /**
     * fonction qui permet de transformer le string d'update en un tableau
     * @return le tableau en vector<vector<int>>
     */
    static std::vector<std::vector<char>> boardStringToVectors(const std::string &);

    /**
     * fonction qui fuit le robot le plus proche
     * @param enemyCoordinates un vector<vector<int>> qui ont les coordonnées des autres robot dans les 25 cases autour
     * @return le string "move x, y"
     */
    static std::string fleeRobot(const std::vector<std::vector<int>> &enemyCoordinates);

    int getRandomNumber(int max);

};


#endif //MONROBOT_H
