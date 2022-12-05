//
// Created by Linus Gasser on 01.12.22.
//

#ifndef INEITI_POSITION_H
#define INEITI_POSITION_H

#include <iostream>
#include <string>
#include "Direction.h"

/**
 * Represents a Position either with wraparound or without. If a width and height
 * are given, the + and - operations will wrap around.
 */
class Position {
    int x{}, y{};
    size_t width = 0, height = 0;
    /// Addition
    friend Position operator+(Position pos, const Position &other);
    /// Subtraction
    friend Position operator-(Position pos, const Position &other);
    /// Adding a Direction
    friend Position operator+(Position pos, const Direction &other);
    /// Subtracting a Direction
    friend Position operator-(Position pos, const Direction &other);
    /// Adding to a Direction
    friend Position operator+(const Direction &dir, Position other);
    /// Subtracting from a Direction
    friend Position operator-(const Direction &dir, Position other);
    /// Equality operator
    friend bool operator==(const Position &p1, const Position &p2);
    /// Output operator
    friend std::ostream &operator<<(std::ostream &out, const Position &pos);

public:
    /// Default constructor.
    Position();
    /**
     * Constructor with optional width and height. If given, the Position will always be
     * calculated modulo the width and the height, making the board wrap around on the
     * sides.
     * @param x position
     * @param y position
     * @param width if given, will wrap around
     * @param height if given, will wrap around
     */
    Position(int x, int y, size_t width = 0, size_t height = 0);

    Position &operator+=(const Position &pos);
    Position &operator-=(const Position &pos);
    Position &operator+=(const Direction &dir);
    Position &operator-=(const Direction &dir);

    [[nodiscard]] int getX() const;
    [[nodiscard]] int getY() const;

    Position &wrap();
    Position &closest();
    Position &neg();

    static Position random(unsigned xMax, unsigned yMax, size_t width = 0, size_t height = 0);

    [[nodiscard]] double mag() const;
    [[nodiscard]] double dist(const Position &other) const;
    [[nodiscard]] Direction directionTo(const Position &other) const;
    [[nodiscard]] size_t indexSquare(size_t side) const;
};

void testPosition();

#endif//INEITI_POSITION_H
