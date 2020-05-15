//
// Created by Lukian Bogdanets on 5/14/20.
//

#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include "Deck.h"
#include "Player.h"
using namespace std;

// Displays first 3 cards in the discard pile
void displayDiscardPile(deque<string>& discardPile);
// Displays current player's hand
void displayPlayerHand(Player& currentPlayer);
// Breaks card into a map of its information
map<string, char> cardInfo(string& card);

int main() {
    unsigned short numOfPlayers;
    vector<Player> players;
    Deck unoDeck = Deck();
    deque<string> discardPile;


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
        players[i - 1].pickUpCards(7, unoDeck);
    }

    bool winningCondition = false;
    // Game starts with Player #1 going first
    unsigned short turn = 0;
    // Card from deck is placed in discard pile to start the game
    discardPile.push_front(unoDeck.getCards()[unoDeck.getNumOfCards() - 1]);
    unoDeck.subtractCards(1);

    // Game loops until a winning condition is found
    while (!winningCondition) {
        unsigned short playerOption;
        string discard;

        displayDiscardPile(discardPile);
        cout << "\n-------------------------------------------------------" << endl;
        displayPlayerHand(players[turn]);
        cout << "\n\nWhat do you want to do?" << endl;
        cout << "1. Discard a card" << endl;
        cout << "2. Pick up a card" << endl;
        cin >> playerOption;
        bool moveCompleted = false;
        if (playerOption == 1) {
            while (!moveCompleted) {
                try {
                    cout << "Which card do you want to discard? (Type 'menu' to go back to move options)" << endl;
                    cin >> discard;

                    auto acceptableCard = players[turn].cardMatch(discard, cardInfo(discard), discardPile[0],
                                                                  cardInfo(discardPile[0]));
                    players[turn].placeDownCards(acceptableCard);
                    discardPile.push_front(discard);
                    moveCompleted = true;
                }
                catch (const char *error) {
                    cout << error << endl;
                }
            }
        }
        else if (playerOption == 2) {
            players[turn].pickUpCards(1, unoDeck);
            string topCard = players[turn].getHand()[players[turn].getNumOfCards() - 1];
            cout << "You picked up a " << topCard << endl;
            vector<string>::iterator acceptableCard;
            try {
                acceptableCard = players[turn].cardMatch(topCard, cardInfo(topCard), discardPile[0],
                                                         cardInfo(discardPile[0]));
            }
            catch (const char *error) {
                moveCompleted = true;
            }

            while (!moveCompleted) {
                try {
                    cout << "Would you like to discard the card you just picked up?"
                         << endl;
                    cout << "1. Yes" << endl;
                    cout << "2. No" << endl;
                    cin >> playerOption;
                    if (playerOption == 1) {
                        players[turn].placeDownCards(acceptableCard);
                        discardPile.push_front(topCard);
                        moveCompleted = true;
                    } else if (playerOption == 2) {
                        moveCompleted = true;
                    } else {
                        throw "That is not a valid input. Please try again.";
                    }
                }
                catch (const char *error) {
                    cout << error << endl;
                }
            }
        }

        if (turn == (numOfPlayers - 1)) {
            turn = 0;
        }
        else {
            turn++;
        }
    }
}

void displayDiscardPile(deque<string>& discardPile) {
    cout << "\nDiscard Pile: ";
    if (discardPile.size() > 3) {
        for (int i = 0; i < 3; i++) {
            cout << discardPile[i] << " ";
        }
    }
    // Display settings in beginning of game when discard pile does not have 3 cards yet
    else {
        for (int i = 0; i < discardPile.size(); i++) {
            cout << discardPile[i] << " ";
        }
    }
}

void displayPlayerHand(Player& currentPlayer) {
    cout << currentPlayer.getName() << "'s hand: ";
    for (int i = 0; i < currentPlayer.getHand().size(); i++) {
        cout << currentPlayer.getHand()[i] << " ";
    }
}

map<string, char> cardInfo(string& card) {
    map<string, char> cardInfo;

    // Checks to see if card exists
    if (card.length() < 2 || card.length() > 3) {
        return cardInfo;
    }
    // Checks to see if card is a plus card ('Y' means yes, 'N' means no)
    else if (card.length() == 3) {
        cardInfo.emplace("Plus", card[1]);
        cardInfo.emplace("Digit", 'N');
        cardInfo.emplace("Color", card[2]);
    }
    else {
        cardInfo.emplace("Plus", 'N');
        cardInfo.emplace("Digit", card[0]);
        cardInfo.emplace("Color", card[1]);
    }

    // Checks to see if card is a  wildcard
    if (cardInfo["Color"] == 'W') {
        cardInfo.emplace("Wildcard", 'Y');
    }
    else {
        cardInfo.emplace("Wildcard", 'N');

    }

    return cardInfo;
}
