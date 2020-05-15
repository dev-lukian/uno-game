//
// Created by Lukian Bogdanets on 5/14/20.
//

#include <iostream>
#include <vector>
#include "Deck.h"
#include "Player.h"
using namespace std;

int main() {
    unsigned short numOfPlayers;
    vector<Player> players;
    Deck unoDeck = Deck();
    vector<string> discardPile;


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
}
