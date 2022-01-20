#include <iostream>
#include <unistd.h>
#include "readInputFile.h"
#include "termbox.h"
#include "game.hpp"
#include "singlyLinkedList.h"
#include <string>
#include <string.h>

using namespace std;

bool isSwitchFEnabled = false;
bool isSwitchSEnabled = false;
bool isSwitch1Enabled = false;
bool isSwitch3Enabled = false;
bool isSwitchLEnabled = false;

unsigned long sSwitchVal;
unsigned int lSwitchVal;

char gameFilename[100];

/**
 * current gameState
 */
extern struct gameState gameState;

/**
* rules data for this game
*/
extern struct rulesData rules;

/**
 * last move made by the player
 */
struct move lastMove;

/**
 * File from which to read the starting state
 */
FILE *inputFile;

/**
 * The nuber of times the stock has been reset
 */
int numTimesStockReset = 0;


/**
 * @author Blake Dunn
 * Terminal GUI baed implementation of Solataire.  The user can chose to provide a starting game state
 * with the -f switch or can leave it out to get a generated game.  The other switches include -s, -1,
 * -3, and -l L. The s switch is a seed to randomize the cration of a game.  the 1 and 3 switches are
 * used to sepcify turn 1 and turn 3 respectively.  The l switch is used to limit the number of times
 * a game can be reset.
 */
int main(int argc, char *argv[]) {
    int opt;
    lastMove.src, lastMove.dest, lastMove.value = 0;
    lastMove.resetStock, lastMove.flipCard = false;

    while ((opt = getopt(argc, argv, ":f:s:13l:")) != -1) {
        switch (opt) {
            case 'f':
                isSwitchFEnabled = true;
                strcpy(gameFilename, optarg);
                inputFile = fopen(gameFilename, "r");
                break;
            case 's':
                isSwitchSEnabled = true;
                sSwitchVal = atoi(optarg);
                break;
            case '1':
                isSwitch1Enabled = true;
                break;
            case '3':
                isSwitch3Enabled = true;
                break;
            case 'l':
                isSwitchLEnabled = true;
                lSwitchVal = atoi(optarg);
                break;
            case ':':
                std::cerr << "Switch needs a value\n";
                exit(-1);
                break;
            case '?':
                std::cerr << "Not a valid switch.  Available switches -f (file) -s (seed) -1 -3 -l (limit)\n";
                exit(-1);
                break;
        }
    }

    if (argc - optind > 1 || argc == 1 || (!isSwitchSEnabled && !isSwitchFEnabled)) {
        std::cerr << "Usage: ./game -f (file) -s (seed) -1 -3 -l (limit).\n";
        if (inputFile != NULL) fclose(inputFile);
        exit(-1);
    }

    if (isSwitchFEnabled) {
        readInputFile(inputFile);
    } else if (isSwitchSEnabled) {
        if (!isSwitch1Enabled && !isSwitch3Enabled) {
            std::cerr << "When using the s switch you must also use either the 1 or 3 switch." << std::endl;
            if (inputFile != NULL) fclose(inputFile);
            exit(-1);
        } else {
            propagateDeckRandomly();
        } 
    }

    initGame();
    processUserMoves();
    if (inputFile != NULL) fclose(inputFile);
    return 0;
}

/**
 * @author Blake Dunn
 * Propagates the desk with random cards based on the user defined seed value
 */
void propagateDeckRandomly() {
    //Set rules
    if (isSwitchLEnabled) {
        rules.unlimited = 0;
        rules.limitNum = lSwitchVal;
    } else {
        rules.unlimited = 1;
    }

    if (isSwitch1Enabled) {
        rules.limitNum = 1;
    } else if (isSwitch3Enabled) {
        rules.limitNum = 3;
    }

    //create deck
    card *deck = new card[52];
    int curIndex = 0;
    for (int i = 0; i < 4; i++) {
        char curSuit;
        switch (i) {
            case 0:
                curSuit = 'c';
                break;
            case 1:
                curSuit = 'd';
                break;
            case 2:
                curSuit = 'h';
                break;
            case 3:
                curSuit = 's';
                break;
        }
        for (int j = 0; j < NUM_CARDS_IN_SUIT; j++) {
            deck[curIndex].suit = curSuit;
            deck[curIndex].isCovered = 0;
            deck[curIndex].value = indexToValue(j);
            curIndex++;
        }
    }

    shuffle(deck);
    dealCards(deck);
}

/**
 * @author Blake Dunn
 * Deals cards to the gamestate from a deck
 */
