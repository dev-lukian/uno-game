//
// Created by Lukian Bogdanets on 5/14/20.
//

#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include "Card.h"
#include "DiscardPile.h"
#include "Deck.h"
#include "Player.h"
using namespace std;

// Displays first 3 cards in the discard pile
void displayDiscardPile();
// Displays current player's hand
void displayPlayerHand(Player& currentPlayer);
// Plays out if option 1 is chosen from moves by player
void discardOption(Player& currentPlayer, string discard, unsigned short playerOption, bool complete);
// Plays out if option 2 is chosen from moves by player
void pickUpOption(Player& currentPlayer, unsigned short playerOption, bool complete);

int main() {
    unsigned short numOfPlayers;
    vector<Player> players;
    Deck::initializeDeck();
    map<char, string> colorAssociation {
            {'R', "Red"},
            {'G', "Green"},
            {'Y', "Yellow"},
            {'B', "Blue"},
            {'N', "None"}
    };
    map<char, string> digitAssociation {
            {'R', "Reverse"},
            {'B', "Block"},
            {'0', "0"},
            {'1', "1"},
            {'2', "2"},
            {'3', "3"},
            {'4', "4"},
            {'5', "5"},
            {'6', "6"},
            {'7', "7"},
            {'8', "8"},
            {'9', "9"},
    };


    cout << "UNO Game" << endl;
    cout << "Number of players (2-10): ";
    cin >> numOfPlayers;
    cout << endl;

    // Prompt asking for each player's name
    for (int i = 1; i <= numOfPlayers; i++) {
        string name;

        cout << "Player #" << i << " name: ";
        cin >> name;

        // Places new player in vector of players
        players.emplace_back(name);
        players[i - 1].pickUpCards(7);
    }

    bool winningCondition = false;
    // Game starts with Player #1 going first
    unsigned short turn = 0;
    // Card from deck is placed in discard pile to start the game
    DiscardPile::addCard(Deck::getTopCard());
    Deck::subtract(1);

    // Checks to see if first card in discard pile is an action card, and if so perform the action

    // Game loops until a winning condition is found
    while (!winningCondition) {
        unsigned short playerOption;
        string discard;

        cout << "\nCurrent color to match: " << colorAssociation[DiscardPile::getCurrentColor()];
        cout << "\nCurrent digit to match: " << digitAssociation[DiscardPile::getCurrentDigit()];
        displayDiscardPile();
        cout << "\n-------------------------------------------------------" << endl;
        displayPlayerHand(players[turn]);
        cout << "\n\nWhat do you want to do?" << endl;
        cout << "1. Discard a card" << endl;
        cout << "2. Pick up a card" << endl;
        cin >> playerOption;
        bool moveCompleted = false;
        if (playerOption == 1) {
            discardOption(players[turn], discard, playerOption, moveCompleted);
            //actionCheck(cardInfo(discard), turn, players, numOfPlayers, currentColor);
        }
        else if (playerOption == 2) {
            pickUpOption(players[turn], playerOption, moveCompleted);
        }

        // Goes to next player at the end of the move
        if (turn == (numOfPlayers - 1)) {
            turn = 0;
        }
        else {
            turn++;
        }
    }
}

void displayDiscardPile() {
    cout << "\nDiscard Pile: ";
    if (DiscardPile::getNumOfCards() > 3) {
        for (int i = 0; i < 3; i++) {
            cout << DiscardPile::getCards()[i]->getDisplayValue() << " ";
        }
    }
    // Display settings in beginning of game when discard pile does not have 3 cards yet
    else {
        for (int i = 0; i < DiscardPile::getNumOfCards(); i++) {
            cout << DiscardPile::getCards()[i]->getDisplayValue() << " ";
        }
    }
}

void displayPlayerHand(Player& currentPlayer) {
    cout << currentPlayer.getName() << "'s hand: ";
    for (int i = 0; i < currentPlayer.getHand().size(); i++) {
        cout << currentPlayer.getHand()[i]->getDisplayValue() << " ";
    }
}

void discardOption(Player& currentPlayer, string discard, unsigned short playerOption, bool complete) {
    while (!complete) {
        try {
            cout << "Which card do you want to discard? (Type '0' to pick up a card)" << endl;
            cin >> discard;

            // Player chooses to pick up card as he cannot discard any
            if (discard == "0") {
                pickUpOption(currentPlayer, playerOption, complete);
                break;
            }

            // Local card object to check whether card can actually be discarded
            Card cardToDiscard =  Card(discard);
            auto acceptableCard = currentPlayer.cardMatch(&cardToDiscard);
            currentPlayer.discardCards(acceptableCard);

            // Card object created on heap to be added to Discard Pile now
            Card* discardedCard = new Card(discard);
            DiscardPile::addCard(discardedCard);
            complete = true;
        }
        catch (const char *error) {
            cout << error << endl;
        }
    }
}

void pickUpOption(Player& currentPlayer, unsigned short playerOption, bool complete) {
    currentPlayer.pickUpCards(1);
    Card* cardPickedUp = currentPlayer.getHand()[currentPlayer.getNumOfCards() - 1];
    cout << "You picked up a " << cardPickedUp->getDisplayValue() << endl;

    // Checks to see if the card just picked up from the deck can be discarded right away
    vector<Card*>::iterator acceptableCard;
    try {
        acceptableCard = currentPlayer.cardMatch(cardPickedUp);
    }
    catch (const char *error) {
        complete = true;
    }

    // Card just picked up can be discarded so gives option to user if he would like to
    while (!complete) {
        try {
            cout << "Would you like to discard the card you just picked up?"
                 << endl;
            cout << "1. Yes" << endl;
            cout << "2. No" << endl;
            cin >> playerOption;
            if (playerOption == 1) {
                currentPlayer.discardCards(acceptableCard);
                DiscardPile::addCard(cardPickedUp);
                complete = true;
            } else if (playerOption == 2) {
                complete = true;
            } else {
                throw "That is not a valid input. Please try again.";
            }
        }
        catch (const char *error) {
            cout << error << endl;
        }
    }
}
