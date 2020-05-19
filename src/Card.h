//
// Created by Lukian Bogdanets on 5/15/20.
//

#pragma once

#include <string>

using namespace std;

class Card {
private:
    string displayValue; // The value you will see in the TUI
    char draw;
    char digit;
    char color;
    bool wildcard;

public:
    // Constructor(s)
    Card(string displayValue);

    // Getter(s)
    string getDisplayValue();
    char getDraw();
    char getDigit();
    char getColor();
    bool getWildCard();

    // Setter(s)
    void changeColor(char color);
};