void dealCards(card *deck) {
    offerWithCover(&gameState.tableauPile1, deck[0].suit, deck[0].value, 0);
    offerWithCover(&gameState.tableauPile2, deck[1].suit, deck[1].value, 1);
    offerWithCover(&gameState.tableauPile3, deck[2].suit, deck[2].value, 1);
    offerWithCover(&gameState.tableauPile4, deck[3].suit, deck[3].value, 1);
    offerWithCover(&gameState.tableauPile5, deck[4].suit, deck[4].value, 1);
    offerWithCover(&gameState.tableauPile6, deck[5].suit, deck[5].value, 1);
    offerWithCover(&gameState.tableauPile7, deck[6].suit, deck[6].value, 1);
    offerWithCover(&gameState.tableauPile2, deck[7].suit, deck[7].value, 0);
    offerWithCover(&gameState.tableauPile3, deck[8].suit, deck[8].value, 1);
    offerWithCover(&gameState.tableauPile4, deck[9].suit, deck[9].value, 1);
    offerWithCover(&gameState.tableauPile5, deck[10].suit, deck[10].value, 1);
    offerWithCover(&gameState.tableauPile6, deck[11].suit, deck[11].value, 1);
    offerWithCover(&gameState.tableauPile7, deck[12].suit, deck[12].value, 1);
    offerWithCover(&gameState.tableauPile3, deck[13].suit, deck[13].value, 0);
    offerWithCover(&gameState.tableauPile4, deck[14].suit, deck[14].value, 1);
    offerWithCover(&gameState.tableauPile5, deck[15].suit, deck[15].value, 1);
    offerWithCover(&gameState.tableauPile6, deck[16].suit, deck[16].value, 1);
    offerWithCover(&gameState.tableauPile7, deck[17].suit, deck[17].value, 1);
    offerWithCover(&gameState.tableauPile4, deck[18].suit, deck[18].value, 0);
    offerWithCover(&gameState.tableauPile5, deck[19].suit, deck[19].value, 1);
    offerWithCover(&gameState.tableauPile6, deck[20].suit, deck[20].value, 1);
    offerWithCover(&gameState.tableauPile7, deck[21].suit, deck[21].value, 1);
    offerWithCover(&gameState.tableauPile5, deck[22].suit, deck[22].value, 0);
    offerWithCover(&gameState.tableauPile6, deck[23].suit, deck[23].value, 1);
    offerWithCover(&gameState.tableauPile7, deck[24].suit, deck[24].value, 1);
    offerWithCover(&gameState.tableauPile6, deck[25].suit, deck[25].value, 0);
    offerWithCover(&gameState.tableauPile7, deck[26].suit, deck[26].value, 1);
    offerWithCover(&gameState.tableauPile7, deck[27].suit, deck[27].value, 0);

    for (int i = 28; i < 52; i++) {
        offerWithCover(&gameState.stockHead, deck[i].suit, deck[i].value, 0);
    }
}

/**
 * @author Blake Dunn
 * Shuffles a deck of cards pefectly
 */
void shuffle(card *deck) {
    for (int i = 0; i < 51; i++) {
        int j = chooseRandomlyBetween(i, 51);
        if (i != j) {
            swapCards(&deck[i], &deck[j]);
        }
    }
}

/**
 * @author Blake Dunn
 * Swaps the contents of two cards in a deck
 */
void swapCards(card *card1, card *card2) {
    char tempVal = card1->value;
    char tempSuit = card1->suit;
    card1->value = card2->value;
    card1->suit = card2->suit;
    card2->value = tempVal;
    card2->suit = tempSuit;
}

/**
 * @author Blake Dunn
 * Returns real values randomly and uniformly between 0 and 1
 */
long chooseRandomlyBetween(long a, long b) {
    return a + (long) ((b - a + 1) * randomNum());
}

/**
 * @author Blake Dunn
 * Returns a random number based off of a seed value
 */
double randomNum() {
    unsigned long seed = sSwitchVal;
    const unsigned long MODULUS = 2147483647;
    const unsigned long MULTIPLIER = 48271;
    const unsigned long Q = MODULUS / MULTIPLIER;
    const unsigned long R = MODULUS % MULTIPLIER;
    unsigned long t1 = MULTIPLIER * (seed % Q);
    unsigned long t2 = R * (seed / Q);

    if (t1 > t2) {
        seed = t1 - t2;
    } else {
        seed = t1 + (MODULUS - t2);
    }

    return ((double) seed / MODULUS);
}

/**
 * @author Blake Dunn
 * Begins the game
 */
void initGame() {
    tb_init();
    tb_set_clear_attributes(TB_BOLD, TB_BOLD);
    drawFullGame();
}

/**
 * @author Blake Dunn
 * Draws the entire game
 */
void drawFullGame() {
    if (gameHasBeenWon()) {
        gameWonScreen();
        if (inputFile != NULL) fclose(inputFile);
        exit(-1);
    }
    tb_clear();
    displayBackdrop();
    displayInstructions();
    displayFoundations();
    displayWaste();
    displayTableau();
    tb_present();
}

/**
 * @author Eva Kohl
 * Gets user inputs and does the appropriate moves
 */
void processUserMoves() {
    char source = 0;
    for (;;) {
        struct tb_event event;
        tb_poll_event(&event);

        switch (event.ch) {
            case 'Q':
                tb_shutdown();
                return;
            case 'R':
                //restart game
                restartGame();
                drawFullGame();
                break;
            case 'S':
                //Save to file
                saveStateToFile();
                break;
            case 'U':
                undoLastMove();
                drawFullGame();
                break;
            case '.':
                //next card
                turnCardinStock();
                drawFullGame();
                break;
            case '>':
                resetStock();
                drawFullGame();
                break;
            case 'W':
                if (source) {
                    completeMove(source, 'w', false);
                    source = 0;
                    drawFullGame();
                } else {
                    source = 'w';
                }
                break;
            case 'F':
                if (source) {
                    completeMove(source, 'f', false);
                    source = 0;
                    drawFullGame();
                } else {
                    source = 'f';
                }
                break;
            case '1':
                if (source) {
                    completeMove(source, '1', false);
                    source = 0;
                    drawFullGame();
                } else {
                    source = '1';
                }
                break;
            case '2':
                if (source) {
                    completeMove(source, '2', false);
                    source = 0;
                    drawFullGame();
                } else {
                    source = '2';
                }
                break;
            case '3':
                if (source) {
                    completeMove(source, '3', false);
                    source = 0;
                    drawFullGame();
                } else {
                    source = '3';
                }
                break;
            case '4':
                if (source) {
                    completeMove(source, '4', false);
                    source = 0;
                    drawFullGame();
                } else {
                    source = '4';
                }
                break;
            case '5':
                if (source) {
                    completeMove(source, '5', false);
                    source = 0;
                    drawFullGame();
                } else {
                    source = '5';
                }
                break;
            case '6':
                if (source) {
                    completeMove(source, '6', false);
                    source = 0;
                    drawFullGame();
                } else {
                    source = '6';
                }
                break;
            case '7':
                if (source) {
                    completeMove(source, '7', false);
                    source = 0;
                    drawFullGame();
                } else {
                    source = '7';
                }
                break;
        }
    }
}

