//
// Created by Lukian Bogdanets on 5/14/20.
//

#pragma once

#include <vector>
#include <string>
#include "Card.h"

using namespace std;

class Deck {
private:
    static unsigned int numOfCards;
    static vector<Card*> cards;

public:
    // Getter(s)
    static unsigned int getNumOfCards();
    static vector<Card*> getCards();
    static Card* getTopCard();

    // Setter(s)
    static void initializeDeck();
    static void shuffleDeck();
    static void subtract(unsigned short num);
};

