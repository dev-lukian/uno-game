//
// Created by Lukian Bogdanets on 5/15/20.
//

#include "Card.h"

Card::Card(string displayValue) {
    this->displayValue = displayValue;

    // Checks to see if card is a plus card ('Y' means yes, 'N' means no)
    if (displayValue.length() == 3) {
        plus = displayValue[1];
        digit = 'N';
        color = displayValue[2];
    }
    else {
        plus = 'N';
        digit = displayValue[0];
        color = displayValue[1];
    }

    // Checks to see if card is a  wildcard
    if (displayValue[displayValue.length()-1] == 'W') {
        wildcard = true;
        color = 'N';
        if (displayValue.length() == 1) {
            plus = 'N';
            digit = 'N';
        }
    }
    else {
        wildcard = false;
    }

}

string Card::getDisplayValue() {
    return displayValue;
}

char Card::getPlus() {
    return plus;
}

char Card::getDigit() {
    return digit;
}

char Card::getColor() {
    return color;
}

bool Card::getWildCard() {
    return wildcard;
}

void Card::changeColor(char color) {
    this->color = color;
}