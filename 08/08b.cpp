//----------------------------------------------------------------------------------------------------------------------
// 2023 Advent of Code -- Puzzle 08b (See Part Two below)
//
// --- Day 8: Haunted Wasteland ---
//
// You're still riding a camel across Desert Island when you spot a sandstorm quickly approaching. When you turn to warn
// the Elf, she disappears before your eyes! To be fair, she had just finished warning you about ghosts a few minutes
// ago.
//
// One of the camel's pouches is labeled "maps" - sure enough, it's full of documents (your puzzle input) about how to
// navigate the desert. At least, you're pretty sure that's what they are; one of the documents contains a list of
// left/right instructions, and the rest of the documents seem to describe some kind of network of labeled nodes.
//
// It seems like you're meant to use the left/right instructions to navigate the network. Perhaps if you have the camel
// follow the same instructions, you can escape the haunted wasteland!
//
// After examining the maps for a bit, two nodes stick out: AAA and ZZZ. You feel like AAA is where you are now, and you
// have to follow the left/right instructions until you reach ZZZ.
//
// This format defines each node of the network individually. For example:
//
//     RL
//
//     AAA = (BBB, CCC)
//     BBB = (DDD, EEE)
//     CCC = (ZZZ, GGG)
//     DDD = (DDD, DDD)
//     EEE = (EEE, EEE)
//     GGG = (GGG, GGG)
//     ZZZ = (ZZZ, ZZZ)
//
// Starting with AAA, you need to look up the next element based on the next left/right instruction in your input. In
// this example, start with AAA and go right (R) by choosing the right element of AAA, CCC. Then, L means to choose the
// left element of CCC, ZZZ. By following the left/right instructions, you reach ZZZ in 2 steps.
//
// Of course, you might not find ZZZ right away. If you run out of left/right instructions, repeat the whole sequence of
// instructions as necessary: RL really means RLRLRLRLRLRLRLRL... and so on. For example, here is a situation that takes
// 6 steps to reach ZZZ:
//
//     LLR
//
//     AAA = (BBB, BBB)
//     BBB = (AAA, ZZZ)
//     ZZZ = (ZZZ, ZZZ)
//
// Starting at AAA, follow the left/right instructions. How many steps are required to reach ZZZ?
//
// --- Part Two ---
//
// The sandstorm is upon you and you aren't any closer to escaping the wasteland. You had the camel follow the
// instructions, but you've barely left your starting position. It's going to take significantly more steps to escape!
//
// What if the map isn't for people - what if the map is for ghosts? Are ghosts even bound by the laws of spacetime?
// Only one way to find out.
//
// After examining the maps a bit longer, your attention is drawn to a curious fact: the number of nodes with names
// ending in A is equal to the number ending in Z! If you were a ghost, you'd probably just start at every node that
// ends with A and follow all of the paths at the same time until they all simultaneously end up at nodes that end with
// Z.
//
// For example:
//
//     LR
//
//     11A = (11B, XXX)
//     11B = (XXX, 11Z)
//     11Z = (11B, XXX)
//     22A = (22B, XXX)
//     22B = (22C, 22C)
//     22C = (22Z, 22Z)
//     22Z = (22B, 22B)
//     XXX = (XXX, XXX)
//
// Here, there are two starting nodes, 11A and 22A (because they both end with A). As you follow each left/right
// instruction, use that instruction to simultaneously navigate away from both nodes you're currently on. Repeat this
// process until all of the nodes you're currently on end with Z. (If only some of the nodes you're on end with Z, they
// act like any other node and you continue as normal.) In this example, you would proceed as follows:
//
//   - Step 0: You are at 11A and 22A.
//   - Step 1: You choose all of the left paths, leading you to 11B and 22B.
//   - Step 2: You choose all of the right paths, leading you to 11Z and 22C.
//   - Step 3: You choose all of the left paths, leading you to 11B and 22Z.
//   - Step 4: You choose all of the right paths, leading you to 11Z and 22B.
//   - Step 5: You choose all of the left paths, leading you to 11B and 22C.
//   - Step 6: You choose all of the right paths, leading you to 11Z and 22Z.
//
// So, in this example, you end up entirely on nodes that end in Z after 6 steps.
//
// Simultaneously start on every node that ends with A. How many steps does it take before you're only on nodes that end
// with Z?
//----------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std;

