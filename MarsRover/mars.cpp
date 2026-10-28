
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
using namespace std;

enum class Direction { N, E, S, W };

class Plateau {
private:
    int maxX, maxY;

public:
    Plateau(int x, int y) : maxX(x), maxY(y) {
        if (x < 0 || y < 0)
            throw invalid_argument("Invalid plateau dimensions");
    }

    bool contains(int x, int y) const {
        return x >= 0 && x <= maxX &&
               y >= 0 && y <= maxY;
    }
};

class Rover {
private:
    int x, y;
    Direction direction;
    const Plateau& plateau;

    Direction turnLeft() const {
        switch (direction) {
            case Direction::N: return Direction::W;
            case Direction::W: return Direction::S;
            case Direction::S: return Direction::E;
            case Direction::E: return Direction::N;
        }
        throw logic_error("Invalid direction");
    }

    Direction turnRight() const {
        switch (direction) {
            case Direction::N: return Direction::E;
            case Direction::E: return Direction::S;
            case Direction::S: return Direction::W;
            case Direction::W: return Direction::N;
        }
        throw logic_error("Invalid direction");
    }

    void moveForward() {
        int nextX = x, nextY = y;

        switch (direction) {
            case Direction::N: ++nextY; break;
            case Direction::E: ++nextX; break;
            case Direction::S: --nextY; break;
            case Direction::W: --nextX; break;
        }

        if (plateau.contains(nextX, nextY)) {
            x = nextX;
            y = nextY;
        }
    }

public:
    Rover(int startX, int startY, Direction dir,
          const Plateau& p)
        : x(startX), y(startY), direction(dir), plateau(p) {
        if (!plateau.contains(x, y))
            throw invalid_argument("Starting position outside plateau");
    }

    void execute(const string& commands) {
        for (char command : commands) {
            switch (command) {
                case 'L':
                    direction = turnLeft();
                    break;
                case 'R':
                    direction = turnRight();
                    break;
                case 'M':
                    moveForward();
                    break;
                default:
                    throw invalid_argument("Invalid command");
            }
        }
    }

    char directionToChar() const {
        switch (direction) {
            case Direction::N: return 'N';
            case Direction::E: return 'E';
            case Direction::S: return 'S';
            case Direction::W: return 'W';
        }
        throw logic_error("Invalid direction");
    }

    void display() const {
        cout << x << " " << y << " "
             << directionToChar() << '\n';
    }
};

Direction parseDirection(char c) {
    switch (c) {
        case 'N': return Direction::N;
        case 'E': return Direction::E;
        case 'S': return Direction::S;
        case 'W': return Direction::W;
        default:
            throw invalid_argument("Invalid starting direction");
    }
}

int main() {
    try {
        int maxX, maxY;
        if (!(cin >> maxX >> maxY))
            return 0;

        Plateau plateau(maxX, maxY);

        int x, y;
        char dir;
        string commands;

        // Process rover inputs until EOF.
        while (cin >> x >> y >> dir >> commands) {
            Rover rover(x, y, parseDirection(dir), plateau);
            rover.execute(commands);
            rover.display();
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
