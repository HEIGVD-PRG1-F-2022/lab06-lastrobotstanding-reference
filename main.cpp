#include "Game.h"
#include "robot/RobotSmart.h"
#include "robot/RobotsSimple.h"
#include <iostream>
#include <map>
#include <random>

using namespace std;

vector<Robot *> students();

class InputParser {
public:
    InputParser(int &argc, char **argv) {
        for (int i = 1; i < argc; ++i) this->tokens.emplace_back(argv[i]);
    }
    /// @author iain
    [[nodiscard]] const std::string &getCmdOption(const std::string &option) const {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()) { return *itr; }
        static const std::string empty_string;
        return empty_string;
    }
    /// @author iain
    [[nodiscard]] bool cmdOptionExists(const std::string &option) const {
        return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
    }

private:
    std::vector<std::string> tokens;
};

enum PresenceSmart {
    NO,
    YES,
    RANDOM,
};

struct cfg {
    cfg(int argc, char *argv[]) {
        InputParser parser(argc, argv);
        standard = parser.cmdOptionExists("--standard");
        rounds = parser.cmdOptionExists("--rounds") ? stoul(parser.getCmdOption("--rounds")) : 1;
        smart = parser.cmdOptionExists("--smart") ? YES : NO;
    }
    bool standard;
    unsigned long rounds;
    PresenceSmart smart;
};

Game setupGame(cfg c) {
    Game G(c.standard);
    G.addRobot(new RobotDiagonal());
    bool addSmart = c.smart == YES;
    if (c.smart == RANDOM) {
        std::random_device rd;
        std::uniform_int_distribution<bool> w(false, true);
        addSmart = w(rd);
    }
    if (addSmart) { G.addRobot(new RobotSmart()); }
    G.addRobot(new RobotWait());
    for (const auto &robot: students()) { G.addRobot(robot); }
    return G;
}

struct Wins {
    string name;
    size_t wins;
};

void gameSilent(cfg c) {
    map<string, size_t> winners;
    Display::init();
    Display::clearScreen();
    size_t draws = 0;
    vector<string> deaths;

    for (size_t round = 0; round < c.rounds; round++) {
        Game G = setupGame(c);
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

void game(cfg c) {
    if (c.smart == NO){
        c.smart = RANDOM;
    }
    Game G = setupGame(c);
    auto winner = G.play(true);
    cout << Display::DString(Display::Color::YELLOW);
    if (winner == nullptr) {
        cout << "The game ended in a draw" << endl;
    } else {
        cout << "Winner is: " << winner->getName() << endl;
    }
    Display::DString().resetColor().print();
}

int main(int argc, char *argv[]) {
    cfg c(argc, argv);
    if (c.rounds == 1) {
        game(c);
    } else {
        gameSilent(c);
    }
    return 0;
}
