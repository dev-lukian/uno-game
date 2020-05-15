//
// Created by Lukian Bogdanets on 5/14/20.
//

#include <random>
#include "Deck.h"

Deck::Deck() {
    numOfCards = 108;
    initializeDeck();
}

unsigned int Deck::getNumOfCards() {
    return numOfCards;
}

vector<string> Deck::getCards() {
    return cards;
}

void Deck::initializeDeck() {
    for (int i = 0; i < 4; i++) {
        // Creating both wildcard +4 and wildcard
        cards.push_back("+4W");
        cards.push_back("WW");

        // Creating the 4 different color suits
        cards.push_back("0" + suits[i]);
        for (int j = 0; j < 2; j++) {
            cards.push_back("1" + suits[i]);
            cards.push_back("2" + suits[i]);
            cards.push_back("3" + suits[i]);
            cards.push_back("4" + suits[i]);
            cards.push_back("5" + suits[i]);
            cards.push_back("6" + suits[i]);
            cards.push_back("7" + suits[i]);
            cards.push_back("8" + suits[i]);
            cards.push_back("9" + suits[i]);
            cards.push_back("+2" + suits[i]);
            cards.push_back("B" + suits[i]);
            cards.push_back("R" + suits[i]);
        }
    }

    shuffleDeck();
}

void Deck::shuffleDeck() {
    int seed = time(nullptr);
    shuffle(begin(cards), end(cards), default_random_engine(seed));
}

// "Num" amount of cards are deleted from end of the vector
void Deck::subtractCards(unsigned short num) {
    numOfCards -= num;
}
