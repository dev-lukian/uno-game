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
// Checks to see if card in discard pile is an action card, and if so perform the action
void actionCheck(Card* card, unsigned short& turn, vector<Player>& players, unsigned short& numOfPlayers, bool& reverse, bool firstCard);
// Goes on to next player (direction depends on value of reverse)
void nextPlayer(unsigned short& turn, bool& reverse, unsigned short& numOfPlayers);
// Plays out if option 1 is chosen from moves by player
Card* discardOption(Player& currentPlayer, string discard, unsigned short playerOption, bool& winningCondition);
// Plays out if option 2 is chosen from moves by player
Card* pickUpOption(Player& currentPlayer, unsigned short playerOption, bool& winningCondition);

int main() {
    unsigned short numOfPlayers;
    vector<Player> players;
    Deck::initializeDeck();
    map<char, string> colorAssociation {
            {'R', "Red"},
            {'G', "Green"},
            {'Y', "Yellow"},
            {'B', "Blue"},
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
            {'N', "None"}
    };


    cout << "UNO Game" << endl;
    while (true) {
        try {
            cout << "Number of players (2-10): ";
            cin >> numOfPlayers;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(80, '\n');
                throw invalid_argument("Please type in a valid number. Please try again");
            }
            else if (numOfPlayers < 2 || numOfPlayers > 10 ) {
                throw invalid_argument("Only 2-10 players can play UNO. Please try again.");
            }
            else {
                break;
            }
        }
        catch (invalid_argument& error) {
            cout << error.what() << endl;
        }
    }
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
    // Game starts with turns going forward, switches if "Reverse" card is played
    bool reverse = false;
    // If first card placed in discard pile is +4W, deck must be reshuffled
    if (Deck::getTopCard()->getWildCard() && Deck::getTopCard()->getDraw() != 'N') {
        cout << "First card draw is a +4 wildcard. Deck is reshuffled." << endl;
        Deck::shuffleDeck();
    }
    // Card from deck is placed in discard pile to start the game
    DiscardPile::addCard(Deck::getTopCard());
    Deck::subtract(1);

    // Checks to see if first card in discard pile is an action card, and if so perform the action
    actionCheck(DiscardPile::getTopCard(), turn, players, numOfPlayers, reverse, true);
    if (reverse) {
        if (turn == 0) {
            turn = (numOfPlayers - 1);
        }
        else {
            turn--;
        }
    }

    // Game loops until a winning condition is found
    while (!winningCondition) {
        unsigned short playerOption;
        string discard;

        cout << "\nCurrent color to match: " << colorAssociation[DiscardPile::getCurrentColor()];
        cout << "\nCurrent digit to match: " << digitAssociation[DiscardPile::getCurrentDigit()];
        displayDiscardPile();
        cout << "\n-------------------------------------------------------" << endl;
        displayPlayerHand(players[turn]);

        while (true) {
            try {
                cout << "\nWhat do you want to do?" << endl;
                cout << "1. Discard a card" << endl;
                cout << "2. Pick up a card" << endl;
                cin >> playerOption;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(80, '\n');
                    throw invalid_argument("Invalid menu option. Please try again.");
                }
                else if (playerOption != 1 && playerOption != 2) {
                    throw invalid_argument("Invalid menu option. Please try again.");
                }
                else {
                    break;
                }
            }
            catch (invalid_argument& error) {
                cout << error.what() << endl;
            }
        }
        if (playerOption == 1) {
            Card* discardedCard = discardOption(players[turn], discard, playerOption, winningCondition);
            if (discardedCard != nullptr) {
                actionCheck(discardedCard, turn, players, numOfPlayers, reverse, false);
            }
        }
        else if (playerOption == 2) {
            Card* pickedUpCard = pickUpOption(players[turn], playerOption, winningCondition);
            if (pickedUpCard != nullptr) {
                actionCheck(pickedUpCard, turn, players, numOfPlayers, reverse, false);
            }
        }

        nextPlayer(turn, reverse, numOfPlayers);
    }

    Player winner = players[0];

    for (int i = 0; i < numOfPlayers; i++) {
        // Player wins by discarding all the cards in his hand
        if (players[i].getWinner()) {
            winner = players[i];
            break;
        }

        // Player wins by having least number of cards in his hand when the deck is gone
        if (players[i].getNumOfCards() < winner.getNumOfCards()) {
            winner = players[i];
        }
    }

    // Winner is announced
    cout << endl << winner.getName() << " is the winner!" << endl << "Game Over" << endl;

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
    cout << endl;
}

