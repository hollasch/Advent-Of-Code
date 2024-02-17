//----------------------------------------------------------------------------------------------------------------------
// 2023 Advent of Code -- Puzzle 12a
//
// --- Day 12: Hot Springs ---
// You finally reach the hot springs! You can see steam rising from secluded areas attached to the primary, ornate
// building.
//
// As you turn to enter, the researcher stops you. "Wait - I thought you were looking for the hot springs, weren't you?"
// You indicate that this definitely looks like hot springs to you.
//
// "Oh, sorry, common mistake! This is actually the onsen! The hot springs are next door."
//
// You look in the direction the researcher is pointing and suddenly notice the massive metal helixes towering overhead.
// "This way!"
//
// It only takes you a few more steps to reach the main gate of the massive fenced-off area containing the springs. You
// go through the gate and into a small administrative building.
//
// "Hello! What brings you to the hot springs today? Sorry they're not very hot right now; we're having a lava shortage
// at the moment." You ask about the missing machine parts for Desert Island.
//
// "Oh, all of Gear Island is currently offline! Nothing is being manufactured at the moment, not until we get more lava
// to heat our forges. And our springs. The springs aren't very springy unless they're hot!"
//
// "Say, could you go up and see why the lava stopped flowing? The springs are too cold for normal operation, but we
// should be able to find one springy enough to launch you up there!"
//
// There's just one problem - many of the springs have fallen into disrepair, so they're not actually sure which springs
// would even be safe to use! Worse yet, their condition records of which springs are damaged (your puzzle input) are
// also damaged! You'll need to help them repair the damaged records.
//
// In the giant field just outside, the springs are arranged into rows. For each row, the condition records show every
// spring and whether it is operational (.) or damaged (#). This is the part of the condition records that is itself
// damaged; for some springs, it is simply unknown (?) whether the spring is operational or damaged.
//
// However, the engineer that produced the condition records also duplicated some of this information in a different
// format! After the list of springs for a given row, the size of each contiguous group of damaged springs is listed in
// the order those groups appear in the row. This list always accounts for every damaged spring, and each number is the
// entire size of its contiguous group (that is, groups are always separated by at least one operational spring: ####
// would always be 4, never 2,2).
//
// So, condition records with no unknown spring conditions might look like this:
//
//     #.#.### 1,1,3
//     .#...#....###. 1,1,3
//     .#.###.#.###### 1,3,1,6
//     ####.#...#... 4,1,1
//     #....######..#####. 1,6,5
//     .###.##....# 3,2,1
//
// However, the condition records are partially damaged; some of the springs' conditions are actually unknown (?). For
// example:
//
//     ???.### 1,1,3
//     .??..??...?##. 1,1,3
//     ?#?#?#?#?#?#?#? 1,3,1,6
//     ????.#...#... 4,1,1
//     ????.######..#####. 1,6,5
//     ?###???????? 3,2,1
//
// Equipped with this information, it is your job to figure out how many different arrangements of operational and
// broken springs fit the given criteria in each row.
//
// In the first line (???.### 1,1,3), there is exactly one way separate groups of one, one, and three broken springs (in
// that order) can appear in that row: the first three unknown springs must be broken, then operational, then broken
// (#.#), making the whole row #.#.###.
//
// The second line is more interesting: .??..??...?##. 1,1,3 could be a total of four different arrangements. The last ?
// must always be broken (to satisfy the final contiguous group of three broken springs), and each ?? must hide exactly
// one of the two broken springs. (Neither ?? could be both broken springs or they would form a single contiguous group
// of two; if that were true, the numbers afterward would have been 2,3 instead.) Since each ?? can either be #. or .#,
// there are four possible arrangements of springs.
//
// The last line is actually consistent with ten different arrangements! Because the first number is 3, the first and
// second ? must both be . (if either were #, the first number would have to be 4 or higher). However, the remaining run
// of unknown spring conditions have many different ways they could hold groups of two and one broken springs:
//
//     ?###???????? 3,2,1
//     .###.##.#...
//     .###.##..#..
//     .###.##...#.
//     .###.##....#
//     .###..##.#..
//     .###..##..#.
//     .###..##...#
//     .###...##.#.
//     .###...##..#
//     .###....##.#
//
// In this example, the number of possible arrangements for each row is:
//
//   - ???.### 1,1,3 - 1 arrangement
//   - .??..??...?##. 1,1,3 - 4 arrangements
//   - ?#?#?#?#?#?#?#? 1,3,1,6 - 1 arrangement
//   - ????.#...#... 4,1,1 - 1 arrangement
//   - ????.######..#####. 1,6,5 - 4 arrangements
//   - ?###???????? 3,2,1 - 10 arrangements
//
// Adding all of the possible arrangement counts together produces a total of 21 arrangements.
//
// For each row, count all of the different arrangements of operational and broken springs that meet the given criteria.
// What is the sum of those counts?
//----------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const bool debug = true;

