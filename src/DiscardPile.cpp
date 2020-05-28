//
// Created by Lukian Bogdanets on 5/15/20.
//

#include "DiscardPile.h"

unsigned int DiscardPile::numOfCards = 0;
deque<Card*> DiscardPile::cards;
char DiscardPile::currentColor;
char DiscardPile::currentDigit;

unsigned int DiscardPile::getNumOfCards() {
    return numOfCards;
}


deque<Card*> DiscardPile::getCards() {
    return cards;
}

Card* DiscardPile::getTopCard() {
    return cards[0];
}

char DiscardPile::getCurrentColor() {
    return currentColor;
}

char DiscardPile::getCurrentDigit() {
    return currentDigit;
}

void DiscardPile::addCard(Card* card) {
    cards.push_front(card);
    numOfCards++;

    setCurrentColor(card->getColor());
    setCurrentDigit(card->getDigit());
}

void DiscardPile::setCurrentColor(char newColor) {
    currentColor = newColor;
}

void DiscardPile::setCurrentDigit(char newDigit) {
    currentDigit = newDigit;
}