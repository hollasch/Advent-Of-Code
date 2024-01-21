//----------------------------------------------------------------------------------------------------------------------
// 2023 Advent of Code -- Puzzle 07b (See Part Two below)
//
// --- Day 7: Camel Cards ---
//
// Your all-expenses-paid trip turns out to be a one-way, five-minute ride in an airship. (At least it's a cool
// airship!) It drops you off at the edge of a vast desert and descends back to Island Island.
//
// "Did you bring the parts?"
//
// You turn around to see an Elf completely covered in white clothing, wearing goggles, and riding a large camel.
//
// "Did you bring the parts?" she asks again, louder this time. You aren't sure what parts she's looking for; you're
// here to figure out why the sand stopped.
//
// "The parts! For the sand, yes! Come with me; I will show you." She beckons you onto the camel.
//
// After riding a bit across the sands of Desert Island, you can see what look like very large rocks covering half of
// the horizon. The Elf explains that the rocks are all along the part of Desert Island that is directly above Island
// Island, making it hard to even get there. Normally, they use big machines to move the rocks and filter the sand, but
// the machines have broken down because Desert Island recently stopped receiving the parts they need to fix the
// machines.
//
// You've already assumed it'll be your job to figure out why the parts stopped when she asks if you can help. You agree
// automatically.
//
// Because the journey will take a few days, she offers to teach you the game of Camel Cards. Camel Cards is sort of
// similar to poker except it's designed to be easier to play while riding a camel.
//
// In Camel Cards, you get a list of hands, and your goal is to order them based on the strength of each hand. A hand
// consists of five cards labeled one of A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, or 2. The relative strength of each card
// follows this order, where A is the highest and 2 is the lowest.
//
// Every hand is exactly one type. From strongest to weakest, they are:
//
//   - Five of a kind, where all five cards have the same label: AAAAA
//   - Four of a kind, where four cards have the same label and one card has a different label: AA8AA
//   - Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
//   - Three of a kind, where three cards have the same label, and the remaining two cards are each different from any
//     other card in the hand: TTT98
//   - Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a
//     third label: 23432
//   - One pair, where two cards share one label, and the other three cards have a different label from the pair and
//     each other: A23A4
//   - High card, where all cards' labels are distinct: 23456
//
// Hands are primarily ordered based on type; for example, every full house is stronger than any three of a kind.
//
// If two hands have the same type, a second ordering rule takes effect. Start by comparing the first card in each hand.
// If these cards are different, the hand with the stronger first card is considered stronger. If the first card in each
// hand have the same label, however, then move on to considering the second card in each hand. If they differ, the hand
// with the higher second card wins; otherwise, continue with the third card in each hand, then the fourth, then the
// fifth.
//
// So, 33332 and 2AAAA are both four of a kind hands, but 33332 is stronger because its first card is stronger.
// Similarly, 77888 and 77788 are both a full house, but 77888 is stronger because its third card is stronger (and both
// hands have the same first and second card).
//
// To play Camel Cards, you are given a list of hands and their corresponding bid (your puzzle input). For example:
//
//     32T3K 765
//     T55J5 684
//     KK677 28
//     KTJJT 220
//     QQQJA 483
//
// This example shows five hands; each hand is followed by its bid amount. Each hand wins an amount equal to its bid
// multiplied by its rank, where the weakest hand gets rank 1, the second-weakest hand gets rank 2, and so on up to the
// strongest hand. Because there are five hands in this example, the strongest hand will have rank 5 and its bid will be
// multiplied by 5.
//
// So, the first step is to put the hands in order of strength:
//
//   - 32T3K is the only one pair and the other hands are all a stronger type, so it gets rank 1.
//   - KK677 and KTJJT are both two pair. Their first cards both have the same label, but the second card of KK677 is
//     stronger (K vs T), so KTJJT gets rank 2 and KK677 gets rank 3.
//   - T55J5 and QQQJA are both three of a kind. QQQJA has a stronger first card, so it gets rank 5 and T55J5 gets rank
//     4.
//
// Now, you can determine the total winnings of this set of hands by adding up the result of multiplying each hand's bid
// with its rank (765 * 1 + 220 * 2 + 28 * 3 + 684 * 4 + 483 * 5). So the total winnings in this example are 6440.
//
// Find the rank of every hand in your set. What are the total winnings?
//
// --- Part Two ---
//
// To make things a little more interesting, the Elf introduces one additional rule. Now, J cards are jokers - wildcards
// that can act like whatever card would make the hand the strongest type possible.
//
// To balance this, J cards are now the weakest individual cards, weaker even than 2. The other cards stay in the same
// order: A, K, Q, T, 9, 8, 7, 6, 5, 4, 3, 2, J.
//
// J cards can pretend to be whatever card is best for the purpose of determining hand type; for example, QJJQ2 is now
// considered four of a kind. However, for the purpose of breaking ties between two hands of the same type, J is always
// treated as J, not the card it's pretending to be: JKKK2 is weaker than QQQQ2 because J is weaker than Q.
//
// Now, the above example goes very differently:
//
//     32T3K 765
//     T55J5 684
//     KK677 28
//     KTJJT 220
//     QQQJA 483
//
//   - 32T3K is still the only one pair; it doesn't contain any jokers, so its strength doesn't increase.
//   - KK677 is now the only two pair, making it the second-weakest hand.
//   - T55J5, KTJJT, and QQQJA are now all four of a kind! T55J5 gets rank 3, QQQJA gets rank 4, and KTJJT gets rank 5.
//
// With the new joker rule, the total winnings in this example are 5905.
//
// Using the new joker rule, find the rank of every hand in your set. What are the new total winnings?
//----------------------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const bool verbose = true;
const int  cardsPerHand = 5;


