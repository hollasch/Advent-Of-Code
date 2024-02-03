//----------------------------------------------------------------------------------------------------------------------
// 2023 Advent of Code -- Puzzle 10b (See Part Two below)
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
// --- Part Two ---
//
// You quickly reach the farthest point of the loop, but the animal never emerges. Maybe its nest is within the area
// enclosed by the loop?
//
// To determine whether it's even worth taking the time to search for such a nest, you should calculate how many tiles
// are contained within the loop. For example:
//
// ...........
// .S-------7.
// .|F-----7|.
// .||.....||.
// .||.....||.
// .|L-7.F-J|.
// .|..|.|..|.
// .L--J.L--J.
// ...........
//
// The above loop encloses merely four tiles - the two pairs of . in the southwest and southeast (marked I below). The
// middle . tiles (marked O below) are not in the loop. Here is the same loop again with those regions marked:
//
// ...........
// .S-------7.
// .|F-----7|.
// .||OOOOO||.
// .||OOOOO||.
// .|L-7OF-J|.
// .|II|O|II|.
// .L--JOL--J.
// .....O.....
//
// In fact, there doesn't even need to be a full tile path to the outside for tiles to count as outside the loop -
// squeezing between pipes is also allowed! Here, I is still within the loop and O is still outside the loop:
//
// ..........
// .S------7.
// .|F----7|.
// .||OOOO||.
// .||OOOO||.
// .|L-7F-J|.
// .|II||II|.
// .L--JL--J.
// ..........
//
// In both of the above examples, 4 tiles are enclosed by the loop.
//
// Here's a larger example:
//
// .F----7F7F7F7F-7....
// .|F--7||||||||FJ....
// .||.FJ||||||||L7....
// FJL7L7LJLJ||LJ.L-7..
// L--J.L7...LJS7F-7L7.
// ....F-J..F7FJ|L7L7L7
// ....L7.F7||L7|.L7L7|
// .....|FJLJ|FJ|F7|.LJ
// ....FJL-7.||.||||...
// ....L---J.LJ.LJLJ...
//
// The above sketch has many random bits of ground, some of which are in the loop (I) and some of which are outside it
// (O):
//
// OF----7F7F7F7F-7OOOO
// O|F--7||||||||FJOOOO
// O||OFJ||||||||L7OOOO
// FJL7L7LJLJ||LJIL-7OO
// L--JOL7IIILJS7F-7L7O
// OOOOF-JIIF7FJ|L7L7L7
// OOOOL7IF7||L7|IL7L7|
// OOOOO|FJLJ|FJ|F7|OLJ
// OOOOFJL-7O||O||||OOO
// OOOOL---JOLJOLJLJOOO
//
// In this larger example, 8 tiles are enclosed by the loop.
//
// Any tile that isn't part of the main loop can count as being enclosed by the loop. Here's another example with many
// bits of junk pipe lying around that aren't connected to the main loop at all:
//
// FF7FSF7F7F7F7F7F---7
// L|LJ||||||||||||F--J
// FL-7LJLJ||||||LJL-77
// F--JF--7||LJLJ7F7FJ-
// L---JF-JLJ.||-FJLJJ7
// |F|F-JF---7F7-L7L|7|
// |FFJF7L7F-JF7|JL---7
// 7-L-JL7||F7|L7F-7F7|
// L.L7LFJ|||||FJL7||LJ
// L7JLJL-JLJLJL--JLJ.L
//
// Here are just the tiles that are enclosed by the loop marked with I:
//
// FF7FSF7F7F7F7F7F---7
// L|LJ||||||||||||F--J
// FL-7LJLJ||||||LJL-77
// F--JF--7||LJLJIF7FJ-
// L---JF-JLJIIIIFJLJJ7
// |F|F-JF---7IIIL7L|7|
// |FFJF7L7F-JF7IIL---7
// 7-L-JL7||F7|L7F-7F7|
// L.L7LFJ|||||FJL7||LJ
// L7JLJL-JLJLJL--JLJ.L
//
// In this last example, 10 tiles are enclosed by the loop.
//
// Figure out whether you have time to search for the nest by calculating the area within the loop. How many tiles are
// enclosed by the loop?
//----------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>

using namespace std;


const bool verbose = false;


using Pipe = uint8_t;
const Pipe PipeUp         = 0x01;
const Pipe PipeDown       = 0x02;
const Pipe PipeLeft       = 0x04;
const Pipe PipeRight      = 0x08;
const Pipe PipeDirections = 0x0f;

const Pipe PipeLoop       = 0x10;
const Pipe PipeInside     = 0x20;
const Pipe PipeStart      = 0x80;

const Pipe PipeUpRight    = PipeUp   | PipeRight;
const Pipe PipeVertical   = PipeUp   | PipeDown;
const Pipe PipeUpLeft     = PipeUp   | PipeLeft;
const Pipe PipeDownRight  = PipeDown | PipeRight;
const Pipe PipeHorizontal = PipeLeft | PipeRight;
const Pipe PipeDownLeft   = PipeDown | PipeLeft;
const Pipe PipeMatchingCorners = PipeUp | PipeDown | PipeLeft | PipeRight;


