//----------------------------------------------------------------------------------------------------------------------
// 2023 Advent of Code -- Puzzle 03a
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
//----------------------------------------------------------------------------------------------------------------------

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


int numberLength(const string& line, int column, int width) {
    int length = 0;
    while (column + length < width && isdigit(line[column + length]))
        ++length;
    return length;
}


bool isSymbol(char c) {
    return c != '.' && !isdigit(c);
}


bool symbolAdjacentLeft(const vector<string>& lines, int lineIndex, int begin, int lineCount, int width) {
    if (begin == 0)
        return false;

    int column = begin - 1;

    if (lineIndex > 0 && isSymbol(lines[lineIndex - 1][column]))
        return true;

    if (lineIndex + 1 < lineCount && isSymbol(lines[lineIndex + 1][column]))
        return true;

    return isSymbol(lines[lineIndex][column]);
}


bool symbolAdjacentRight(const vector<string>& lines, int lineIndex, int end, int lineCount, int width) {
    int column = end + 1;

    if (column >= width)
        return false;

    if (lineIndex > 0 && isSymbol(lines[lineIndex - 1][column]))
        return true;

    if (lineIndex + 1 < lineCount && isSymbol(lines[lineIndex + 1][column]))
        return true;

    return isSymbol(lines[lineIndex][column]);
}


bool symbolAdjacentTop(const vector<string>& lines, int lineIndex, int begin, int end, int lineCount, int width) {
    --lineIndex;
    if (lineIndex < 0)
        return false;

    for (int i = begin - 1;  i <= end + 1;  ++i) {
        if (i < 0 || i >= width)
            continue;
        
        if (isSymbol(lines[lineIndex][i]))
            return true;
    }

    return false;
}


bool symbolAdjacentBottom(const vector<string>& lines, int lineIndex, int begin, int end, int lineCount, int width) {
    ++lineIndex;
    if (lineIndex >= lineCount)
        return false;

    for (int i = begin - 1;  i <= end + 1;  ++i) {
        if (i < 0 || i >= width)
            continue;
        
        if (isSymbol(lines[lineIndex][i]))
            return true;
    }

    return false;
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

    int lineCount = lines.size();
    for (int lineIndex = 0;  lineIndex < lineCount;  ++lineIndex) {
        // cout << "\n> " << lines[lineIndex] << '\n';
        int column = 0;
        while (column < width) {
            while (column < width && !isdigit(lines[lineIndex][column]))
                ++column;
            if (column >= width)
                break;
            int partNumber = atoi(lines[lineIndex].c_str() + column);

            int partNumberLength = numberLength(lines[lineIndex], column, width);
            int partNumberEnd = column + partNumberLength - 1;

            if (   symbolAdjacentLeft(lines, lineIndex, column, lineCount, width)
                || symbolAdjacentRight(lines, lineIndex, partNumberEnd, lineCount, width)
                || symbolAdjacentTop(lines, lineIndex, column, partNumberEnd, lineCount, width)
                || symbolAdjacentBottom(lines, lineIndex, column, partNumberEnd, lineCount, width)
            ) {
                sum += partNumber;
                // cout << "! " << partNumber << '\n';
            }

            column = partNumberEnd + 1;
        }
        // cout << '\n';
    }

    cout << '\n' << sum << '\n';
    return 0;
}
