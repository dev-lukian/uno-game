//
// Created by Lukian Bogdanets on 5/14/20.
//

#include "Player.h"

Player::Player(string& name) {
    this->name = name;
    numOfCards = 0;
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

// Picks up "num" amount of cards from the deck
bool Player::pickUpCards(unsigned short num, Deck& deck) {
    short topCardIndex = deck.getNumOfCards() - 1;

    if (topCardIndex < 0) {
        return false;
    }

    for (int i = 0; i < num; i++) {
        hand.push_back(deck.getCards()[deck.getNumOfCards() - 1 - i]);
        numOfCards++;
    }

    deck.subtractCards(num);
    return true;
}

vector<string>::iterator Player::cardMatch(string& playerCard, map<string, char> playerCardInfo, string& pileCard, map<string, char> pileCardInfo) {
    if (playerCard.empty()) {
        throw "You do not have such a card in your hand! Try again.";
    }

    for (auto iter = hand.begin(); iter != hand.end(); iter++) {
        if (playerCard == *iter) {
            if (playerCardInfo["Wildcard"] == 'Y' || playerCardInfo["Color"] == pileCardInfo["Color"] || playerCardInfo["Digit"] == pileCardInfo["Digit"]) {
                return iter;
            }
            else {
                throw "Card does not match with card on top of discard pile! Try again.";
            }
        }
    }

    throw "You do not have such a card in your hand! Try again.";
}

void Player::placeDownCards(vector<string>::iterator index) {
    hand.erase(index);
    numOfCards--;
}