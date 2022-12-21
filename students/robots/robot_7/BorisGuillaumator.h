#ifndef BORISGUILLAUMATOR_H
#define BORISGUILLAUMATOR_H

#include <iostream>
#include <vector>
#include <array>
#include "librobots/Robot.h"

class BorisGuillaumator : public Robot {
    const std::string borisGuillaumatorName = "BorisGuillaumator";
    size_t width;
    size_t height;
    unsigned energy;
    unsigned power;

    static int coordToDirection(int x);

public:
    void setConfig(size_t width_p, size_t height, unsigned energy, unsigned power) override;

    std::string action(std::vector<std::string> updates) override;

    [[nodiscard]] std::string name() const override;

    static std::vector<std::string> split(std::string str, const char *delim);
};


#endif