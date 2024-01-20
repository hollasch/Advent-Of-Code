//----------------------------------------------------------------------------------------------------------------------
// 2023 Advent of Code -- Puzzle 03b (See Part Two below)
//
// --- Day 3: Gear Ratios ---

// You and the Elf eventually reach a gondola lift station; he says the gondola lift will take you
// up to the water source, but this is as far as he can bring you. You go inside.
//
// It doesn't take long to find the gondolas, but there seems to be a problem: they're not moving.
//
// "Aaah!"
//
// You turn around to see a slightly-greasy Elf with a wrench and a look of surprise. "Sorry, I
// wasn't expecting anyone! The gondola lift isn't working right now; it'll still be a while before
// I can fix it." You offer to help.
//
// The engineer explains that an engine part seems to be missing from the engine, but nobody can
// figure out which one. If you can add up all the part numbers in the engine schematic, it should
// be easy to work out which part is missing.
//
// The engine schematic (your puzzle input) consists of a visual representation of the engine. There
// are lots of numbers and symbols you don't really understand, but apparently any number adjacent
// to a symbol, even diagonally, is a "part number" and should be included in your sum. (Periods (.)
// do not count as a symbol.)
//
// Here is an example engine schematic:
//
//     467..114..
//     ...*......
//     ..35..633.
//     ......#...
//     617*......
//     .....+.58.
//     ..592.....
//     ......755.
//     ...$.*....
//     .664.598..
//
// In this schematic, two numbers are not part numbers because they are not adjacent to a symbol:
// 114 (top right) and 58 (middle right). Every other number is adjacent to a symbol and so is a
// part number; their sum is 4361.
//
// Of course, the actual engine schematic is much larger. What is the sum of all of the part numbers
// in the engine schematic?
//
// --- Part Two ---
//
// The engineer finds the missing part and installs it in the engine! As the engine springs to life,
// you jump in the closest gondola, finally ready to ascend to the water source.
//
// You don't seem to be going very fast, though. Maybe something is still wrong? Fortunately, the
// gondola has a phone labeled "help", so you pick it up and the engineer answers.
//
// Before you can explain the situation, she suggests that you look out the window. There stands the
// engineer, holding a phone in one hand and waving with the other. You're going so slowly that you
// haven't even left the station. You exit the gondola.
//
// The missing part wasn't the only issue - one of the gears in the engine is wrong. A gear is any *
// symbol that is adjacent to exactly two part numbers. Its gear ratio is the result of multiplying
// those two numbers together.
//
// This time, you need to find the gear ratio of every gear and add them all up so that the engineer
// can figure out which gear needs to be replaced.
//
// Consider the same engine schematic again:
//
//     467..114..
//     ...*......
//     ..35..633.
//     ......#...
//     617*......
//     .....+.58.
//     ..592.....
//     ......755.
//     ...$.*....
//     .664.598..
//
// In this schematic, there are two gears. The first is in the top left; it has part numbers 467 and
// 35, so its gear ratio is 16345. The second gear is in the lower right; its gear ratio is 451490.
// (The * adjacent to 617 is not a gear because it is only adjacent to one part number.) Adding up
// all of the gear ratios produces 467835.
//
// What is the sum of all of the gear ratios in your engine schematic?
//
//----------------------------------------------------------------------------------------------------------------------

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


int getNumberAt(const char *s, int offset) {
    while (offset > 0 && isdigit(s[offset-1]))
        --offset;
    return atoi(s + offset);
}


int getGearRatio(const vector<string>& lines, int width, int lineIndex, int columnIndex) {
    int countAdjacent = 0;
    int gearRatio = 1;

    bool roomTop = lineIndex > 0;
    bool roomBottom = lineIndex < (lines.size() - 1);

    bool roomLeft = columnIndex > 0;
    bool roomRight = columnIndex < (width - 1);

    if (roomTop) {
        const string& lineAbove = lines[lineIndex-1];
        if (isdigit(lineAbove[columnIndex])) {
            gearRatio *= getNumberAt(lineAbove.c_str(), columnIndex);
            ++countAdjacent;
        } else {
            if (roomLeft && isdigit(lineAbove[columnIndex-1])) {
                gearRatio *= getNumberAt(lineAbove.c_str(), columnIndex-1);
                ++countAdjacent;
            }
            if (roomRight && isdigit(lineAbove[columnIndex+1])) {
                gearRatio *= getNumberAt(lineAbove.c_str(), columnIndex+1);
                ++countAdjacent;
            }
        }
    }

    if (countAdjacent > 2)
        return -1;

    if (roomLeft && isdigit(lines[lineIndex][columnIndex-1])) {
        gearRatio *= getNumberAt(lines[lineIndex].c_str(), columnIndex-1);
        ++countAdjacent;
    }

    if (roomRight && isdigit(lines[lineIndex][columnIndex+1])) {
        gearRatio *= getNumberAt(lines[lineIndex].c_str(), columnIndex+1);
        ++countAdjacent;
    }

    if (countAdjacent > 2)
        return -1;

    if (roomBottom) {
        const string& lineBelow = lines[lineIndex+1];
        if (isdigit(lineBelow[columnIndex])) {
            gearRatio *= getNumberAt(lineBelow.c_str(), columnIndex);
            ++countAdjacent;
        } else {
            if (roomLeft && isdigit(lineBelow[columnIndex-1])) {
                gearRatio *= getNumberAt(lineBelow.c_str(), columnIndex-1);
                ++countAdjacent;
            }
            if (roomRight && isdigit(lineBelow[columnIndex+1])) {
                gearRatio *= getNumberAt(lineBelow.c_str(), columnIndex+1);
                ++countAdjacent;
            }
        }
    }

    return (countAdjacent == 2) ? gearRatio : -1;
}


int main() {
    vector<string> lines;
    string line;
    int lineNumber = 0;
    int sum = 0;

    int width = -1;
    while (getline(cin, line)) {
        ++lineNumber;
        if (width == -1) {
            width = line.length();
        } else if (width != line.length()) {
            cout << "Error: line length mismatch at line " << lineNumber << ".\n";
            return 1;
        }
        lines.push_back(line);
    }

    // Scan the field, top to bottom.

    int lineCount = lines.size();
    for (int lineIndex = 0;  lineIndex < lineCount;  ++lineIndex) {

        // Convert all extraneous symbols (neither '*' nor digit) to dots.
        for (auto& c : lines[lineIndex]) {
            if (c != '*' && !isdigit(c))
                c = '.';
        }

        for (int column = 0;  column < width;  ++column) {

            if (lines[lineIndex][column] != '*')
                continue;

            // cout << "Asterisk at " << (lineIndex + 1) << ", " << (column + 1) << ". ";

            int gearRatio = getGearRatio(lines, width, lineIndex, column);

            // cout << "Gear ratio " << gearRatio << ".\n";

            if (gearRatio != -1)
                sum += gearRatio;
        }
    }

    cout << '\n' << sum << '\n';
    return 0;
}
