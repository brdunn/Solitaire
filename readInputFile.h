#ifndef READINPUTFILE_H
#define READINPUTFILE_H

#include <stdio.h>
#include "singlyLinkedList.h"

#define NUM_CARDS_IN_SUIT 13


struct rulesData {
    int cardsPerTurn;
    //unlimited 1, limited 0
    char unlimited;
    int limitNum;
};

struct stockData {
    int wasteCard;
    int stockCard;
};


struct deck {
    //A 2 3 4 5 6 7 8 9 T  J   Q   K
    //0 1 2 3 4 5 6 7 8 9  10  11  12 
    struct card clubs[NUM_CARDS_IN_SUIT];
    struct card diamonds[NUM_CARDS_IN_SUIT];
    struct card hearts[NUM_CARDS_IN_SUIT];
    struct card spades[NUM_CARDS_IN_SUIT];
};

struct gameState {
    //dumby head nodes for each tableau column
    struct singlyLinkedListNode tableauPile1;
    struct singlyLinkedListNode tableauPile2;
    struct singlyLinkedListNode tableauPile3;
    struct singlyLinkedListNode tableauPile4;
    struct singlyLinkedListNode tableauPile5;
    struct singlyLinkedListNode tableauPile6;
    struct singlyLinkedListNode tableauPile7;

    //dumby head node for each of the foundations
    struct singlyLinkedListNode foundationClubsHead;
    struct singlyLinkedListNode foundationDiamondsHead;
    struct singlyLinkedListNode foundationHeartsHead;
    struct singlyLinkedListNode foundationSpadesHead;

    //dumby head node for the waste pile
    struct singlyLinkedListNode wasteHead;

    //dumby head node for the stock pile
    struct singlyLinkedListNode stockHead;
};

void addToTableau(int pile, char suit, char value, int isCovered);

void resetReadInputFile();

void readInputFile(FILE* inputFile);

void processRules(FILE *inputFile);

void processFoundations(FILE *inputFile);

int processTableau(FILE *inputFile);

struct stockData processStock(FILE *inputFile);

void processMoves(FILE *inputFile);

void consumeWhiteSpaceAndComments(FILE *);

char getNextValuableChar(FILE *inputFile);

void lineError(const char* expectedText);

void populateDeckWithFoundation(char topCard, char suit);

void checkForDoubles(char cardValue, char cardSuit, struct card suit[]);

void checkForMissingCards();

int valueToIndex(char cardValue);

char indexToValue(int index);

void printCheckResults();

#endif