/**
 * @author Eva Kohl
 * this will undo whatever was just done
 */
void undoLastMove() {
    if (lastMove.flipCard) {
        unFlipCard();
        setLastMove(false, false, 0, 0, 0);
    } else if (lastMove.resetStock) {
        unResetStock();
        setLastMove(false, false, 0, 0, 0);
    } else if (lastMove.src != 0 && lastMove.dest != 0) {
        completeMove(lastMove.dest, lastMove.src, true);
        setLastMove(false, false, 0, 0, 0);
    }
}

/**
 * @auhtor Blake Dunn
 * Restarts the game to the original state
 */
void restartGame() {
    destory(&gameState.foundationClubsHead);
    destory(&gameState.foundationHeartsHead);
    destory(&gameState.foundationSpadesHead);
    destory(&gameState.foundationDiamondsHead);

    destory(&gameState.tableauPile1);
    destory(&gameState.tableauPile2);
    destory(&gameState.tableauPile3);
    destory(&gameState.tableauPile4);
    destory(&gameState.tableauPile5);
    destory(&gameState.tableauPile6);
    destory(&gameState.tableauPile7);

    destory(&gameState.wasteHead);
    destory(&gameState.stockHead);

    if (isSwitchFEnabled) {
        fclose(inputFile);
        inputFile = fopen(gameFilename, "r");
        resetReadInputFile();
        readInputFile(inputFile);
    } else if (isSwitchSEnabled) {
        propagateDeckRandomly();
    }

    numTimesStockReset = 0;
}

/**
 * @author Eva Kohl
 * This method has been made to let the user know they have won and that they can feel good about themselves.:)
 */
void gameWonScreen() {
    tb_clear();
    struct tb_cell cell;

    cell.fg = TB_WHITE;
    cell.bg = TB_WHITE;
    cell.ch = '\0';

    for (int i = 0; i < 90; i++) {
        for (int j = 0; j < 90; j++) {
            tb_put_cell(i, j, &cell);
            tb_put_cell(i, j, &cell);
        }
    }

    displayString("Game Won!", 39, 7, TB_BLUE, TB_BOLD);
    tb_present();

    for (long long i = 0; i < 1000000000; i++);
    tb_shutdown();
}

/**
 * @author Eva Kohl
 * Checks to see if the game has been won
 * @return bool - true is te game has been won otherwise false
 */
bool gameHasBeenWon() {
    if (checkKing(peekLast(&gameState.foundationClubsHead)->data.value, 1, 0, 0) &&
        checkKing(peekLast(&gameState.foundationDiamondsHead)->data.value, 1, 0, 0) &&
        checkKing(peekLast(&gameState.foundationHeartsHead)->data.value, 1, 0, 0) &&
        checkKing(peekLast(&gameState.foundationSpadesHead)->data.value, 1, 0, 0)) {
        return true;
    }
    return false;
}

/**
 * @author Eva Kohl
 * This method, given the source and the destination, then completes one move
 */
void completeMove(char scr, char dest, bool calledFromUndoMove) {
    int source = scr - '0';
    int destinaion = dest - '0';
    switch (scr) {
        case 'w':
            switch (dest) {
                case 'f':

                    wasteToFoundations();
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                    wasteToTableau(destinaion);
                    break;
                default: {
                }
            }
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
            if (!calledFromUndoMove) {
                switch (dest) {
                    case 'f':
                        tableauToFoundation(source);
                        break;
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                        tableauToTableau(source, destinaion);
                        break;
                }
            } else {
                switch (dest) {
                    case 'w':
                        tableauToWasteForUndo(source);
                        break;
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                        tableauToTableauForUndo(source, destinaion);
                        break;
                }
            }
            break;
        case 'c':
        case 'd':
        case 's':
        case 'h':
            switch (dest) {
                case 'w':
                    foundationToWaste(scr);
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                    foundationToTableau(scr, destinaion);
                    break;
            }
            break;
    }
}

/**
 * @author Eva Kohl
 * This method is to be called by the undo move. It moves on card from
 * the tableau to the waste.
 */
void tableauToWasteForUndo(int scr) {
    struct singlyLinkedListNode *source = colNumToTableauPile(scr);
    offerWithReference(&gameState.wasteHead, poll(source));
}

/**
 * @author Eva Kohl
 * 
 */
