// ================================================================================
// File : Point2D.cpp
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz, Antoine Leresche
// File created by Florian Duruz on 30/11/2022.
// ================================================================================

#include "Point2D.h"

Point2D operator+(Point2D lhs, const Point2D &rhs) {
    lhs += rhs;
    return lhs;
}

Point2D operator-(Point2D lhs, const Point2D &rhs) {
    lhs -= rhs;
    return lhs;
}

bool operator==(const Point2D &lhs, const Point2D &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

Point2D &Point2D::operator+=(const Point2D &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Point2D &Point2D::operator-=(const Point2D &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

double Point2D::mag() const {
    return hypot(x, y);
}

double Point2D::dist(const Point2D &rhs) const {
    return (rhs - *this).mag();
}

std::ostream &operator<<(std::ostream &lhs, const Point2D &rhs) {
    lhs << rhs.x << ',' << rhs.y;
    return lhs;
}

long int Point2D::getX() const {
    return x;
}

long int Point2D::getY() const {
    return y;
}

Point2D Point2D::normalize() const {
    long int xnorm = x == 0 ? 0 : (1 / sqrt(x * x) * x);
    long int ynorm = y == 0 ? 0 : (1 / sqrt(y * y) * y);
    return Point2D(xnorm, ynorm);
}