int promoteScore(int score, int numJokers) {
    static struct {
        int score;
        int addedJokers[6];
    } promotionTable[7] {    // Jokers:    0     1     2     3     4     5
        { /* High Card       */    0, {    0,    1,   10,  100, 1000, 1000 } },
        { /* One Pair        */    1, {    1,   10,  100, 1000,   -1,   -1 } },
        { /* Two Pair        */    2, {    2,   11,   -1,   -1,   -1,   -1 } },
        { /* Three of a Kind */   10, {   10,  100, 1000,   -1,   -1,   -1 } },
        { /* Full House      */   11, {   11,   -1,   -1,   -1,   -1,   -1 } },
        { /* Four of a Kind  */  100, {  100, 1000,   -1,   -1,   -1,   -1 } },
        { /* Five of a Kind  */ 1000, { 1000,   -1,   -1,   -1,   -1,   -1 } },
    };

    for (int i=0;  i < 7;  ++i) {
        if (promotionTable[i].score == score)
            return promotionTable[i].addedJokers[numJokers];
    }

    return -1;
}


class Hand {
  public:
    Hand(string desc) {
        auto sit = desc.begin();

        for (int i = 0;  i < 5;  ++i)
            cards[i] = *sit++;

        ++sit; // Skip space.

        bid = 0;
        while (sit != desc.end())
            bid = 10*bid + (*sit++ - '0');

        calculateScore();
    }

    void calculateScore() {
        score = 0;
        char c[5] {cards[0], cards[1], cards[2], cards[3], cards[4]};

        // Gather jokers
        int numJokers = 0;
        for (int i = 0;  i < cardsPerHand;  ++i) {
            if (c[i] == 'J') {
                ++numJokers;
                c[i] = 0;
            }
        }

        for (int i = 0;  i < cardsPerHand-1;  ++i) {
            int dupeCount = 1;
            if (c[i] == 0)
                continue;
            for (int j = i+1;  j < cardsPerHand;  ++j) {
                if (c[i] == c[j]) {
                    ++dupeCount;
                    c[j] = 0;
                }
            }
            switch (dupeCount) {
                case 2:
                    score += 1;
                    break;
                case 3:
                    score += 10;
                    break;
                case 4:
                    score += 100;
                    break;
                case 5:
                    score += 1000;
                    break;
            }
        }

        // Account for any jokers.
        score = promoteScore(score, numJokers);
    }

    char cards[5];
    int score {0};
    int bid;
};


int cardRank(char c) {
    switch (c) {
        case 'A': return 14;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return  0;
        case 'T': return 10;
    }
    return c - '0';
}


bool compareHands(const Hand& h1, const Hand& h2) {
    if (h1.score != h2.score)
        return h1.score < h2.score;

    for (int i = 0;  i < cardsPerHand;  ++i) {
        if (cardRank(h1.cards[i]) != cardRank(h2.cards[i]))
            return cardRank(h1.cards[i]) < cardRank(h2.cards[i]);
    }

    return false;
}


int main() {
    string line;

    vector<Hand> hands;

    while (getline(cin, line)) {
        hands.push_back({line});
    }

    if (verbose) {
        for (const auto& hand : hands) {
            cout << "Hand: " << hand.cards[0] << hand.cards[1] << hand.cards[2] << hand.cards[3] << hand.cards[4]
                 << ", bid " << hand.bid
                 << ", score " << hand.score << '\n';
        }
        cout << '\n';
    }

    sort(hands.begin(), hands.end(), compareHands);

    if (verbose)
        cout << "--------------------------------------------------------------------------------\n\n";

    long totalWinnings = 0;
    int  numHands = hands.size();

    for (int rank = 1;  rank <= numHands;  ++rank) {
        const Hand& hand = hands[rank - 1];
        if (verbose) {
            cout << "Hand: rank " << rank << ", " << hand.cards[0] << hand.cards[1] << hand.cards[2] << hand.cards[3] << hand.cards[4]
                 << ", bid " << hand.bid
                 << ", score " << hand.score << '\n';
        }

        totalWinnings += hand.bid * rank;
    }

    if (verbose)
        cout << "\n\n";

    cout << "Total winnings: " << totalWinnings << '\n';

    return 0;
}
