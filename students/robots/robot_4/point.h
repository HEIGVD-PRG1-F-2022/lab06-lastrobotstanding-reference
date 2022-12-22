/*
-----------------------------------------------------------------------------------
Project Name    : lab06-lastrobotstanding-comme_tu_veux
File's Name     : Point.h
Author          :   Aellen Quentin
                    Atasever Mehmet
                    Salamin Chloé
Creation Date   : 02.12.2022
Description     :
Remarque(s)     :
Compiler        : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
*/

#ifndef ROBOT_POINT_H
#define ROBOT_POINT_H

#include <string>

class Point {
    friend Point operator+(Point lhs, const Point &rhs);
    friend Point operator-(Point lhs, const Point &rhs);
public:
    int x, y;

    Point();

    Point(int x, int y) : x(x), y(y) {}
    Point(const std::string& x, const std::string& y) : x(std::stoi(x)), y(std::stoi(y)) {}

    explicit operator std::string() const;

    Point normalize();

    static Point fromStrToPoint(const std::string& coords, const std::string& sep = ",");

    bool operator == (Point coords) const;

    Point& operator += (const Point& rhs);

    Point& operator -= (const Point& rhs);

    static Point& wrap(Point &coords, int min, int max);

    int distance(Point coords) const;
};

#endif //ROBOT_POINT_H
