//----------------------------------------------------------------------------------------------------------------------
// 2023 Advent of Code -- Puzzle 08a
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
//----------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std;

const bool verbose = false;


class Node {
  public:
    string name;
    Node* left;
    Node* right;

    Node() : name(""), left(nullptr), right(nullptr) {}
    Node(const string_view& name) : name(name), left(this), right(this) {}
};


class Navigator {
  public:
    string route;
    string::iterator currentStep;

    Navigator(const string_view& routeString)
      : route(routeString)
    {
    }

    int walk(const Node& start, const Node& end) {
        currentStep = route.begin();
        const Node* startNode = &start;
        const Node* endNode = &end;
        const Node* currentNode = startNode;
        int routeLength = 0;

        while (currentNode != endNode) {
            ++routeLength;
            if (verbose)
                cout << "Step " << routeLength << ": Walking from " << currentNode->name;
            if (*currentStep == 'L') {
                currentNode = currentNode->left;
                if (verbose)
                    cout << " left";
            } else if (*currentStep == 'R') {
                currentNode = currentNode->right;
                if (verbose)
                    cout << " right";
            } else {
                throw runtime_error("Invalid route string");
            }

            if (verbose)
                cout << " to " << currentNode->name << '\n';

            ++currentStep;
            if (currentStep == route.end())
                currentStep = route.begin();
        }

        return routeLength;
    }
};


int main() {
    string line;

    getline(cin, line);
    Navigator navigator(line);

    if (verbose)
        cout << "Route string is " << navigator.route << '\n';

    unordered_map<string, Node> nodes;

    nodes.insert({"AAA", Node("AAA")});
    nodes.insert({"ZZZ", Node("ZZZ")});

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

        if (verbose)
            cout << "Node " << nodeName << " has left node " << leftNodeName << " and right node " << rightNodeName << '\n';

        if (!nodes.contains(nodeName))
            nodes.insert({nodeName, Node(nodeName)});

        Node& node = nodes[nodeName];

        nodes.insert({leftNodeName,  Node(leftNodeName)});
        nodes.insert({rightNodeName, Node(rightNodeName)});

        node.left  = &(nodes[leftNodeName]);
        node.right = &(nodes[rightNodeName]);
    }

    if (verbose)
        cout << '\n';

    auto routeLength = navigator.walk(nodes["AAA"], nodes["ZZZ"]);
    cout << "\nArrived at node ZZZ with in " << routeLength << " steps.\n";

    return 0;
}
