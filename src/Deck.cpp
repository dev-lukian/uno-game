//
// Created by Lukian Bogdanets on 5/14/20.
//

#include <random>
#include <algorithm>
#include "Deck.h"

// Initial UNO deck consists of 108 cards
unsigned int Deck::numOfCards = 108;
vector<Card*> Deck::cards;

unsigned int Deck::getNumOfCards() {
    return numOfCards;
}

vector<Card*> Deck::getCards() {
    return cards;
}

Card* Deck::getTopCard() {
    return cards[numOfCards - 1];
}

/* There are four suits, Red, Green, Yellow and Blue, each consisting of one 0 card,
 * two 1 cards, two 2s, 3s, 4s, 5s, 6s, 7s, 8s and 9s; two Draw Two cards; two Skip cards;
 * and two Reverse cards. In addition there are four Wild cards and four Wild Draw Four cards.
 */
void Deck::initializeDeck() {
    vector<string> suits {"R", "G", "Y", "B"};

    for (int i = 0; i < 4; i++) {
        // Creating both wildcard +4 and wildcard
        cards.push_back(new Card("+4W"));
        cards.push_back(new Card("W"));

        // Creating the 4 different color suits
        cards.push_back(new Card("0" + suits[i]));
        for (int j = 0; j < 2; j++) {
            cards.push_back(new Card("1" + suits[i]));
            cards.push_back(new Card("2" + suits[i]));
            cards.push_back(new Card("3" + suits[i]));
            cards.push_back(new Card("4" + suits[i]));
            cards.push_back(new Card("5" + suits[i]));
            cards.push_back(new Card("6" + suits[i]));
            cards.push_back(new Card("7" + suits[i]));
            cards.push_back(new Card("8" + suits[i]));
            cards.push_back(new Card("9" + suits[i]));
            cards.push_back(new Card("+2" + suits[i]));
            cards.push_back(new Card("B" + suits[i]));
            cards.push_back(new Card("R" + suits[i]));
        }
    }

    shuffleDeck();
}

// Uses time as a seed to get a different shuffle every time
void Deck::shuffleDeck() {
    int seed = time(nullptr);
    shuffle(begin(cards), end(cards), default_random_engine(seed));
}

/* Number of cards is subtracted by "num" amount. Important to keep track of number of cards
 * as cards are not actually getting deleted themselves from the vector. New cards are getting pulled
 * from deck by keeping index where we left off, which we do with 'numOfCards'
 */
void Deck::subtract(unsigned short num) {
    numOfCards -= num;
}
