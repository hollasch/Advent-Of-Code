//----------------------------------------------------------------------------------------------------------------------
// 2023 Advent of Code -- Puzzle 10a
//
// --- Day 10: Pipe Maze ---
//
// You use the hang glider to ride the hot air from Desert Island all the way up to the floating metal island. This
// island is surprisingly cold and there definitely aren't any thermals to glide on, so you leave your hang glider
// behind.
//
// You wander around for a while, but you don't find any people or animals. However, you do occasionally find signposts
// labeled "Hot Springs" pointing in a seemingly consistent direction; maybe you can find someone at the hot springs and
// ask them where the desert-machine parts are made.
//
// The landscape here is alien; even the flowers and trees are made of metal. As you stop to admire some metal grass,
// you notice something metallic scurry away in your peripheral vision and jump into a big pipe! It didn't look like any
// animal you've ever seen; if you want a better look, you'll need to get ahead of it.
//
// Scanning the area, you discover that the entire field you're standing on is densely packed with pipes; it was hard to
// tell at first because they're the same metallic silver color as the "ground". You make a quick sketch of all of the
// surface pipes you can see (your puzzle input).
//
// The pipes are arranged in a two-dimensional grid of tiles:
//
//   - | is a vertical pipe connecting north and south.
//   - - is a horizontal pipe connecting east and west.
//   - L is a 90-degree bend connecting north and east.
//   - J is a 90-degree bend connecting north and west.
//   - 7 is a 90-degree bend connecting south and west.
//   - F is a 90-degree bend connecting south and east.
//   - . is ground; there is no pipe in this tile.
//   - S is the starting position of the animal; there is a pipe on this tile, but your sketch doesn't show what shape
//     the pipe has.
//
// Based on the acoustics of the animal's scurrying, you're confident the pipe that contains the animal is one large,
// continuous loop.
//
// For example, here is a square loop of pipe:
//
//     .....
//     .F-7.
//     .|.|.
//     .L-J.
//     .....
//
// If the animal had entered this loop in the northwest corner, the sketch would instead look like this:
//
//     .....
//     .S-7.
//     .|.|.
//     .L-J.
//     .....
//
// In the above diagram, the S tile is still a 90-degree F bend: you can tell because of how the adjacent pipes connect
// to it.
//
// Unfortunately, there are also many pipes that aren't connected to the loop! This sketch shows the same loop as above:
//
//     -L|F7
//     7S-7|
//     L|7||
//     -L-J|
//     L|-JF
//
// In the above diagram, you can still figure out which pipes form the main loop: they're the ones connected to S, pipes
// those pipes connect to, pipes those pipes connect to, and so on. Every pipe in the main loop connects to its two
// neighbors (including S, which will have exactly two pipes connecting to it, and which is assumed to connect back to
// those two pipes).
//
// Here is a sketch that contains a slightly more complex main loop:
//
//     ..F7.
//     .FJ|.
//     SJ.L7
//     |F--J
//     LJ...
//
// Here's the same example sketch with the extra, non-main-loop pipe tiles also shown:
//
//     7-F7-
//     .FJ|7
//     SJLL7
//     |F--J
//     LJ.LJ
//
// If you want to get out ahead of the animal, you should find the tile in the loop that is farthest from the starting
// position. Because the animal is in the pipe, it doesn't make sense to measure this by direct distance. Instead, you
// need to find the tile that would take the longest number of steps along the loop to reach from the starting point -
// regardless of which way around the loop the animal went.
//
// In the first example with the square loop:
//
//     .....
//     .S-7.
//     .|.|.
//     .L-J.
//     .....
//
// You can count the distance each tile in the loop is from the starting point like this:
//
//     .....
//     .012.
//     .1.3.
//     .234.
//     .....
//
// In this example, the farthest point from the start is 4 steps away.
//
// Here's the more complex loop again:
//
//     ..F7.
//     .FJ|.
//     SJ.L7
//     |F--J
//     LJ...
//
// Here are the distances for each tile on that loop:
//
//     ..45.
//     .236.
//     01.78
//     14567
//     23...
//
// Find the single giant loop starting at S. How many steps along the loop does it take to get from the starting
// position to the point farthest from the starting position?
//
//----------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>

using namespace std;


using Pipe = uint8_t;
const Pipe PipeUp    = 0x01;
const Pipe PipeDown  = 0x02;
const Pipe PipeLeft  = 0x04;
const Pipe PipeRight = 0x08;
const Pipe PipeStart = 0x80;