void tableauToTableauForUndo(int src, int dest) {
    struct singlyLinkedListNode *source = colNumToTableauPile(src);
    struct singlyLinkedListNode *destination = colNumToTableauPile(dest);
    struct singlyLinkedListNode *tmp = peekFirstUncovered(source);

    if (isEmpty(destination)) {
        offerWithReference(destination, pollFromMiddle(source, tmp));
        setLastMove(false, false, 0, 0, 0);
        return;
    } else {
        while (tmp) {
            if (lastMove.value == tmp->data.value) {
                if (size(peekFirstUncovered(destination)) < 1)peekFirstUncovered(destination)->data.isCovered = 1;
                offerWithReference(destination, pollFromMiddle(source, tmp));
                peekLast(source)->data.isCovered = 0;
                setLastMove(false, false, 0, 0, 0);
                return;
            } else {
                tmp = tmp->next;
            }
        }
    }
}

/**
 * @author Eva Kohl
 * This is to transfer any card from the foundations to the tableau
 * This method should only be called when using the undo button
 */
void foundationToTableau(char scr, int dest) {
    struct singlyLinkedListNode *destination = colNumToTableauPile(dest);
    switch (scr) {
        case 'c':
            offerWithReference(destination, poll(&gameState.foundationClubsHead));
            break;
        case 'd':
            offerWithReference(destination, poll(&gameState.foundationDiamondsHead));
            break;
        case 's':
            offerWithReference(destination, poll(&gameState.foundationSpadesHead));
            break;
        case 'h':
            offerWithReference(destination, poll(&gameState.foundationHeartsHead));
            break;
    }
}

/**
 * @author Eva Kohl
 * This is to transfer any card from the foundations to the waste
 * This method should only be called when using the undo button
 */
void foundationToWaste(char scr) {
    struct singlyLinkedListNode *destination = &gameState.wasteHead;
    switch (scr) {
        case 'c':
            offerWithReference(destination, poll(&gameState.foundationClubsHead));
            break;
        case 'd':
            offerWithReference(destination, poll(&gameState.foundationDiamondsHead));
            break;
        case 's':
            offerWithReference(destination, poll(&gameState.foundationSpadesHead));
            break;
        case 'h':
            offerWithReference(destination, poll(&gameState.foundationHeartsHead));
            break;
    }
}

/**
 * @author Eva Kohl
 * This will unset reset stock, this method should only be called
 * by undo move method
 */
void unResetStock() {
    while (!isEmpty(&gameState.stockHead)) offerWithReference(&gameState.wasteHead, poll(&gameState.stockHead));
    numTimesStockReset--;
}

/**
 * @author Eva Kohl
 * This method un-filps a card in the stock pile
 */
void unFlipCard() {
    if (isEmpty(&gameState.wasteHead)) return;
    if (isSwitch1Enabled || rules.cardsPerTurn == 1) {
        offerWithReference(&gameState.stockHead, poll(&gameState.wasteHead));
    } else if (isSwitch3Enabled || rules.cardsPerTurn == 3) {
        for (int i = 0; i < 3; i++) {
            if (!isEmpty(&gameState.wasteHead)) {
                offerWithReference(&gameState.stockHead, poll(&gameState.wasteHead));
            }
        }
    }
}

/**
 * @author Eva Kohl
 * This method resets the backdrop of the whole game
 */
void displayBackdrop() {
    struct tb_cell cell;
    cell.fg = TB_BOLD;
    cell.bg = TB_BOLD;
    cell.ch = '\0';

    for (int i = 0; i < 90; i++) {
        for (int j = 0; j < 40; j++) {
            tb_put_cell(i, j, &cell);
            tb_put_cell(i, j, &cell);
        }
    }
}

/**
 * @author Blake Dunn
 * displays the instructions
 */
void displayInstructions() {
    struct tb_cell cell;
    cell.fg = TB_DEFAULT;
    cell.bg = TB_GREEN;
    cell.ch = '\0';

    for (int i = 0; i < 35; i++) {
        tb_put_cell(i, 0, &cell);
        tb_put_cell(i, 9, &cell);
    }

    for (int i = 1; i < 9; i++) {
        tb_put_cell(0, i, &cell);
        tb_put_cell(34, i, &cell);
    }

    if (isSwitchFEnabled) {
        std:
        string fileName(gameFilename);
        displayString("File: " + fileName, 8, 0, TB_GREEN, TB_BLACK);
    } else if (isSwitchSEnabled) {
        displayString("Seed: " + std::to_string(sSwitchVal), 8, 0, TB_GREEN, TB_BLACK);
    }

    cell.bg = TB_MAGENTA;
    cell.ch = 'Q';
    tb_put_cell(2, 1, &cell);

    cell.ch = 'R';
    tb_put_cell(2, 2, &cell);

    cell.ch = '.';
    tb_put_cell(2, 3, &cell);

    cell.ch = 'S';
    tb_put_cell(17, 1, &cell);

    cell.ch = 'U';
    tb_put_cell(17, 2, &cell);

    cell.ch = '>';
    tb_put_cell(17, 3, &cell);

    displayString(": quit", 3, 1);
    displayString(": restart", 3, 2);
    displayString(": next card", 3, 3);
    displayString(": save to file", 18, 1);
    displayString(": undo move", 18, 2);
    displayString(": reset stock", 18, 3);

    displayString("To move, press the source and", 2, 6);
    displayString("then destination (one of W, F,", 2, 7);
    displayString("1, 2, 3, 4, 5, 6, 7)", 2, 8);
}

/**
 * @author Blake Dunn
 * Displays the contents of the foundations
 */
