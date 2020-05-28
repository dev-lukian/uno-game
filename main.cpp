//
// Created by Lukian Bogdanets on 5/14/20.
//

#include <curses.h>
#include <cctype>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include "Card.h"
#include "DiscardPile.h"
#include "Deck.h"
#include "Player.h"
using namespace std;

// Displays highlight menu options & returns option player chooses
int displayMenu(vector<string>& choices, int highlight, int y, WINDOW* window);
// Displays first 3 cards in the discard pile
void displayDiscardPile();
// Displays current player's hand
void displayPlayerHand(Player& currentPlayer);
// Checks to see if card in discard pile is an action card, and if so perform the action
void actionCheck(WINDOW* window, Card* card, unsigned short& turn, vector<Player>& players, int& numOfPlayers, bool& reverse, bool firstCard);
// Goes on to next player (direction depends on value of reverse)
void nextPlayer(unsigned short& turn, bool& reverse, int& numOfPlayers);
// Plays out if option 1 is chosen from moves by player
Card* discardOption(WINDOW* window, Player& currentPlayer, unsigned short playerOption, bool& winningCondition);
// Plays out if option 2 is chosen from moves by player
Card* pickUpOption(WINDOW* window, Player& currentPlayer, unsigned short playerOption, bool& winningCondition);

