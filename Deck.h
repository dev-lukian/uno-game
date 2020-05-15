//
// Created by Lukian Bogdanets on 5/14/20.
//

#pragma once

#include <vector>
#include <string>

using namespace std;

class Deck {
private:
    unsigned int numOfCards;
    vector<string> cards;
    const vector<string> suits {"R", "G", "Y", "B"};

public:
    // Constructor(s)
    Deck();

    // Getter(s)
    unsigned int getNumOfCards();
    vector<string> getCards();

    // Setter(s)
    void initializeDeck();
    void shuffleDeck();
    void subtractCards(unsigned short num);
};