void displayFoundations() {
    struct tb_cell cell;
    cell.fg = TB_BLACK;
    cell.bg = TB_MAGENTA;
    cell.ch = 'F';
    tb_put_cell(11, 11, &cell);
    displayString("oundations", 12, 11);

    if (isEmpty(&gameState.foundationClubsHead)) {
        cell.bg = TB_GREEN;
        cell.ch = suitToUnicode('c');
        for (int y = 13; y < 17; y++) {
            for (int x = 5; x < 9; x++) {
                tb_put_cell(x, y, &cell);
            }
        }
    } else {
        displayCard('c', peekLast(&gameState.foundationClubsHead)->data.value, 5, 13);
    }

    if (isEmpty(&gameState.foundationDiamondsHead)) {
        cell.bg = TB_GREEN;
        cell.ch = suitToUnicode('d');
        for (int y = 13; y < 17; y++) {
            for (int x = 11; x < 15; x++) {
                tb_put_cell(x, y, &cell);
            }
        }
    } else {
        displayCard('d', peekLast(&gameState.foundationDiamondsHead)->data.value, 11, 13);
    }

    if (isEmpty(&gameState.foundationHeartsHead)) {
        cell.bg = TB_GREEN;
        cell.ch = suitToUnicode('h');
        for (int y = 13; y < 17; y++) {
            for (int x = 17; x < 21; x++) {
                tb_put_cell(x, y, &cell);
            }
        }
    } else {
        displayCard('h', peekLast(&gameState.foundationHeartsHead)->data.value, 17, 13);
    }

    if (isEmpty(&gameState.foundationSpadesHead)) {
        cell.bg = TB_GREEN;
        cell.ch = suitToUnicode('s');
        for (int y = 13; y < 17; y++) {
            for (int x = 23; x < 27; x++) {
                tb_put_cell(x, y, &cell);
            }
        }
    } else {
        displayCard('s', peekLast(&gameState.foundationSpadesHead)->data.value, 23, 13);
    }
}

/**
 * @author Blake Dunn and Eva Kohl
 * Adds the waste to the termbox bufer
 */
void displayWaste() {
    struct tb_cell cell;
    cell.fg = TB_BLACK;
    cell.bg = TB_MAGENTA;
    cell.ch = 'W';
    tb_put_cell(4, 18, &cell);
    displayString("aste", 5, 18);

    if (!rules.unlimited) {
        displayString('(' + std::to_string(rules.limitNum - numTimesStockReset) + " resets remaining)", 12, 18);
    }

    //waste
    if (size(&gameState.wasteHead) >= 3) {
        struct card cardToDisplay = peekAtIndex(&gameState.wasteHead, size(&gameState.wasteHead) - 3)->data;
        displayCard(cardToDisplay.suit, cardToDisplay.value, 13, 20);
    }

    if (size(&gameState.wasteHead) >= 2) {
        struct card cardToDisplay = peekAtIndex(&gameState.wasteHead, size(&gameState.wasteHead) - 2)->data;
        displayCard(cardToDisplay.suit, cardToDisplay.value, 14, 20);
    }

    if (size(&gameState.wasteHead) >= 1) {
        struct card cardToDisplay = peekLast(&gameState.wasteHead)->data;
        displayCard(cardToDisplay.suit, cardToDisplay.value, 15, 20);
    }

    //stock
    cell.fg = TB_BLACK;
    cell.bg = TB_CYAN;
    cell.ch = '#';
    if (!isEmpty(&gameState.stockHead)) {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                tb_put_cell(21 + x, 20 + y, &cell);
            }
        }
    }

    cell.fg = TB_WHITE;
    cell.bg = TB_BLACK;
    cell.ch = size(&gameState.stockHead) / 10 + '0';
    tb_put_cell(22, 24, &cell);
    cell.ch = size(&gameState.stockHead) % 10 + '0';
    tb_put_cell(23, 24, &cell);
}

/**
 * @author Blake Dunn
 * Displays the current state of the tableau
 */
void displayTableau() {
    struct tb_cell cell;
    cell.fg = TB_BLACK;
    cell.bg = TB_MAGENTA;

    //headers
    for (int i = 1; i < 8; i++) {
        cell.ch = i + '0';
        tb_put_cell(34 + 6 * i + 1, 0, &cell);
    }

    //cards
    for (int i = 1; i < 8; i++) {
        int curY = 1;
        struct singlyLinkedListNode *cur = colNumToTableauPile(gameState, i)->next;
        while (cur) {
            if (cur->data.isCovered) {
                cell.bg = TB_CYAN;
                cell.ch = '#';
                for (int coverdX = 0; coverdX < 4; coverdX++) {
                    tb_put_cell(34 + 6 * i + coverdX, curY, &cell);
                }
                curY++;
            } else {
                displayCard(cur->data.suit, cur->data.value, 34 + 6 * i, curY++);
            }
            cur = cur->next;
        }
    }
}

/**
 * @author Blake Dunn
 * Displays a string starting at location x, y with specified text on a specified background
 * This function does not provide wrapping into a new line
 */
void displayString(std::string toDisplay, int startX, int startY, uint16_t bg, uint16_t fg) {
    struct tb_cell cell;
    cell.fg = fg;
    cell.bg = bg;

    for (int i = 0; i < toDisplay.size(); i++) {
        cell.ch = toDisplay[i];
        tb_put_cell(startX + i, startY, &cell);
    }
}

/**
 * @author Blake Dunn
 * Displays a string starting at location x, y with white text on a black background.
 * This function does not provide wrapping into a new line
 */
void displayString(std::string toDisplay, int startX, int startY) {
    displayString(toDisplay, startX, startY, TB_BLACK, TB_WHITE);
}

