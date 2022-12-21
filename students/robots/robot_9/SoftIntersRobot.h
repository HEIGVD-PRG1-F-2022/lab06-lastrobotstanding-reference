//
// Created by bioan on 29.11.2022.
//

#ifndef LAB06_LASTROBOTSTANDING_SOFT_INTERS_SOFTINTERSROBOT_H
#define LAB06_LASTROBOTSTANDING_SOFT_INTERS_SOFTINTERSROBOT_H

#include <string>
#include <vector>
#include "librobots/Robot.h"
// Classe de notre robot à nous
class SoftIntersRobot : public Robot{
public:
    // Constructeur par défaut de notre robot
    SoftIntersRobot() : hauteurBoard(0), largeurBoard(0), hp(0), dmgPower(0) {};
    //
    void setConfig(size_t width, size_t height, unsigned energy,
                   unsigned power) override;
    // Evolution des diffèrentes actions des robots
    std::string action(std::vector<std::string> updates) override;
    // Nom des robots
    [[nodiscard]] std::string name() const override;

private:
    // Changement du robot
    void robotChanges(const std::vector<std::string> &updates);

    size_t hauteurBoard;
    size_t largeurBoard;
    unsigned hp;
    unsigned dmgPower;
    const std::string NAME_ROBOT = "SoftInters";

};


#endif //LAB06_LASTROBOTSTANDING_SOFT_INTERS_SOFTINTERSROBOT_H
