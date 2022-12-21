// ================================================================================
// File : Point2D.h
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz
// File created by Florian Duruz on 30/11/2022.
// ================================================================================

#ifndef LASTROBOTSTANDING_POINT2D_H
#define LASTROBOTSTANDING_POINT2D_H

#include <cassert>
#include <cmath>
#include <iostream>

class Point2D {
    long int x = 0;
    long int y = 0;

    //friend Point2D operator=(Point2D& lhs, const Point2D& rhs);
    friend Point2D operator+(Point2D lhs, const Point2D &rhs);
    friend Point2D operator-(Point2D lhs, const Point2D &rhs);
    friend bool operator==(const Point2D &lhs, const Point2D &rhs);
    friend std::ostream &operator<<(std::ostream &lhs, const Point2D &rhs);

public:
    Point2D() : x(0), y(0){};
    Point2D(long int x, long int y) : x(x), y(y){};

    Point2D &operator+=(const Point2D &rhs);
    Point2D &operator-=(const Point2D &rhs);
    //Point2D& operator=( const Point2D& rhs);
    double mag() const;

    double dist(const Point2D &rhs) const;

    long int getX() const;
    long int getY() const;

    Point2D normalize() const;
};

#endif//LASTROBOTSTANDING_POINT2D_H
