#include "advance.h"
#include "readInputFile.h"
#include "singlyLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * 1 if the m switch was enabled
 */
char isSwitchMEnabled;

/**
 * number of moves to make if the m switch is enabled
 */
int numMoveToComplete;

/**
 * 1 if the o switch was enabled
 */
char isSwitchOEnabled;

/**
 * file to write to if o switch is enabled
 */
char outputFile[100];

/**
 * 1 if the x switch was enabled
 */
char isSwitchXEnabled;

/**
 * Number of completed moves
 */
int numMovesCompleted = 0;

/**
* rules data for this game
*/
extern struct rulesData rules;

/**
* 1 if the moves section contains all valid moves, false otherwise
*/
char areMovesValid = 1;

/**
 * holds all of the information related to the stat eof the game
 */
extern struct gameState gameState;


/**
 * @author Eva Kohl Blake Dunn
 * This method reads an input file that describes a game configuration (possibly
 * in the middle of a game) followed by a sequence of zero or more moves. If the input file is valid,
 * then the method checks that the sequence of moves is valid, and displays the game state after the moves.
 * @param argc number of command line arguments
 * @param argv array of the command line arguments
 * @return status of the program, 0 for successful
 */
int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "m:o:x")) != -1) {
        switch (opt) {
            case 'm':
                isSwitchMEnabled = 1;
                numMoveToComplete = atoi(optarg);
                break;
            case 'o':
                isSwitchOEnabled = 1;
                strcpy(outputFile, optarg);
                break;
            case 'x':
                isSwitchXEnabled = 1;
                break;
            case ':':
                fprintf(stderr, "Switch needs a value\n");
                exit(-1);
                break;
            case '?':
                fprintf(stderr, "Usage: ./advance -m N -o file -x inputfile.\n");
                exit(-1);
                break;
        }
    }

    if (argc - optind > 1) {
        fprintf(stderr, "Usage: ./advance -m N -o file -x inputfile.\n");
        exit(-1);
    }

    FILE *inputFile;
    if (!(argc - optind)) {
        inputFile = stdin;
    } else {
        inputFile = fopen(argv[optind], "r");

        if (!inputFile) {
            fprintf(stderr, "Could not open file.  File does not exist.\n");
            return -1;
        }
    }

    readInputFile(inputFile);
    completeMoves(inputFile);
    printMovesResults(NULL);
    fclose(inputFile);
    return 0;
}

/**
 * @author Blake Dunn
 * begins reading the input file right after the MOVES: header and reads until the end of the moves section
 */