/**
 * @author Blake Dunn
 * displays a card with the given suit and value at the startX and startY position
 */
void displayCard(char suit, char value, int startX, int startY) {
    struct tb_cell cell;
    cell.bg = TB_WHITE;
    if (suit == 'c' || suit == 's') {
        cell.fg = TB_BLACK;
    } else {
        cell.fg = TB_RED;
    }

    cell.ch = '\0';
    for (int y = startY; y < startY + 4; y++) {
        for (int x = startX; x < startX + 4; x++) {
            tb_put_cell(x, y, &cell);
        }
    }

    //value
    cell.ch = value;
    tb_put_cell(startX, startY, &cell);
    tb_put_cell(startX + 3, startY + 3, &cell);

    //suit
    cell.ch = suitToUnicode(suit);
    tb_put_cell(startX + 1, startY, &cell);
    tb_put_cell(startX + 2, startY + 3, &cell);
}

/**
 * @author Blake Dunn
 * Converts a suit char into the unicode character for the suit
 */
uint32_t suitToUnicode(char value) {
    uint32_t retVal;
    switch (value) {
        case 'c':
            return 0x2663;
            break;
        case 'd':
            return 0x2666;
            break;
        case 'h':
            return 0x2665;
            break;
        case 's':
            return 0x2660;
            break;
    }
    return '0';
}

/**
 * @author Eva Kol
 * This is a helper method to update the last move member variable
 */
void setLastMove(bool flipCard, bool resetStock, char src, char dest, char value) {
    lastMove.flipCard = flipCard;
    lastMove.resetStock = resetStock;
    lastMove.dest = dest;
    lastMove.src = src;
    lastMove.value = value;
}

/**
 * @author Blake Dunn
 * converts a column number colNum into a a tableau pile
 */
struct singlyLinkedListNode *colNumToTableauPile(struct gameState &curGameState, int colNum) {
    switch (colNum) {
        case 1:
            return &curGameState.tableauPile1;
            break;
        case 2:
            return &curGameState.tableauPile2;
            break;
        case 3:
            return &curGameState.tableauPile3;
            break;
        case 4:
            return &curGameState.tableauPile4;
            break;
        case 5:
            return &curGameState.tableauPile5;
            break;
        case 6:
            return &curGameState.tableauPile6;
            break;
        case 7:
            return &curGameState.tableauPile7;
            break;
        default:
            return NULL;
    }
}

/**
 * @author Blake Dunn
 * Moving the top waste card to the foundations. If the top waste card is an Ace, it may be
 * moved to the appropriate (empty) foundation. Otherwise, the top waste card may be placed
 * on top of the card with one lower rank, in the same suit, in the foundations.
 */
void wasteToFoundations() {
    switch (peekLast(&gameState.wasteHead)->data.suit) {
        case 'c':
            if (valueToIndex(peekLast(&gameState.foundationClubsHead)->data.value) + 1 ==
                valueToIndex(peekLast(&gameState.wasteHead)->data.value) ||
                isEmpty(&gameState.foundationClubsHead) &&
                valueToIndex(peekLast(&gameState.wasteHead)->data.value) == 0) {
                offerWithReference(&gameState.foundationClubsHead, poll(&gameState.wasteHead));
                setLastMove(false, false, 'w', 'c', 0);
            } else {
                return;
            }
            break;
        case 'd':
            if (valueToIndex(peekLast(&gameState.foundationDiamondsHead)->data.value) + 1 ==
                valueToIndex(peekLast(&gameState.wasteHead)->data.value) ||
                isEmpty(&gameState.foundationDiamondsHead) &&
                valueToIndex(peekLast(&gameState.wasteHead)->data.value) == 0) {
                offerWithReference(&gameState.foundationDiamondsHead, poll(&gameState.wasteHead));
                setLastMove(false, false, 'w', 'd', 0);
            } else {
                return;
            }
            break;
        case 'h':
            if (valueToIndex(peekLast(&gameState.foundationHeartsHead)->data.value) + 1 ==
                valueToIndex(peekLast(&gameState.wasteHead)->data.value) ||
                isEmpty(&gameState.foundationHeartsHead) &&
                valueToIndex(peekLast(&gameState.wasteHead)->data.value) == 0) {
                offerWithReference(&gameState.foundationHeartsHead, poll(&gameState.wasteHead));
                setLastMove(false, false, 'w', 'h', 0);
            } else {
                return;
            }
            break;
        case 's':
            if (valueToIndex(peekLast(&gameState.foundationSpadesHead)->data.value) + 1 ==
                valueToIndex(peekLast(&gameState.wasteHead)->data.value) ||
                isEmpty(&gameState.foundationSpadesHead) &&
                valueToIndex(peekLast(&gameState.wasteHead)->data.value) == 0) {
                offerWithReference(&gameState.foundationSpadesHead, poll(&gameState.wasteHead));
                setLastMove(false, false, 'w', 's', 0);
            } else {
                return;
            }
            break;
    }
}

/**
 * @author Eva Kohl
 * checks if a card is one lower then it's card to be placed on
 */
int isCardSmaller(char bottomCard, char cardToBePlaced, int srcColumn, int destColumn) {
    if (valueToIndex(bottomCard) != (valueToIndex(cardToBePlaced) + 1)) {
        return 0;
    }
    return 1;
}

/**
 * @author Eva Kohl
 * checks if two suits are of the opposite color
 */
