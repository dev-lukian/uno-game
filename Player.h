//
// Created by Lukian Bogdanets on 5/14/20.
//

#pragma once

#include <vector>
#include <map>
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
    bool pickUpCards(unsigned short num, Deck& deck);
    vector<string>::iterator cardMatch(string& playerCard, map<string, char> playerCardInfo, string& pileCard, map<string, char> pileCardInfo);
    void placeDownCards(vector<string>::iterator index);
};