void actionCheck(Card* card, unsigned short& turn, vector<Player>& players, unsigned short& numOfPlayers, bool& reverse, bool firstCard) {
    // bool firstCard is true if the card being checked is the first card in the discard pile
    int lastTurn = numOfPlayers - 1;

    // "Block" action
    if (card->getDigit() == 'B') {
        cout << endl << "The card discarded is a block card!" << endl;
        if (turn == lastTurn) {
            turn = 0;
            cout << players[0].getName() << " skips his turn. " << endl;
        }
        else {
            turn++;
            if (firstCard) {
                cout << players[0].getName() << " skips his turn. " << endl;
            }
            else {
                cout << players[turn].getName() << " skips his turn. " << endl;
            }
        }
    }
    // "Reverse" action
    else if (card->getDigit() == 'R') {
        cout << endl << "The card discarded is a reverse card!" << endl;
        // Switches to opposite direction as previously
        reverse = !reverse;
    }
    // "Add cards" action
    else if (card->getDraw() != 'N') {
        cout << endl << "The card discarded is a draw card!" << endl;
        if (firstCard || turn == lastTurn) {
            int num = card->getDraw() - 48;
            players[0].pickUpCards(num);
            cout << players[0].getName() << " picks up " << num << " cards." << endl;
        }
        else {
            int num = card->getDraw() - 48;
            players[turn + 1].pickUpCards(num);
            cout << players[turn + 1].getName() << " picks up " << num << " cards." << endl;
        }
    }

    // "Wildcard" action
    if (card->getWildCard()) {
        while(true) {
            try {
                char newColor;
                cout << endl << "The card discarded is a wildcard!" << endl;
                cout
                        << "What color " << players[turn].getName() << " would you like to change to? (Type 'R' for Red, 'G' for Green, 'B' for Blue, and 'Y' for Yellow)"
                        << endl;
                cin >> newColor;

                if (newColor == 'R' || newColor == 'G' || newColor == 'B' || newColor == 'Y') {
                    DiscardPile::setCurrentColor(newColor);
                    break;
                }
                else {
                    throw "Invalid color! Try again.";
                }
            }
            catch (const char* error) {
                cout << error << endl;
            }
        }
    }
}

void nextPlayer(unsigned short& turn, bool& reverse, unsigned short& numOfPlayers) {
    // Goes backwards
    if (reverse) {
        if (turn == 0) {
            turn = (numOfPlayers - 1);
        }
        else {
            turn--;
        }
    }
    // Goes forwards
    else {
        if (turn == (numOfPlayers - 1)) {
            turn = 0;
        }
        else {
            turn++;
        }
    }
}

Card* discardOption(Player& currentPlayer, string discard, unsigned short playerOption, bool& winningCondition) {
    while (true) {
        try {
            cout << "Which card do you want to discard? (Type '0' to pick up a card)" << endl;
            cin >> discard;

            // Player chooses to pick up card as he cannot discard any
            if (discard == "0") {
                Card* pickedUpCard = pickUpOption(currentPlayer, playerOption, winningCondition);
                return pickedUpCard;
            }

            // Local card object to check whether card can actually be discarded
            Card cardToDiscard =  Card(discard);
            auto acceptableCard = currentPlayer.cardMatch(&cardToDiscard);
            currentPlayer.discardCards(acceptableCard);

            // Winning condition check: Player has no more cards in his hand
            if (currentPlayer.getNumOfCards() == 0) {
                winningCondition = true;
                currentPlayer.setWinner();
                return nullptr;
            }

            // Card object created on heap to be added to Discard Pile now
            Card* discardedCard = new Card(discard);
            DiscardPile::addCard(discardedCard);
            return discardedCard;
        }
        catch (const char *error) {
            cout << error << endl;
        }
    }
}

Card* pickUpOption(Player& currentPlayer, unsigned short playerOption, bool& winningCondition) {
    // Winning condition check: Deck runs out of cards
    if (Deck::getNumOfCards() == 0) {
        winningCondition = true;
        return nullptr;
    }

    currentPlayer.pickUpCards(1);
    Card* cardPickedUp = currentPlayer.getHand()[currentPlayer.getNumOfCards() - 1];
    cout << "You picked up a " << cardPickedUp->getDisplayValue() << endl;

    // Checks to see if the card just picked up from the deck can be discarded right away
    vector<Card*>::iterator acceptableCard;
    try {
        acceptableCard = currentPlayer.cardMatch(cardPickedUp);
    }
    catch (const char *error) {
        return nullptr;
    }

    // Card just picked up can be discarded so gives option to user if he would like to
    while (true) {
        try {
            cout << "Would you like to discard the card you just picked up?"
                 << endl;
            cout << "1. Yes" << endl;
            cout << "2. No" << endl;
            cin >> playerOption;
            if (playerOption == 1) {
                currentPlayer.discardCards(acceptableCard);
                // Winning condition check: Player has no more cards in his hand
                if (currentPlayer.getNumOfCards() == 0) {
                    winningCondition = true;
                    currentPlayer.setWinner();
                    return nullptr;
                }
                DiscardPile::addCard(cardPickedUp);
                return cardPickedUp;
            } else if (playerOption == 2) {
                return nullptr;
            } else {
                throw "That is not a valid input. Please try again.";
            }
        }
        catch (const char *error) {
            cout << error << endl;
        }
    }
}