int checkIfSuitColorIsDifferent(char suitOne, char suitTwo, int srcColumn, int destColumn) {
    if (suitOne == 'h' || suitOne == 'd') {
        if (!(suitTwo == 's' || suitTwo == 'c')) {
            return 0;
        }
    } else {
        if (!(suitTwo == 'h' || suitTwo == 'd')) {
            return 0;
        }
    }
    return 1;
}

/**
 * @author Eva Kohl Blake Dunn
 * returns 1 if valid king placement, 0 otherwise
 */
char checkKing(char value, char isEmpty, int srcColumn, int destColumn) {
    if (value == 'K' && isEmpty) {
        return 1;
    } else if (isEmpty) {
        return 0;
    }
    return 0;
}

/**
 * @author Eva Kohl
 * Moving the top waste card to the tableau. If the top waste card is a King, it may be moved
 * to an empty tableau column. Otherwise, the top waste card may be placed onto the bottom
 * card in a tableau column, if the waste card has rank one lower than, and the opposite color
 * of, the bottom card.
 */
void wasteToTableau(int destColumn) {
    struct singlyLinkedListNode *dest = colNumToTableauPile(destColumn);
    char canPlaceKing = checkKing(peekLast(&gameState.wasteHead)->data.value, isEmpty(dest), -1, destColumn);
    if (canPlaceKing) {
        offerWithReference(dest, poll(&gameState.wasteHead));
        setLastMove(false, false, 'w', destColumn + '0', 0);
    } else if (checkIfSuitColorIsDifferent(peekLast(dest)->data.suit, peekLast(&gameState.wasteHead)->data.suit, 'w',
            destColumn) && isCardSmaller(peekLast(dest)->data.value, peekLast(&gameState.wasteHead)->data.value, 'w', destColumn)) {
        if (!isEmpty(&gameState.wasteHead)) {
            offerWithReference(dest, poll(&gameState.wasteHead));
            setLastMove(false, false, 'w', destColumn + '0', 0);
        }
    }
}

/**
 * @author Eva Kohl
 * Moving a tableau card to the foundations. If the bottom card of a tableau column is an Ace,
 * it may be moved to the appropriate (empty) foundation. Otherwise, the bottom card of a tableau
 * column may be placed on top of the card with one lower rank, in the same suit, in the foundations.
 */
