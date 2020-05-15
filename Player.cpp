//
// Created by Lukian Bogdanets on 5/14/20.
//

#include "Player.h"

Player::Player(string& name) {
    this->name = name;
    numOfCards = 7;
    hand.resize(7);
}

string Player::getName() {
    return name;
}

unsigned short Player::getNumOfCards() {
    return numOfCards;
}

vector<string> Player::getHand() {
    return hand;
}

void Player::pickUpCards(unsigned short num, Deck& deck) {
    for (int i = 0; i < num; i++) {
        hand[i] = deck.getCards()[deck.getNumOfCards() - 1 - i];
    }

    deck.subtractCards(num);
}