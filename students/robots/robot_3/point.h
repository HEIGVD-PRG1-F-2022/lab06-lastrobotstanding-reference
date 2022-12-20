/*
-----------------------------------------------------------------------------------
Project Name    : lab06-lastrobotstanding-comme_tu_veux
File's Name     : Point3.h
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

class Point3 {
    friend Point3 operator+(Point3 lhs, const Point3 &rhs);
public:
    int x, y;

    Point3();

    Point3(int x, int y) : x(x), y(y) {}

    explicit operator std::string() const;

    static Point3 fromStrToPoint3(const std::string& coords, const std::string& sep = ",");

    bool operator == (Point3 coords) const;

    Point3& operator += (const Point3& rhs);

    static Point3& wrap(Point3 &coords, int min, int max);

    int distance(Point3 coords) const;
};

#endif //ROBOT_POINT_H
