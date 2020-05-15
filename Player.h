//
// Created by Lukian Bogdanets on 5/14/20.
//

#pragma once

#include <vector>
#include <string>
#include "Deck.h"

using namespace std;

class Player {
private:
    string name;
    unsigned short numOfCards;
    vector<string> hand;

public:
    // Constructor(s)
    Player(string& name);

    // Getter(s)
    string getName();
    unsigned short getNumOfCards();
    vector<string> getHand();

    // Setter(s)
    void pickUpCards(unsigned short num, Deck& deck);
};

