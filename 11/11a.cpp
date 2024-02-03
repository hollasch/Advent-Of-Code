//----------------------------------------------------------------------------------------------------------------------
// 2023 Advent of Code -- Puzzle 11a
//
// --- Day 11: Cosmic Expansion ---
//
// You continue following signs for "Hot Springs" and eventually come across an observatory. The Elf within turns out to
// be a researcher studying cosmic expansion using the giant telescope here.
//
// He doesn't know anything about the missing machine parts; he's only visiting for this research project. However, he
// confirms that the hot springs are the next-closest area likely to have people; he'll even take you straight there
// once he's done with today's observation analysis.
//
// Maybe you can help him with the analysis to speed things up?
//
// The researcher has collected a bunch of data and compiled the data into a single giant image (your puzzle input). The
// image includes empty space (.) and galaxies (#). For example:
//
//     ...#......
//     .......#..
//     #.........
//     ..........
//     ......#...
//     .#........
//     .........#
//     ..........
//     .......#..
//     #...#.....
//
// The researcher is trying to figure out the sum of the lengths of the shortest path between every pair of galaxies.
// However, there's a catch: the universe expanded in the time it took the light from those galaxies to reach the
// observatory.
//
// Due to something involving gravitational effects, only some space expands. In fact, the result is that any rows or
// columns that contain no galaxies should all actually be twice as big.
//
// In the above example, three columns and two rows contain no galaxies:
//
//        v  v  v
//      ...#......
//      .......#..
//      #.........
//     >..........<
//      ......#...
//      .#........
//      .........#
//     >..........<
//      .......#..
//      #...#.....
//        ^  ^  ^
// These rows and columns need to be twice as big; the result of cosmic expansion therefore looks like this:
//
//     ....#........
//     .........#...
//     #............
//     .............
//     .............
//     ........#....
//     .#...........
//     ............#
//     .............
//     .............
//     .........#...
//     #....#.......
//
// Equipped with this expanded universe, the shortest path between every pair of galaxies can be found. It can help to
// assign every galaxy a unique number:
//
//     ....1........
//     .........2...
//     3............
//     .............
//     .............
//     ........4....
//     .5...........
//     ............6
//     .............
//     .............
//     .........7...
//     8....9.......
//
// In these 9 galaxies, there are 36 pairs. Only count each pair once; order within the pair doesn't matter. For each
// pair, find any shortest path between the two galaxies using only steps that move up, down, left, or right exactly one
// . or # at a time. (The shortest path between two galaxies is allowed to pass through another galaxy.)
//
// For example, here is one of the shortest paths between galaxies 5 and 9:
//
//     ....1........
//     .........2...
//     3............
//     .............
//     .............
//     ........4....
//     .5...........
//     .##.........6
//     ..##.........
//     ...##........
//     ....##...7...
//     8....9.......
//
// This path has length 9 because it takes a minimum of nine steps to get from galaxy 5 to galaxy 9 (the eight locations
// marked # plus the step onto galaxy 9 itself). Here are some other example shortest path lengths:
//
//   - Between galaxy 1 and galaxy 7: 15
//   - Between galaxy 3 and galaxy 6: 17
//   - Between galaxy 8 and galaxy 9: 5
//
// In this example, after expanding the universe, the sum of the shortest path between all 36 pairs of galaxies is 374.
//
// Expand the universe, then find the length of the shortest path between every pair of galaxies. What is the sum of
// these lengths?
//
//----------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool verbose = false;

class Galaxy {
  public:
    Galaxy(int sn, int x, int y) : sn(sn), x(x), y(y) {}

    int sn;
    int x;
    int y;
};


int main() {
    
    // Read in all lines from input.

    string line;
    vector<string> lines;
    int width = 0;
    while (getline(cin, line)) {

        if (width == 0)
            width = line.size();
        else {
            if (line.size() != width) {
                cerr << "Error: line width mismatch\n";
                return 1;
            }
        }

        lines.push_back(line);
    }
    int height = lines.size();

    // Initialize the horizontal sizes array.

    vector<int> sizesX;
    sizesX.resize(width);
    for (int i = 0;  i < width;  ++i)
        sizesX[i] = 2;

    // Initialize the vertical sizes array.

    vector<int>  sizesY;
    sizesY.resize(height);

    // Grab all galaxies from the field into a list. At the same time, set the elements of the
    // sizesX and sizesY arrays.

    int galaxySN = 0;
    vector<Galaxy> galaxies;
    for (int j = 0;  j < height;  ++j) {
        auto& line = lines[j];
        if (verbose) cout << '(' << line << ")\n";

        size_t i = 0;
        bool galaxyInLine = false;
        while (string::npos != (i = line.find('#', i))) {
            galaxies.push_back(Galaxy(galaxySN++, i, j));
            sizesX[i] = 1;
            galaxyInLine = true;
            ++i;
        }

        sizesY[j] = galaxyInLine ? 1 : 2;
    }

    // Using the sizesX array, calculate the distancesX array.

    if (verbose) cout << "\nSizesX: ";
    vector<int> distancesX;
    distancesX.resize(width);
    for (int i = 0;  i < width;  ++i) {
        if (verbose) cout << sizesX[i];

        if (i == 0)
            distancesX[i] = 0;
        else
            distancesX[i] = distancesX[i-1] + sizesX[i-1];

        if (verbose) cout << '(' << distancesX[i] << "), ";
    }

    // Using the sizesY array, calculate the distancesY array.

    if (verbose) cout << "\nDeltaY: ";
    vector<int> distancesY;
    distancesY.resize(height);
    for (int i = 0;  i < height;  ++i) {
        if (verbose) cout << sizesY[i];

        if (i == 0)
            distancesY[i] = 0;
        else
            distancesY[i] = distancesY[i-1] + sizesY[i-1];

        if (verbose) cout << '(' << distancesY[i] << "), ";
    }
    if (verbose) cout << "\n\n";

    // Calculate the sum of the distances between all pairs of galaxies.

    long sumDistances = 0;

    for (int i = 0;  i < galaxies.size();  ++i) {
        auto& galaxy = galaxies[i];
        if (verbose) cout << "Galaxy " << (galaxy.sn+1) << " at (" << galaxy.x << ", " << galaxy.y << ")\n";
        for (int j = i+1;  j < galaxies.size();  ++j) {
            auto& otherGalaxy = galaxies[j];
            int dx = abs(distancesX[galaxy.x] - distancesX[otherGalaxy.x]);
            int dy = abs(distancesY[galaxy.y] - distancesY[otherGalaxy.y]);
            int distance = dx + dy;
            sumDistances += distance;
            if (verbose) cout << "  Distance to " << (otherGalaxy.sn+1) << " is " << distance << '\n';
        }
    }

    // Report the result.

    cout << "Total distances: " << sumDistances << '\n';

    return 0;
}
