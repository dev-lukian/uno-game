//
// Created by Lukian Bogdanets on 5/14/20.
//

#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "DiscardPile.h"
#include "Deck.h"

using namespace std;

class Player {
private:
    string name;
    unsigned short numOfCards;
    vector<Card*> hand;
    bool winner;

public:
    // Constructor(s)
    Player(string& name);

    // Getter(s)
    string getName();
    unsigned short getNumOfCards();
    vector<Card*> getHand();
    bool getWinner();

    // Setter(s)
    void pickUpCards(unsigned short num);
    vector<Card*>::iterator cardMatch(Card* playerCard);
    void discardCards(vector<Card*>::iterator index);
    void setWinner();
};