class SpringGroup {
  public:
    int length;
    int rangeStart {0};
    int rangeEnd {0};

    SpringGroup(int len) : length(len) {}
};


class SpringSet {
  public:
    SpringSet(string text) {
        //if (debug) cout << "\n<" << text << ">\n";
        stringstream iss(text);
        iss >> springsText;
        string numbers;
        iss >> numbers;
        while (numbers.size() > 0) {
            int n = stoi(numbers);
            groups.push_back({n});
            auto sepPos = numbers.find(',');
            if (sepPos == string::npos)
                break;
            numbers.erase(numbers.begin(), numbers.begin() + sepPos + 1);
        }

        int numGroups = groups.size();

        for (int i=0;  i < numGroups;  ++i) {
            if (i == 0)
                groups[i].rangeStart = 0;
            else {
                auto& pred = groups[i-1];
                groups[i].rangeStart = pred.length + pred.rangeStart + 1;
            }
        }

        for (int i = numGroups - 1;  i >= 0;  --i) {
            auto currLength = groups[i].length;
            if (i == (numGroups - 1))
                groups[i].rangeEnd = springsText.size() - currLength;
            else {
                auto& succ = groups[i+1];
                groups[i].rangeEnd = succ.rangeEnd - currLength - 1;
            }
        }
    }

    string springsText;
    vector<SpringGroup> groups;
};


bool isGood(const string& springsText, int pos) {
    if (pos < 0 || pos >= springsText.size()) return true;
    auto c = springsText[pos];
    return (c == '?' || c == '.');
}

bool isBad(const string& springsText, int pos) {
    if (pos < 0 || pos >= springsText.size()) return false;
    auto c = springsText[pos];
    return (c == '?' || c == '#');
}

bool matchRun(const string& springsText, int pos, int length) {
    if (pos > 0 && !isGood(springsText, pos-1))
        return false;
    if (pos < springsText.size() - length && !isGood(springsText, pos+length))
        return false;
    for (int i=pos;  i < pos+length;  ++i) {
        if (!isBad(springsText, i))
            return false;
    }
    return true;
}

bool remainderGood(const string& springsText, int pos) {
    for (int i=pos;  i < springsText.size();  ++i) {
        if (!isGood(springsText, i))
            return false;
    }
    return true;
}

uint64_t countArrangements(const SpringSet& springSet, int groupIndex, int startPos) {
    uint64_t sumArrangements = 0;
    auto& springGroup = springSet.groups[groupIndex];
    auto groupLength = springGroup.length;
    for (auto pos = startPos;  pos <= springGroup.rangeEnd;  ++pos) {
        if (matchRun(springSet.springsText, pos, groupLength)) {
            if (groupIndex == springSet.groups.size() - 1) {
                if (remainderGood(springSet.springsText, pos + groupLength + 1))
                    sumArrangements += 1;
            } else {
                sumArrangements += countArrangements(springSet, groupIndex + 1, pos + groupLength + 1);
            }
        }
    }
    return sumArrangements;
}

int main() {
    vector<SpringSet> springSets;
    vector<string> lines;
    string line;
    while (getline(cin, line))
        springSets.push_back({line});

    if (debug) {
        for (auto& ss : springSets) {
            cout << '(' << ss.springsText << ")\n";
            for (auto& sg : ss.groups)
                cout << "  (" << sg.length << ") " << sg.rangeStart << '-' << sg.rangeEnd << '\n';
            cout << "  Num Arrangements: " << countArrangements(ss, 0, 0) << '\n';
            cout << '\n';
        }
    }

    uint64_t totalArrangements = 0;
    for (auto& springSet : springSets) {
        auto numArrangements = countArrangements(springSet, 0, 0);
        if (debug) {
            cout << "Number Arrangements: " << numArrangements << " (" << springSet.springsText << ")\n";
        }
        totalArrangements += numArrangements;
    }

    cout << "Total arrangements: " << totalArrangements << '\n';

    return 0;
}
