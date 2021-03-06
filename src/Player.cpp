//
// Created by Lukian Bogdanets on 5/14/20.
//

#include "Player.h"

Player::Player(string& name) {
    this->name = name;
    numOfCards = 0;
    winner = false;
}

string Player::getName() {
    return name;
}

unsigned short Player::getNumOfCards() {
    return numOfCards;
}

vector<Card*> Player::getHand() {
    return hand;
}

bool Player::getWinner() {
    return winner;
}

// Picks up "num" amount of cards from the deck
void Player::pickUpCards(unsigned short num) {
    for (int i = 0; i < num; i++) {
        hand.push_back(Deck::getTopCard());
        numOfCards++;
        Deck::subtract(1);
    }
}

vector<Card*>::iterator Player::cardMatch(Card* playerCard) {
    for (auto iter = hand.begin(); iter != hand.end(); iter++) {
        if (playerCard->getDisplayValue() == (*iter)->getDisplayValue()) {
            if (playerCard->getWildCard() || playerCard->getColor() == DiscardPile::getCurrentColor() || playerCard->getDigit() == DiscardPile::getCurrentDigit()) {
                return iter;
            }
            else {
                throw invalid_argument("Card does not match with card on top of discard pile! Try again.");
            }
        }
    }

    throw invalid_argument("You do not have such a card in your hand! Try again.");
}

void Player::discardCards(vector<Card*>::iterator index) {
    hand.erase(index);
    numOfCards--;
}

void Player::setWinner() {
    winner = true;
}