Pipe getPipeFromChar(char c) {
    switch (c) {
        case '|': return PipeUp   | PipeDown;
        case '-': return PipeLeft | PipeRight;
        case 'L': return PipeUp   | PipeRight;
        case 'J': return PipeUp   | PipeLeft;
        case '7': return PipeDown | PipeLeft;
        case 'F': return PipeDown | PipeRight;
        case 'S': return PipeStart;
    }

    return 0;
}

class PipeMap {
  public: 
    PipeMap (const vector<string>& mapText) {
        width = mapText[0].size();
        height = mapText.size();

        pipes = make_unique<Pipe[]>(width * height);

        // Copy map data into buffer.

        for (int y = 0; y < height; ++y) {
            const string& line = mapText[y];
            for (int x = 0; x < width; ++x) {
                auto p = getPipeFromChar(line[x]);
                if (p == PipeStart) {
                    startX = x;
                    startY = y;
                }
                (*this)(x, y) = p;
            }
        }
    }

    Pipe& operator()(int x, int y) {
        return pipes[y*width + x];
    }

    const Pipe& operator()(int x, int y) const {
        return pipes[y*width + x];
    }

    void dump() const {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                const Pipe& pipe = (*this)(x, y);
                switch (pipe) {
                    case PipeUp    | PipeRight: cout << 'L'; break;
                    case PipeUp    | PipeDown:  cout << '|'; break;
                    case PipeUp    | PipeLeft:  cout << 'J'; break;
                    case PipeRight | PipeDown:  cout << 'F'; break;
                    case PipeRight | PipeLeft:  cout << '-'; break;
                    case PipeDown  | PipeLeft:  cout << '7'; break;
                    case PipeStart:             cout << 'S'; break;

                    case 0:  cout << '.'; break;
                    default: cout << '@'; break;
                }
            }
            cout << '\n';
        }
    }

    int width;
    int height;
    int startX;
    int startY;

  private:
    unique_ptr<Pipe[]> pipes;
};


class Runner {
  public:
    Runner() : x(0), y(0), distance(0), direction(PipeStart) {}

    void Reset(int x, int y, Pipe direction) {
        this->x = x;
        this->y = y;
        this->distance = 0;
        this->direction = direction;
    }

    void Advance(PipeMap& pipeMap) {
        switch (direction) {
            case PipeUp:
                --y;
                direction = pipeMap(x,y) & ~PipeDown;
                break;
            case PipeDown:
                ++y;
                direction = pipeMap(x,y) & ~PipeUp;
                break;
            case PipeLeft:
                --x;
                direction = pipeMap(x,y) & ~PipeRight;
                break;
            case PipeRight:
                ++x;
                direction = pipeMap(x,y) & ~PipeLeft;
                break;
        }
        ++distance;
    }

    int  x;
    int  y;
    int  distance;
    Pipe direction;
};


int maxDistanceFromStart(PipeMap& pipeMap) {
    bool haveRunner1 = false;

    Runner runner1, runner2;
    int startX = pipeMap.startX;
    int startY = pipeMap.startY;
    int width = pipeMap.width;
    int height = pipeMap.height;
    Pipe pipe {0};

    if (startY > 0 && (pipeMap(startX, startY-1) & PipeDown)) {
        runner1.Reset(startX, startY, PipeUp);
        haveRunner1 = true;
    }

    if (startY < pipeMap.height && (pipeMap(startX, startY+1) & PipeUp)) {
        if (haveRunner1) {
            runner2.Reset(startX, startY, PipeDown);
        } else {
            runner1.Reset(startX, startY, PipeDown);
            haveRunner1 = true;
        }
    }

    if (startX > 0 && (pipeMap(startX-1, startY) & PipeRight)) {
        if (haveRunner1) {
            runner2.Reset(startX, startY, PipeLeft);
        } else {
            runner1.Reset(startX, startY, PipeLeft);
            haveRunner1 = true;
        }
    }

    if (startX < height && (pipeMap(startX+1, startY) & PipeLeft)) {
        if (haveRunner1) {
            runner2.Reset(startX, startY, PipeRight);
        } else {
            runner1.Reset(startX, startY, PipeRight);
            haveRunner1 = true;
        }
    }

    while (true) {
        runner1.Advance(pipeMap);
        if (runner1.x == runner2.x && runner1.y == runner2.y)
            break;
        runner2.Advance(pipeMap);
        if (runner1.x == runner2.x && runner1.y == runner2.y)
            break;
    }

    return runner1.distance;
}

int main() {
    vector<string> mapText;
    string line;

    while (getline(cin, line))
        mapText.push_back(line);

    // for (auto& line : mapText)
    //     cout << '(' << line << ")\n";

    // cout << '\n';

    PipeMap pipeMap(mapText);

    // pipeMap.dump();

    cout << "Maximum distance from start: " << maxDistanceFromStart(pipeMap) << '\n';

    return 0;
}
