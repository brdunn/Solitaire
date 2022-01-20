#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include "singlyLinkedList.h"

/**
 * moves made by the player
 */
struct move{
    bool flipCard;
    bool resetStock;
    char value;
    char src;
    char dest;
};

void propagateDeckRandomly();
void dealCards(card* deck);
void shuffle(card* deck);
void swapCards(card* card1, card* card2);
long chooseRandomlyBetween(long a, long b);
double randomNum();
void initGame();
void drawFullGame();
void processUserMoves();
void undoLastMove();
void restartGame();
void gameWonScreen();
bool gameHasBeenWon();
void completeMove(char scr, char dest, bool calledFromUndoMove);
void tableauToWasteForUndo(int scr);
void tableauToTableauForUndo(int scr, int dest);
void foundationToTableau(char scr, int dest);
void foundationToWaste(char scr);
void unResetStock();
void unFlipCard();
void displayBackdrop();
void displayInstructions();
void displayFoundations();
void displayWaste();
void displayTableau();
void displayString(std::string toDisplay, int startX, int startY, uint16_t bg, uint16_t fg);
void displayString(std::string toDisplay, int startX, int startY);
void displayCard(char suit, char value, int startX, int startY);
uint32_t suitToUnicode(char value);
void setLastMove(bool flipCard, bool resetStock, char src, char dest, char value);
struct singlyLinkedListNode* colNumToTableauPile(struct gameState& curGameState, int colNum);
void wasteToFoundations();
int isCardSmaller(char bottomCard, char cardToBePlaced, int srcColumn, int destColumn);
int checkIfSuitColorIsDifferent(char suitOne, char suitTwo, int srcColumn, int destColumn);
char checkKing(char value, char isEmpty, int srcColumn, int destColumn);
void wasteToTableau(int destColumn);
void tableauToFoundation(int srcColumn);
void tableauToTableau(int srcColumn, int destColumn);
void turnCardinStock();
void resetStock();
void saveStateToFile();
void printPile(FILE *outFile);
void printTableau(FILE *outFile);
struct singlyLinkedListNode* colNumToTableauPile(int colNum);

#endif
