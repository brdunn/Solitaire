#ifndef ADVANCE_H
#define ADVANCE_H

#include <stdio.h>

void printPile(FILE *outFile);

void printFromTab(struct singlyLinkedListNode *tabPile, FILE *outFile);

void completeMoves(FILE *inputFile);

void wasteToFoundations();

void wasteToTableau(int destColumn);

char checkKing(char value, char isEmpty, int srcColumn, int destColumn);

void tableauToFoundation(int srcColumn);

void tableauToTableau(int srcColumn, int destColumn);

void turnCardinStock();

void resetStock();

void printMovesResults(const char* invalidMove);

void printTableauForXSwitch(FILE *outFile);

int isCardSmaller(char bottomCard, char cardToBePlaced, int srcColumn, int destColumn);

int checkIfSuitColorIsDifferent(char suitOne, char suitTwo, int srcColumn, int destColumn);

void moveError(const char* errorMessage);

struct singlyLinkedListNode* colNumToTableauPile(int colNum);

#endif