int main() {

    // Curses initialization
    WINDOW* main = initscr();
    keypad(main, true);
    cbreak();

    // Get screen size
    int yMax, xMax;
    getmaxyx(main, yMax, xMax);

    // Creates 2nd window
    WINDOW* inputWin = newwin(8, 80, 11, 0);
    keypad(inputWin, true);

    int numOfPlayers = 0;
    vector<string> choices;
    int playerOption = 0;
    vector<Player> players;
    Deck::initializeDeck();
    map<char, string> colorAssociation{
            {'R', "Red"},
            {'G', "Green"},
            {'Y', "Yellow"},
            {'B', "Blue"},
    };
    map<char, string> digitAssociation{
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


    printw("UNO Game");

    while (true) {
        try {
            mvprintw(2, 0, "Number of players (2-10): ");
            char input[2];
            clrtoeol();
            getnstr(input, 2);
            numOfPlayers = stoi(input);
            if (numOfPlayers < 2 || numOfPlayers > 10) {
                throw invalid_argument("Only 2-10 players can play UNO. Please try again.");
            } else {
                break;
            }
        }
        catch (invalid_argument& error) {
            attron(A_BOLD);
            clrtobot();
            mvprintw(4, 0, error.what());
            attroff(A_BOLD);
        }
    }

    // Prompt asking whether or not default names should be given
    printw("\nHow would you like to name the players?\n");
    choices.emplace_back("1. Give each player a default name (Player [#])");
    choices.emplace_back("2. Manually name each player");
    playerOption = displayMenu(choices, playerOption, 5, main);
    choices.clear();

    // Default name given to each player
    if (playerOption == 0) {
        for (int i = 1; i <= numOfPlayers; i++) {
            string name = "Player " + to_string(i);

            mvprintw(i+7, 0, "Player %d", i);

            // Places new player in vector of players
            players.emplace_back(name);
            players[i - 1].pickUpCards(7);
        }
    }
    // Prompt asking for each player's name
    else if (playerOption == 1) {
        for (int i = 1; i <= numOfPlayers; i++) {
            char input[20];

            mvprintw(i+7, 0, "Player %d name: ", i);
            getnstr(input, 20);

            // Places new player in vector of players
            string name(input);
            players.emplace_back(name);
            players[i - 1].pickUpCards(7);
        }
    }

    bool winningCondition = false;
    // Game starts with Player #1 going first
    unsigned short turn = 0;
    // Game starts with turns going forward, switches if "Reverse" card is played
    bool reverse = false;

    mvprintw(9 + numOfPlayers, 0, "First card is drawn from the deck...\n\n");
    // If first card placed in discard pile is +4W, deck must be reshuffled
    if (Deck::getTopCard()->getWildCard() && Deck::getTopCard()->getDraw() != 'N') {
        printw("\nFirst card drawn is a +4 wildcard. Deck is reshuffled.\n\n");
        Deck::shuffleDeck();
    }
    // Card from deck is placed in discard pile to start the game
    DiscardPile::addCard(Deck::getTopCard());
    Deck::subtract(1);

    // Checks to see if first card in discard pile is an action card, and if so perform the action
    actionCheck(main, DiscardPile::getTopCard(), turn, players, numOfPlayers, reverse, true);
    if (reverse) {
        if (turn == 0) {
            turn = (numOfPlayers - 1);
        } else {
            turn--;
        }
    }

    mvprintw(yMax - 1, 0, "Press any key to start playing");
    getch();
    clear();

    // Game loops until a winning condition is found
    while (!winningCondition) {
        refresh();
        wrefresh(inputWin);

        mvprintw(0, 0, "Current color to match: %s", colorAssociation[DiscardPile::getCurrentColor()].c_str());
        mvprintw(1, 0, "Current digit to match: %s", digitAssociation[DiscardPile::getCurrentDigit()].c_str());
        displayDiscardPile();
        mvprintw(4, 0, "-------------------------------------------------------");
        displayPlayerHand(players[turn]);

        mvprintw(7, 0, "What do you want to do?");
        choices.emplace_back("1. Discard a card");
        choices.emplace_back("2. Pick up a card");
        playerOption = displayMenu(choices, playerOption, 8, main);
        choices.clear();

        if (playerOption == 0) {
            Card *discardedCard = discardOption(inputWin, players[turn], playerOption, winningCondition);
            if (discardedCard != nullptr) {
                actionCheck(inputWin, discardedCard, turn, players, numOfPlayers, reverse, false);
            }
        } else if (playerOption == 1) {
            Card *pickedUpCard = pickUpOption(inputWin, players[turn], playerOption, winningCondition);
            if (pickedUpCard != nullptr) {
                actionCheck(inputWin, pickedUpCard, turn, players, numOfPlayers, reverse, false);
            }
        }

        nextPlayer(turn, reverse, numOfPlayers);
        mvprintw(yMax - 1, 0, "Press any key to go on to the next player");
        wgetch(main);
        clear();
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
    attron(A_BOLD | A_UNDERLINE);
    printw("%s is the winner!", winner.getName().c_str());
    printw("\nGame Over");
    attroff(A_BOLD | A_UNDERLINE);
    getch();

    endwin();
    return EXIT_SUCCESS;
}

int displayMenu(vector<string>& choices, int highlight, int y, WINDOW* window) {
    while(true) {
        int choice;
        for(int i = 0; i < choices.size(); i++) {
            if (i == highlight) {
                wattron(window, A_REVERSE);
            }
            mvwprintw(window, y+i, 0, choices[i].c_str());
            wattroff(window, A_REVERSE);
        }

        choice = wgetch(window);

        switch(choice) {
            case KEY_UP:
                highlight--;
                if(highlight == -1) {
                    highlight = 0;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if(highlight == choices.size()) {
                    highlight = choices.size() - 1;
                }
                break;
        }

        if (choice == 10) {
            break;
        }
    }

    return highlight;
}

void displayDiscardPile() {
    mvprintw(3, 0, "Discard Pile: ");
    if (DiscardPile::getNumOfCards() > 3) {
        for (int i = 0; i < 3; i++) {
            printw("%s ", DiscardPile::getCards()[i]->getDisplayValue().c_str());
        }
    }
    // Display settings in beginning of game when discard pile does not have 3 cards yet
    else {
        for (int i = 0; i < DiscardPile::getNumOfCards(); i++) {
            printw("%s ", DiscardPile::getCards()[i]->getDisplayValue().c_str());
        }
    }
}

void displayPlayerHand(Player& currentPlayer) {
    mvprintw(5, 0, "%s's hand: ", currentPlayer.getName().c_str());
    for (int i = 0; i < currentPlayer.getHand().size(); i++) {
        printw("%s ", currentPlayer.getHand()[i]->getDisplayValue().c_str());
    }
}

void actionCheck(WINDOW* window, Card* card, unsigned short& turn, vector<Player>& players, int& numOfPlayers, bool& reverse, bool firstCard) {
    if (!firstCard) {
        wclear(window);
    }

    // bool firstCard is true if the card being checked is the first card in the discard pile
    int lastTurn = numOfPlayers - 1;

    // "Block" action
    if (card->getDigit() == 'B') {
        wprintw(window, "The card discarded is a block card!");
        if (turn == lastTurn) {
            turn = 0;
            wprintw(window, "\n%s skips his turn.",  players[0].getName().c_str());
        }
        else {
            turn++;
            if (firstCard) {
                wprintw(window, "\n%s skips his turn.",  players[0].getName().c_str());
            }
            else {
                wprintw(window, "\n%s skips his turn.",  players[turn].getName().c_str());
            }
        }
        wrefresh(window);
    }
    // "Reverse" action
    else if (card->getDigit() == 'R') {
        wprintw(window, "The card discarded is a reverse card!");
        // Switches to opposite direction as previously
        reverse = !reverse;
        wrefresh(window);
    }
    // "Add cards" action
    else if (card->getDraw() != 'N') {
        wprintw(window, "The card discarded is a draw card!");
        if (firstCard || turn == lastTurn) {
            int num = card->getDraw() - '0';
            players[0].pickUpCards(num);
            wprintw(window, "\n%s picks up %d cards.", players[0].getName().c_str(), num);
        }
        else {
            int num = card->getDraw() - '0';
            players[turn + 1].pickUpCards(num);
            wprintw(window, "\n%s picks up %d cards.", players[turn + 1].getName().c_str(), num);
        }
        wrefresh(window);
    }

    // "Wildcard" action
    if (card->getWildCard()) {
        int newColor = 0;
        int y;
        if (firstCard) {
            wprintw(window, "The card discarded is a wildcard!");
            y = numOfPlayers + 12;
        }
        else if (card->getDraw() != 'N') {
            wprintw(window, "\nThe card discarded is a wildcard!");
            y = 3;
        }
        else {
            wprintw(window, "The card discarded is a wildcard!");
            y = 1;
        }
        mvwprintw(window, y, 0, "What color %s would you like to change to?", players[turn].getName() .c_str());
        vector<string> choices = {"Red", "Green", "Blue", "Yellow"};
        newColor = displayMenu(choices, newColor, y + 1, window);
        wrefresh(window);

        switch (newColor) {
            case 0:
                DiscardPile::setCurrentColor('R');
                break;
            case 1:
                DiscardPile::setCurrentColor('G');
                break;
            case 2:
                DiscardPile::setCurrentColor('B');
                break;
            case 3:
                DiscardPile::setCurrentColor('Y');
                break;
        }
    }
}


void nextPlayer(unsigned short& turn, bool& reverse, int& numOfPlayers) {
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

Card* discardOption(WINDOW* window, Player& currentPlayer, unsigned short playerOption, bool& winningCondition) {
    wclear(window);

    while (true) {
        try {
            char input[3];
            int i = 0;
            string discard;

            mvwprintw(window, 0, 0, "Which card do you want to discard? (Type '0' to pick up a card)\n");
            wrefresh(window);
            wclrtoeol(window);
            wgetnstr(window, input, 3);
            while (input[i] != '\0') {
                discard += toupper(input[i]);
                i++;
            }


            // Player chooses to pick up card as he cannot discard any
            if (discard == "0") {
                Card *pickedUpCard = pickUpOption(window, currentPlayer, playerOption, winningCondition);
                return pickedUpCard;
            }

            // Local card object to check whether card can actually be discarded
            Card cardToDiscard = Card(discard);
            auto acceptableCard = currentPlayer.cardMatch(&cardToDiscard);
            currentPlayer.discardCards(acceptableCard);

            // Winning condition check: Player has no more cards in his hand
            if (currentPlayer.getNumOfCards() == 0) {
                winningCondition = true;
                currentPlayer.setWinner();
                return nullptr;
            }

            // Card object created on heap to be added to Discard Pile now
            Card *discardedCard = new Card(discard);
            DiscardPile::addCard(discardedCard);
            return discardedCard;
         }
        catch (invalid_argument& error) {
            wattron(window, A_BOLD);
            wclrtobot(window);
            mvwprintw(window, 2, 0, error.what());
            wattroff(window, A_BOLD);
        }
    }
}

Card* pickUpOption(WINDOW* window, Player& currentPlayer, unsigned short playerOption, bool& winningCondition) {
    wclear(window);

    // Winning condition check: Deck runs out of cards
    if (Deck::getNumOfCards() == 0) {
        winningCondition = true;
        return nullptr;
    }

    currentPlayer.pickUpCards(1);
    Card* cardPickedUp = currentPlayer.getHand()[currentPlayer.getNumOfCards() - 1];
    mvwprintw(window, 0, 0, "You picked up a %s", cardPickedUp->getDisplayValue().c_str());
    wrefresh(window);

    // Checks to see if the card just picked up from the deck can be discarded right away
    vector<Card*>::iterator acceptableCard;
    try {
        acceptableCard = currentPlayer.cardMatch(cardPickedUp);
    }
    catch (invalid_argument& error) {
        return nullptr;
    }

    // Card just picked up can be discarded so gives option to user if he would like to
    mvwprintw(window, 2, 0, "Would you like to discard the card you just picked up?");
    vector<string> choices = {"1. Yes", "2. No"};
    playerOption = displayMenu(choices, playerOption, 3, window);
    wrefresh(window);

    if (playerOption == 0) {
        currentPlayer.discardCards(acceptableCard);
        // Winning condition check: Player has no more cards in his hand
        if (currentPlayer.getNumOfCards() == 0) {
            winningCondition = true;
            currentPlayer.setWinner();
            return nullptr;
        }
        DiscardPile::addCard(cardPickedUp);
        return cardPickedUp;
    }
    else {
        return nullptr;
    }
}