void tableauToFoundation(int srcColumn) {
    struct singlyLinkedListNode *src = colNumToTableauPile(srcColumn);
    switch (peekLast(src)->data.suit) {
        case 'c':
            if (valueToIndex(peekLast(&gameState.foundationClubsHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&gameState.foundationClubsHead) && valueToIndex(peekLast(src)->data.value) == 0) {

                offerWithReference(&gameState.foundationClubsHead, poll(src));
                peekLast(src)->data.isCovered = 0;
                setLastMove(false, false, srcColumn + '0', 'c', 0);
            } else {
                return;
            }
            break;
        case 'd':
            if (valueToIndex(peekLast(&gameState.foundationDiamondsHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&gameState.foundationDiamondsHead) && valueToIndex(peekLast(src)->data.value) == 0) {
                offerWithReference(&gameState.foundationDiamondsHead, poll(src));
                peekLast(src)->data.isCovered = 0;
                setLastMove(false, false, srcColumn + '0', 'd', 0);
            } else {
                return;
            }
            break;
        case 'h':
            if (valueToIndex(peekLast(&gameState.foundationHeartsHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&gameState.foundationHeartsHead) && valueToIndex(peekLast(src)->data.value) == 0) {
                offerWithReference(&gameState.foundationHeartsHead, poll(src));
                peekLast(src)->data.isCovered = 0;
                setLastMove(false, false, srcColumn + '0', 'h', 0);
            } else {
                return;
            }
            break;
        case 's':
            if (valueToIndex(peekLast(&gameState.foundationSpadesHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&gameState.foundationSpadesHead) && valueToIndex(peekLast(src)->data.value) == 0) {
                offerWithReference(&gameState.foundationSpadesHead, poll(src));
                peekLast(src)->data.isCovered = 0;
                setLastMove(false, false, srcColumn + '0', 's', 0);
            } else {
                return;
            }
            break;
    }
}

/**
 * @author Eva Kohl
 * Moving tableau cards from one column to another. Any pile of cards, starting from any uncovered card,
 * and including all cards down to the bottom of the column, may be moved from one tableau column to
 * another column, if the top card in the pile could be moved to the destination tableau column as a
 * waste card.
 */
void tableauToTableau(int srcColumn, int destColumn) {
    struct singlyLinkedListNode *src = colNumToTableauPile(srcColumn);
    struct singlyLinkedListNode *dest = colNumToTableauPile(destColumn);
    if (isEmpty(src)) {
        return;
    }

    struct singlyLinkedListNode *tmp = peekFirstUncovered(src);
    char canPlaceKing = checkKing(tmp->data.value, isEmpty(dest), srcColumn, destColumn);

    if (canPlaceKing) {
        offerWithReference(dest, pollFromMiddle(src, tmp));
        peekLast(src)->data.isCovered = 0;
        setLastMove(false, false, srcColumn + '0', destColumn + '0', tmp->data.value);
        return;
    } else {
        while (tmp) {
            if (isCardSmaller(peekLast(dest)->data.value, tmp->data.value, srcColumn + '0', destColumn) &&
                checkIfSuitColorIsDifferent(peekLast(dest)->data.suit, tmp->data.suit, srcColumn + '0', destColumn)) {
                offerWithReference(dest, pollFromMiddle(src, tmp));
                peekLast(src)->data.isCovered = 0;
                setLastMove(false, false, srcColumn + '0', destColumn + '0', tmp->data.value);
                return;
            } else {
                tmp = tmp->next;
            }
        }
    }
}

/**
 * @author Blake Dunn
 * Turning over the next cards. If there are cards remaining in the stock, we may turn over (either
 * 1 or 3, depending on the rules) stock cards and place them on top of the waste pile.
 */
void turnCardinStock() {
    if (isEmpty(&gameState.stockHead))return;

    if (isSwitch1Enabled || rules.cardsPerTurn == 1) {
        offerWithReference(&gameState.wasteHead, poll(&gameState.stockHead));
        setLastMove(true, false, 0, 0, 0);
    } else if (isSwitch3Enabled || rules.cardsPerTurn == 3) {
        for (int i = 0; i < 3; i++) {
            if (!isEmpty(&gameState.stockHead)) {
                offerWithReference(&gameState.wasteHead, poll(&gameState.stockHead));
            }
        }
        setLastMove(true, false, 0, 0, 0);
    }
}

/**
 * @author Blake Dunn
 * Resetting the stock pile. If there are no cards remaining in the stock, we may turn over the waste
 * pile and use it as the stock again. The first card added to the waste pile (on the bottom) becomes the
 * first card of the stock pile, and the last card placed on the waste pile becomes the last card of the stock
 * pile (i.e., we go through the stock again, in the same order, except for any cards that were moved).
 * Different variations of Solitaire have different rules about the number of stock resets allowed
 */
void resetStock() {
    if (!rules.unlimited && numTimesStockReset >= rules.limitNum) return;
    if (gameState.stockHead.next) return;

    while (!isEmpty(&gameState.wasteHead)) offerWithReference(&gameState.stockHead, poll(&gameState.wasteHead));
    numTimesStockReset++;
    setLastMove(false, true, 0, 0, 0);
}

/**
* @author Blake Dunn
 *@author Eva Kohl
* prints off the results of processing the moves section
*/
void saveStateToFile() {
    FILE *outFile = fopen("solataire-save-game.txt", "w");

    fprintf(outFile, "RULES: \n\tturn %d\n", rules.cardsPerTurn);
    if (rules.unlimited) {
        fprintf(outFile, "\tunlimited\n");
    } else {
        fprintf(outFile, "\tlimit %d\n", rules.limitNum);
    }

    fprintf(outFile, "FOUNDATIONS:\n");
    fprintf(outFile, "\t%c%c\n",
            (gameState.foundationClubsHead.next) ? peekLast(&gameState.foundationClubsHead)->data.value : '_', 'c');
    fprintf(outFile, "\t%c%c\n",
            (gameState.foundationDiamondsHead.next) ? peekLast(&gameState.foundationDiamondsHead)->data.value : '_',
            'd');
    fprintf(outFile, "\t%c%c\n",
            (gameState.foundationHeartsHead.next) ? peekLast(&gameState.foundationHeartsHead)->data.value : '_', 'h');
    fprintf(outFile, "\t%c%c\n",
            (gameState.foundationSpadesHead.next) ? peekLast(&gameState.foundationSpadesHead)->data.value : '_', 's');

    fprintf(outFile, "TABLEAU:\n");
    printTableau(outFile);

    fprintf(outFile, "STOCK: \n\t");

    for (int i = 0; i < size(&gameState.wasteHead); i++) {
        struct card cardToPrint = peekAtIndex(&gameState.wasteHead, i)->data;
        fprintf(outFile, "%c%c ", cardToPrint.value, cardToPrint.suit);
    }

    fprintf(outFile, "| ");

    for (int i = size(&gameState.stockHead) - 1; i >= 0; i--) {
        struct card cardToPrint = peekAtIndex(&gameState.stockHead, i)->data;
        fprintf(outFile, "%c%c ", cardToPrint.value, cardToPrint.suit);
    }

    fprintf(outFile, "\nMOVES:\n");
    fclose(outFile);
}

/**
 * @author Eva Kohl Blake Dunn
 * prints the tableau
 */
void printTableau(FILE *outFile) {
    for (int i = 7; i > 0; i--) {
        struct singlyLinkedListNode *tabPile = colNumToTableauPile(i);
        int isCovered = 1;
        fprintf(outFile, "\t");
        for (int i = 0; i < size(tabPile); i++) {
            struct card cardToDisplay = peekAtIndex(tabPile, i)->data;

            if (!cardToDisplay.isCovered && isCovered) {
                fprintf(outFile, "| ");
                isCovered = 0;
            }
            fprintf(outFile, "%c%c ", cardToDisplay.value, cardToDisplay.suit);

        }
        if (isCovered) {
            fprintf(outFile, "| ");
        }

        fprintf(outFile, "\n");
    }
}

/**
 * @author Blake Dunn
 * converts a column number colNum into a a tableau pile
 */
struct singlyLinkedListNode *colNumToTableauPile(int colNum) {
    switch (colNum) {
        case 1:
            return &gameState.tableauPile1;
            break;
        case 2:
            return &gameState.tableauPile2;
            break;
        case 3:
            return &gameState.tableauPile3;
            break;
        case 4:
            return &gameState.tableauPile4;
            break;
        case 5:
            return &gameState.tableauPile5;
            break;
        case 6:
            return &gameState.tableauPile6;
            break;
        case 7:
            return &gameState.tableauPile7;
            break;
        default:
            return NULL;
    }
}