Pipe getPipeFromChar(char c) {
    switch (c) {
        case '|': return PipeVertical;
        case '-': return PipeHorizontal;
        case 'L': return PipeUpRight;
        case 'J': return PipeUpLeft;
        case '7': return PipeDownLeft;
        case 'F': return PipeDownRight;
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

        deduceStartShape();
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
                const Pipe pipe = (*this)(x, y) & ~PipeLoop;
                switch (pipe) {
                    case PipeHorizontal: cout << '-'; break;
                    case PipeVertical:   cout << '|'; break;
                    case PipeUpRight:    cout << 'L'; break;
                    case PipeUpLeft:     cout << 'J'; break;
                    case PipeDownRight:  cout << 'F'; break;
                    case PipeDownLeft:   cout << '7'; break;
                    case PipeStart:      cout << 'S'; break;
                    case PipeInside:     cout << 'i'; break;
                    case 0:              cout << '.'; break;
                    default:             cout << '@'; break;
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
    void deduceStartShape() {
        Pipe pipe {0};
        if (startX > 0 && ((*this)(startX-1, startY) & PipeRight))
            pipe |= PipeLeft;
        if (startX < width-1 && ((*this)(startX+1, startY) & PipeLeft))
            pipe |= PipeRight;
        if (startY > 0 && ((*this)(startX, startY-1) & PipeDown))
            pipe |= PipeUp;
        if (startY < height-1 && ((*this)(startX, startY+1) & PipeUp))
            pipe |= PipeDown;

        (*this)(startX, startY) = pipe;
    }

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
        pipeMap(x,y) |= PipeLoop;
        switch (direction) {
            case PipeUp:    --y;  direction = pipeMap(x,y) & ~PipeDown;   break;
            case PipeDown:  ++y;  direction = pipeMap(x,y) & ~PipeUp;     break;
            case PipeLeft:  --x;  direction = pipeMap(x,y) & ~PipeRight;  break;
            case PipeRight: ++x;  direction = pipeMap(x,y) & ~PipeLeft;   break;
        }
        direction &= ~PipeLoop;
        ++distance;
    }

    int  x;
    int  y;
    int  distance;
    Pipe direction;
};


void markLoop(PipeMap& pipeMap) {
    Runner runner;
    int startX = pipeMap.startX;
    int startY = pipeMap.startY;
    int width  = pipeMap.width;
    int height = pipeMap.height;

    if (startY > 0 && (pipeMap(startX, startY-1) & PipeDown)) {
        runner.Reset(startX, startY, PipeUp);
    } else if (startY < pipeMap.height && (pipeMap(startX, startY+1) & PipeUp)) {
        runner.Reset(startX, startY, PipeDown);
    } else if (startX > 0 && (pipeMap(startX-1, startY) & PipeRight)) {
        runner.Reset(startX, startY, PipeLeft);
    } else if (startX < height && (pipeMap(startX+1, startY) & PipeLeft)) {
        runner.Reset(startX, startY, PipeRight);
    }

    do {
        Pipe &pipe = pipeMap(runner.x, runner.y);
        if (pipe == PipeStart) {
            pipe = 0;
            if (runner.x > 0 && (pipeMap(runner.x-1, runner.y) & PipeRight))
                pipe |= PipeLeft;
            if (runner.x < width && (pipeMap(runner.x+1, runner.y) & PipeLeft))
                pipe |= PipeRight;
            if (runner.y > 0 && (pipeMap(runner.x, runner.y-1) & PipeDown))
                pipe |= PipeUp;
            if (runner.y < height && (pipeMap(runner.x, runner.y+1) & PipeUp))
                pipe |= PipeDown;
        }

        runner.Advance(pipeMap);

    } while (runner.x != startX || runner.y != startY);
}


void removeJunk(PipeMap& pipeMap) {
    for (auto y=0;  y < pipeMap.height;  ++y) {
        for (auto x=0;  x < pipeMap.width;  ++x) {
            const auto pipe = pipeMap(x,y);
            if ((pipe & PipeLoop) == 0)
                pipeMap(x,y) = 0;
        }
    }
}

int markInside(PipeMap& pipeMap) {
    int insideArea = 0;

    for (int y=0;  y < pipeMap.height;  ++y) {

        bool inside = false;
        bool inHorizontalEdge = false;
        Pipe horizontalEdgeCornerStart = 0;

        for (int x=0;  x < pipeMap.width;  ++x) {
            auto pipe = pipeMap(x,y) & PipeDirections;

            if (inHorizontalEdge) {
                if (pipe == PipeHorizontal) continue;

                // If the run is a horizontal jog while continuing vertically, then treat it as a
                // normal vertical edge. If it's from going up-across-down or down-across-up, then
                // just ignore it altogether.

                inHorizontalEdge = false;
                if ((pipe | horizontalEdgeCornerStart) == PipeMatchingCorners)
                    inside = !inside;

            } else if (pipe == PipeVertical) {
                inside = !inside;

            } else if (pipe == 0) {
                if (inside) {
                    ++insideArea;
                    pipeMap(x,y) = PipeInside;
                }

            } else if (pipe == PipeUpRight || pipe == PipeDownRight) {
                inHorizontalEdge = true;
                horizontalEdgeCornerStart = pipe;
            }
        }
    }

    return insideArea;
}


int main() {
    vector<string> mapText;
    string line;

    while (getline(cin, line))
        mapText.push_back(line);

    PipeMap pipeMap(mapText);
    if (verbose) {
        cout << "\nOn Load:\n";
        pipeMap.dump();
    }

    if (verbose) {
        cout << "\nMarking Loop:";
    }
    markLoop(pipeMap);
    if (verbose) {
        cout << "\nLoop Marked:\n";
        pipeMap.dump();
    }

    removeJunk(pipeMap);
    if (verbose) {
        cout << "\nJunk Removed:\n";
        pipeMap.dump();
    }

    int insideArea = markInside(pipeMap);
    if (verbose) {
        cout << "\nInside Marked:\n";
        pipeMap.dump();
        cout << '\n';
    }

    cout << "Inside area: " << insideArea << '\n';

    return 0;
}