const bool verbose = false;
const bool showProgress = true;


class Node {
  public:
    string name;
    Node* path[2];
    bool  isEnd;

    Node() : name{""}, path{nullptr, nullptr}, isEnd{false} {}
    Node(const string_view& name) : name(name), path{nullptr, nullptr} {
        isEnd = (name.back() == 'Z');
    }
};


class Navigator {
  public:
    vector<int> directions;

    Navigator(const string_view& routeString)
    {
        for (auto c : routeString)
            directions.push_back((c == 'R') ? 1 : 0);
    }

    uint64_t walk(unordered_map<string, Node>& nodes) {

        vector<const Node*> runners;

        if (verbose)
            cout << "Runners:";

        for (auto & node : nodes) {
            if (node.second.name.back() == 'A') {
                runners.push_back(&node.second);
                if (verbose)
                    cout << ' ' << node.second.name;
            }
        }

        if (verbose)
            cout << '\n';

        uint64_t routeLength = 0;
        uint64_t progressCounter = 0;
        auto direction = directions.begin();

        while (true) {

            ++routeLength;

            // if (verbose)
            //     cout << "\nStep " << routeLength << ":\n";

            if (++progressCounter >= 1'000'000'000UL) {
                progressCounter = 0;
                cout << "\rStep " << routeLength << ' ' << flush;
            }

            // Advance each runner.

            auto runner = runners.begin();
            for (;  runner != runners.end();  ++runner) {
                auto* runnerNext = (*runner)->path[*direction];

                // if (verbose) {
                //     cout << "  " << (*runner)->name
                //          << " --" << (*direction == 0 ? "left" : "right") << "--> "
                //          << runnerNext->name << '\n';
                // }

                (*runner) = runnerNext;

                if (!runnerNext->isEnd)
                    break;
            }

            if (runner == runners.end())
                break;

            for (++runner;  runner < runners.end();  ++runner) {
                const auto* runnerNext = (*runner)->path[*direction];

                // if (verbose) {
                //     cout << "  " << (*runner)->name
                //          << " --" << (*direction == 0 ? "left" : "right") << "--> "
                //          << runnerNext->name << '\n';
                // }

                (*runner) = runnerNext;
            }

            if (++direction == directions.end())
                direction = directions.begin();
        }

        cout << '\n';

        return routeLength;
    }
};


int main() {
    string line;

    getline(cin, line);
    Navigator navigator(line);

    if (verbose)
        cout << "\nRoute string is " << line << '\n';

    unordered_map<string, Node> nodes;

    while (getline(cin, line)) {

        if (line.empty())
            continue;

        istringstream iss(line);

        string token;
        string nodeName;
        string leftNodeName;
        string rightNodeName;

        iss >> nodeName;
        iss >> token;    // Equal sign

        iss >> token;    // Looks like "(ABCDE,"
        leftNodeName = token.substr(1, token.size() - 2);

        iss >> token;    // Looks like "ABCDE)"
        rightNodeName = token.substr(0, token.size() - 1);

        if (verbose) {
            cout << "Node " << nodeName
                 << " has left node " << leftNodeName
                 << " and right node " << rightNodeName << '\n';
        }

        if (!nodes.contains(nodeName))
            nodes.insert({nodeName, Node(nodeName)});

        Node& node = nodes[nodeName];

        nodes.insert({leftNodeName,  Node(leftNodeName)});
        nodes.insert({rightNodeName, Node(rightNodeName)});

        node.path[0] = &(nodes[leftNodeName]);
        node.path[1] = &(nodes[rightNodeName]);
    }

    if (verbose)
        cout << '\n';

    auto routeLength = navigator.walk(nodes);
    cout << "\nRoutes completed in " << routeLength << " steps.\n";

    return 0;
}
