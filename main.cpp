#include "Game.h"
#include "robot/RobotSmart.h"
#include "robot/RobotsSimple.h"
#include <iostream>
#include <map>
#include <cstring>

using namespace std;

vector<Robot *> students();

Game setupGame() {
    Game G;
    G.addRobot(new RobotDiagonal());
    G.addRobot(new RobotSmart());
    G.addRobot(new RobotWait());
    for (const auto &robot: students()) { G.addRobot(robot); }
    return G;
}

struct Wins {
    string name;
    size_t wins;
};

void gameSilent(size_t rounds) {
    map<string, size_t> winners;
    Display::init();
    Display::clearScreen();
    size_t draws = 0;

    for (size_t round = 0; round < rounds; round++) {
        Game G = setupGame();
        auto winner = G.play(false);
        if (winner == nullptr) {
            draws++;
        } else {
            string name = winner->getName();
            if (winners.contains(name)) {
                winners.insert_or_assign(name, winners.at(name) + 1);
            } else {
                winners.insert_or_assign(name, 1);
            }
        }
        vector<Wins> wins;
        for (const auto &n: winners) { wins.push_back(Wins{n.first, n.second}); }
        sort(wins.begin(), wins.end(), [](const Wins &a, const Wins &b) { return a.wins > b.wins; });
        Display::DString()
                .clearScreen()
                .setColor(Display::Color::GREEN)
                .append("Round: ")
                .append(round)
                .append(" - Draws: ")
                .append(draws)
                .append("\n")
                .append(Display::Color::YELLOW)
                .print();
        for (const auto &w: wins) { cout << "Robot: " << w.name << " - victories: " << w.wins << endl; }
    }
}

void game() {
    Game G = setupGame();
    auto winner = G.play(false);
    cout << Display::DString(Display::Color::YELLOW);
    if (winner == nullptr) {
        cout << "The game ended in a draw" << endl;
    } else {
        cout << "Winner is: " << winner->getName() << endl;
    }
    Display::DString().resetColor().print();
}

int main(int argc, char *argv[]) {
    size_t rounds = 1;
    if (argc == 3 && strcmp(argv[1], "--rounds") == 0) { rounds = stoul(argv[2]); }
    if (rounds == 1) {
        game();
    } else {
        gameSilent(rounds);
    }
    return 0;
}