void completeMoves(FILE *inputFile) {
    consumeWhiteSpaceAndComments(inputFile);
    char curChar;
    while ((curChar = fgetc(inputFile)) != EOF) {
        if (isSwitchMEnabled && numMovesCompleted >= numMoveToComplete) {
            printMovesResults(NULL);
            exit(0);
        }

        int src;
        switch (curChar) {
            case '.':
                turnCardinStock();
                break;
            case 'r':
                resetStock();
                break;
            case 'w':
                if (curChar = fgetc(inputFile) != '-') {
                    lineError("src->dest, r, or .");
                }
                if (curChar = fgetc(inputFile) != '>') {
                    lineError("src->dest, r, or .");
                }
                switch (curChar = fgetc(inputFile)) {
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
                        wasteToTableau(curChar - '0');
                        break;
                    default: { //need brackets here to create scope for result[5]
                        char result[5];
                        sprintf(result, "w->%c", curChar);
                        moveError(result); }
                }
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                src = curChar - '0';
                if (curChar = fgetc(inputFile) != '-') {
                    lineError("src->dest, r, or .");
                }
                if (curChar = fgetc(inputFile) != '>') {
                    lineError("src->dest, r, or .");
                }
                switch (curChar = fgetc(inputFile)) {
                    case 'f':
                        tableauToFoundation(src);
                        break;
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                        tableauToTableau(src, curChar - '0');
                        break;
                    default: { //need brackets here to create scope for result[5]
                        char result[5];
                        sprintf(result, "%d->%c", src, curChar);
                        moveError(result); }
                }
                break;
            default:
                moveError("does not match any legal moves");
        }
        numMovesCompleted++;
        consumeWhiteSpaceAndComments(inputFile);
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
                isEmpty(&gameState.foundationClubsHead) && valueToIndex(peekLast(&gameState.wasteHead)->data.value) == 0) {
                offerWithReference(&gameState.foundationClubsHead, poll(&gameState.wasteHead));
            } else {
                moveError("w->f");
            }
            break;
        case 'd':
            if (valueToIndex(peekLast(&gameState.foundationDiamondsHead)->data.value) + 1 ==
                valueToIndex(peekLast(&gameState.wasteHead)->data.value) ||
                isEmpty(&gameState.foundationDiamondsHead) && valueToIndex(peekLast(&gameState.wasteHead)->data.value) == 0) {
                offerWithReference(&gameState.foundationDiamondsHead, poll(&gameState.wasteHead));
            } else {
                moveError("w->f");
            }
            break;
        case 'h':
            if (valueToIndex(peekLast(&gameState.foundationHeartsHead)->data.value) + 1 ==
                valueToIndex(peekLast(&gameState.wasteHead)->data.value) ||
                isEmpty(&gameState.foundationHeartsHead) && valueToIndex(peekLast(&gameState.wasteHead)->data.value) == 0) {
                offerWithReference(&gameState.foundationHeartsHead, poll(&gameState.wasteHead));
            } else {
                moveError("w->f");
            }
            break;
        case 's':
            if (valueToIndex(peekLast(&gameState.foundationSpadesHead)->data.value) + 1 ==
                valueToIndex(peekLast(&gameState.wasteHead)->data.value) ||
                isEmpty(&gameState.foundationSpadesHead) && valueToIndex(peekLast(&gameState.wasteHead)->data.value) == 0) {
                offerWithReference(&gameState.foundationSpadesHead, poll(&gameState.wasteHead));
            } else {
                moveError("w->f");
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
        char result[5];
        sprintf(result, "%c->%d", srcColumn, destColumn);
        moveError(result);
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
            char result[5];
            sprintf(result, "%c->%d", srcColumn, destColumn);
            moveError(result);
        }
    } else {
        if (!(suitTwo == 'h' || suitTwo == 'd')) {
            char result[5];
            sprintf(result, "%c->%d", srcColumn, destColumn);
            moveError(result);
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
        char result[5];
        sprintf(result, "%d->%d", srcColumn, destColumn);
        moveError(result);
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
    struct singlyLinkedListNode* dest = colNumToTableauPile(destColumn);
    char canPlaceKing = checkKing(peekLast(&gameState.wasteHead)->data.value, isEmpty(dest), -1, destColumn);
    if (canPlaceKing) {
        offerWithReference(dest, poll(&gameState.wasteHead));
    } else if (checkIfSuitColorIsDifferent(peekLast(dest)->data.suit, peekLast(&gameState.wasteHead)->data.suit, 'w', destColumn) &&
               isCardSmaller(peekLast(dest)->data.value, peekLast(&gameState.wasteHead)->data.value, 'w', destColumn)) {
        if (!isEmpty(&gameState.wasteHead)) {
            offerWithReference(dest, poll(&gameState.wasteHead));
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
    struct singlyLinkedListNode* src = colNumToTableauPile(srcColumn);
    switch (peekLast(src)->data.suit) {
        case 'c':
            if (valueToIndex(peekLast(&gameState.foundationClubsHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&gameState.foundationClubsHead) && valueToIndex(peekLast(src)->data.value) == 0) {
                offerWithReference(&gameState.foundationClubsHead, poll(src));
                peekLast(src)->data.isCovered = 0;
            } else {
                char result[5];
                sprintf(result, "%d->f", srcColumn);
                moveError(result);
            }
            break;
        case 'd':
            if (valueToIndex(peekLast(&gameState.foundationDiamondsHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&gameState.foundationDiamondsHead) && valueToIndex(peekLast(src)->data.value) == 0) {
                offerWithReference(&gameState.foundationDiamondsHead, poll(src));
                peekLast(src)->data.isCovered = 0;
            } else {
                char result[5];
                sprintf(result, "%d->f", srcColumn);
                moveError(result);
            }
            break;
        case 'h':
            if (valueToIndex(peekLast(&gameState.foundationHeartsHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&gameState.foundationHeartsHead) && valueToIndex(peekLast(src)->data.value) == 0) {
                offerWithReference(&gameState.foundationHeartsHead, poll(src));
                peekLast(src)->data.isCovered = 0;
            } else {
                char result[5];
                sprintf(result, "%d->f", srcColumn);
                moveError(result);
            }
            break;
        case 's':
            if (valueToIndex(peekLast(&gameState.foundationSpadesHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&gameState.foundationSpadesHead) && valueToIndex(peekLast(src)->data.value) == 0) {
                offerWithReference(&gameState.foundationSpadesHead, poll(src));
                peekLast(src)->data.isCovered = 0;
            } else {
                char result[5];
                sprintf(result, "%d->f", srcColumn);
                moveError(result);
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
    struct singlyLinkedListNode* src = colNumToTableauPile(srcColumn);
    struct singlyLinkedListNode* dest = colNumToTableauPile(destColumn);
   
    if (isEmpty(src)) {
        char result[5];
        sprintf(result, "%d->%d", srcColumn, destColumn);
        moveError(result);
    }

    struct singlyLinkedListNode *tmp = peekFirstUncovered(src);
    char canPlaceKing = checkKing(tmp->data.value, isEmpty(dest), srcColumn, destColumn);

    if (canPlaceKing) {
        offerWithReference(dest, pollFromMiddle(src, tmp));
        peekLast(src)->data.isCovered = 0;
        return;
    } else {
        while (tmp) {
            if (isCardSmaller(peekLast(dest)->data.value, tmp->data.value, srcColumn + '0', destColumn) &&
                    checkIfSuitColorIsDifferent(peekLast(dest)->data.suit, tmp->data.suit, srcColumn + '0', destColumn)) {
                offerWithReference(dest, pollFromMiddle(src, tmp));
                peekLast(src)->data.isCovered = 0;
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
    if (isEmpty(&gameState.stockHead)) {
        moveError(".");
    }

    if (rules.cardsPerTurn == 1) {
        offerWithReference(&gameState.wasteHead, poll(&gameState.stockHead));
    } else if (rules.cardsPerTurn == 3) {
        for (int i = 0; i < 3; i++) {
            if (!isEmpty(&gameState.stockHead)) {
                offerWithReference(&gameState.wasteHead, poll(&gameState.stockHead));
            }
        }
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
    static int numTimesReset = 0;
    if (!rules.unlimited && numTimesReset >= rules.limitNum) {
        moveError("r");
    }

    if (!gameState.stockHead.next) {
        while (!isEmpty(&gameState.wasteHead)) offerWithReference(&gameState.stockHead, poll(&gameState.wasteHead));
        numTimesReset++;
    } else {
        moveError("r");
    }
}

/**
* @author Blake Dunn
 *@author Eva Kohl
* prints off the results of processing the moves section
*/
void printMovesResults(const char* invalidMove) {
    if (areMovesValid) {
        printf("Processed %d moves, all valid\n", numMovesCompleted);
    } else {
        printf("Move %d is illegal: %s\n", numMovesCompleted + 1, invalidMove);
    }

    FILE *outFile;
    if (isSwitchOEnabled) {
        outFile = fopen(outputFile, "w");
    } else {
        outFile = stdout;
    }

    if(isSwitchXEnabled) {
        fprintf(outFile, "RULES: \n\tturn %d\n", rules.cardsPerTurn);
        if(rules.unlimited) {
            fprintf(outFile, "\tunlimited\n");
        } else {
            fprintf(outFile,"\tlimit %d\n", rules.limitNum );
        }
        fprintf(outFile,"FOUNDATIONS:\n");

        fprintf(outFile,  "\t%c%c\n", (gameState.foundationClubsHead.next) ? peekLast(&gameState.foundationClubsHead)->data.value : '_', 'c');
        fprintf(outFile,  "\t%c%c\n", (gameState.foundationDiamondsHead.next) ? peekLast(&gameState.foundationDiamondsHead)->data.value : '_', 'd');
        fprintf(outFile,  "\t%c%c\n", (gameState.foundationHeartsHead.next) ? peekLast(&gameState.foundationHeartsHead)->data.value : '_', 'h');
        fprintf(outFile,  "\t%c%c\n", (gameState.foundationSpadesHead.next) ? peekLast(&gameState.foundationSpadesHead)->data.value : '_' ,'s');
        fprintf(outFile, "TABLEAU:\n");
        printTableauForXSwitch(outFile);
        fprintf(outFile, "STOCK: \n\t");
        while(!isEmpty(&gameState.wasteHead)) {
            fprintf(outFile, "%c%c ", gameState.wasteHead.next->data.value, gameState.wasteHead.next->data.suit);
            pollFromFront(&gameState.wasteHead);
        }
        fprintf(outFile, "|");
        while(!isEmpty(&gameState.stockHead)) {
            struct singlyLinkedListNode temp = *poll(&gameState.stockHead);
            fprintf(outFile, " %c%c", temp.data.value, temp.data.suit);
        }

        fprintf(outFile, "\nMOVES:\n");

    } else {
        fprintf(outFile, "Foundations\n");
        fprintf(outFile, "%c%c ", (gameState.foundationClubsHead.next) ? peekLast(&gameState.foundationClubsHead)->data.value : '_', 'c');
        fprintf(outFile, "%c%c ", (gameState.foundationDiamondsHead.next) ? peekLast(&gameState.foundationDiamondsHead)->data.value : '_', 'd');
        fprintf(outFile, "%c%c ", (gameState.foundationHeartsHead.next) ? peekLast(&gameState.foundationHeartsHead)->data.value : '_', 'h');
        fprintf(outFile, "%c%c\n", (gameState.foundationSpadesHead.next) ? peekLast(&gameState.foundationSpadesHead)->data.value : '_', 's');
        fprintf(outFile, "Tableau\n");

        printPile(outFile);

        fprintf(outFile, "Waste top\n");
        if (isEmpty(&gameState.wasteHead)) {
            fprintf(outFile, "(empty)\n");
        } else {
            fprintf(outFile, "%c%c\n", peekLast(&gameState.wasteHead)->data.value, peekLast(&gameState.wasteHead)->data.suit);
        }
    }
}

/**
 * @author Eva Kohl
 * prints the tableau
 */
void printPile(FILE *outFile) {
    while (gameState.tableauPile1.next || gameState.tableauPile2.next || gameState.tableauPile3.next || gameState.tableauPile4.next ||
    gameState.tableauPile5.next || gameState.tableauPile6.next || gameState.tableauPile7.next) {
        printFromTab(&gameState.tableauPile1, outFile);
        printFromTab(&gameState.tableauPile2, outFile);
        printFromTab(&gameState.tableauPile3, outFile);
        printFromTab(&gameState.tableauPile4, outFile);
        printFromTab(&gameState.tableauPile5, outFile);
        printFromTab(&gameState.tableauPile6, outFile);
        printFromTab(&gameState.tableauPile7, outFile);
        fprintf(outFile, "\n");
    }
}

/**
 * @author Eva Kohl
 * takes in a tableau pile and prints off the bottom item on the stack
 * @param tabPile pile to print to outFile
 * @param outFile file to print results to
 */
void printFromTab(struct singlyLinkedListNode *tabPile, FILE *outFile) {
    if (!isEmpty(tabPile) && (tabPile->next->data.isCovered)) {
        fprintf(outFile, "## ");
        pollFromFront(tabPile);
    } else if (!isEmpty(tabPile)) {
        fprintf(outFile, "%c", (tabPile->next) ? tabPile->next->data.value : '.');
        fprintf(outFile, "%c ", (tabPile->next) ? tabPile->next->data.suit : '.');
        pollFromFront(tabPile);
    } else {
        fprintf(outFile, ".. ");
    }
}

/**
 * @author Eva Kohl
 * prints the tableau for the x switch configurations,
 * it also calls other helper method that further simplify the process.
 */
void printTableauForXSwitch(FILE *outFile) {
    for (int i = 7; i > 0; i--) {
        struct singlyLinkedListNode* tabPile = colNumToTableauPile(i);
        int isCovered = 1;
        fprintf(outFile, "\t");
        while (!isEmpty(tabPile)) {
            if (!tabPile->next->data.isCovered && isCovered) {
                fprintf(outFile, "| ");
                isCovered = 0;
            }

            fprintf(outFile, "%c", (tabPile->next) ? tabPile->next->data.value : ' ');
            fprintf(outFile, "%c ", (tabPile->next) ? tabPile->next->data.suit : ' ');
            pollFromFront(tabPile);
        }
        if(isCovered) {
            fprintf(outFile,"| ");
        }

        fprintf(outFile, "\n");
    }
}

/**
 * @author Blake Dunn
 * prints off the current state of the game when an illegal move is attempted
 */
void moveError(const char* errorMessage) {
    areMovesValid = 0;
    printMovesResults(errorMessage);
    exit(-1);
}

/**
 * @author Blake Dunn
 * converts a column number colNum into a a tableau pile
 */
struct singlyLinkedListNode* colNumToTableauPile(int colNum) {
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