//
// Created by Lukian Bogdanets on 5/15/20.
//

#pragma once

#include <deque>
#include "Card.h"

class DiscardPile {
private:
    static unsigned int numOfCards;
    static deque<Card*> cards;
    static char currentColor;
    static char currentDigit;

public:
    // Getter(s)
    static unsigned int getNumOfCards();
    static deque<Card*> getCards();
    static Card* getTopCard();
    static char getCurrentColor();
    static char getCurrentDigit();

    // Setter(s)
    static void addCard(Card* card);
    static void setCurrentColor(char newColor);
    static void setCurrentDigit(char newDigit);
};

