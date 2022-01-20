#include "winnable.hpp"
#include "readInputFile.h"
#include "singlyLinkedList.h"
#include "cSwitchHashTable.hpp"
#include <unistd.h>
#include <iostream>
#include "movesSLL.hpp"
#include <string>

/**
 * bool to determine if switch M has been enabled
 */
bool isSwitchMEnabled = false;

/**
 * int that caps the max amount of moves a computer should try
 */
int numMovesForComputerToDo = 1000;

/**
 * The number of configs that hav ebeen checked
 */
unsigned long long numConfigsChecked = 0;

/**
 * The number of configs that was last printed
 */
unsigned long long lastNumConfigsCheckedPrinted = 0;

/**
 * bool to determine if switch C has been enabled
 */
bool isSwitchCEnabled = false;

/**
 * bool to determine if switch F has been enabled
 */
bool isSwitchFEnabled = false;

/**
 * bool to determine if switch V has been enabled
 */
bool isSwitchVEnabled = false;

/**
 * gameState after readInputFile
 */
extern struct gameState gameState;

/**
* rules data for this game
*/
extern struct rulesData rules;

/**
 * The number of times the stock has been reset
 */
int numTimesStockReset;

/**
 * The hash table that contains the game configurations
 */
cSwitchHashTable hashTable(10000000);

using namespace std;

/**
 * @author Blake Dunn
 * Takes in a solataire file argument or input from stdin and tries to complete 
 * the game.  The results of whether the game is completable are printed to 
 * stdout.  The available switches are m, c, and v.
 */
int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, ":m:cfv")) != -1) {
        switch (opt) {
            case 'm':
                isSwitchMEnabled = true;
                numMovesForComputerToDo = atoi(optarg);
                break;
            case 'c':
                isSwitchCEnabled = true;
                break;
            case 'f':
                isSwitchFEnabled = true;
                break;
            case 'v':
                isSwitchVEnabled = true;
                break;
            case ':':
                cerr << "Switch needs a value\n";
                exit(-1);
                break;
            case '?':
                cerr << "Not a valid switch.  Available switches -m N -c -f -v\n";
                exit(-1);
                break;
        }
    }

    if (argc - optind > 1) {
        cerr << "Usage: ./advance -m N -c -f -v inputfile.\n";
        exit(-1);
    }

    FILE *inputFile;
    if (!(argc - optind)) {
        inputFile = stdin;
    } else {
        inputFile = fopen(argv[optind], "r");

        if (!inputFile) {
            cerr << "Could not open file.  File does not exist.\n";
            return -1;
        }
    }

    readInputFile(inputFile);

    movesSLL completedMoves;
    if (!attemptMove(gameState, completedMoves)) {
        if (isSwitchCEnabled) cerr << "Cache had " << hashTable.numberOfEntries() << " entries." << endl;
        cout << "# Game is not winnable within " << numMovesForComputerToDo << " moves" << endl;
    }

    fclose(inputFile);
    return 0;
}


/**
 * @author Blake Dunn
 * A recursive method that attempts different moves until a successful move is
 * made.
 */
bool attemptMove(struct gameState& curGameState, movesSLL completedMoves) {
    if (isInWinningState(curGameState)) {
        if (isSwitchVEnabled) cerr << numConfigsChecked << " configurations checked so far\n";

        cout << "# Game is winnable in " << completedMoves.getSize() << " moves\n";
        cout << completedMoves;
        if (isSwitchCEnabled) cerr << "Cache had " << hashTable.numberOfEntries() << " entries." << endl;
        return true;
    } else if (completedMoves.getSize() < numMovesForComputerToDo) {
        if (isSwitchVEnabled && lastNumConfigsCheckedPrinted + 1000000 < numConfigsChecked) {
            cerr << numConfigsChecked << " configurations checked so far\n";
            lastNumConfigsCheckedPrinted = numConfigsChecked;
        }
        
        if(isSwitchCEnabled) {
            if(hashTable.ifEntryExists(curGameState) && hashTable.get(curGameState) > (numMovesForComputerToDo - completedMoves.getSize())) {
                return false;
            }
        }

        //force moves to the foundations if the f switch is enabled
        if (isSwitchFEnabled) {
            //Tableau to foundation
            for (int i = 1; i < 8; i++) {
                if (canMoveTableauToFoundation(curGameState, i)) {
                    struct gameState* tempGameState = new struct gameState;
                    deepCopyGameStateStruct(&curGameState, tempGameState);
                    movesSLL tempCompletedMoves = completedMoves;

                    struct card tempCard = peekLast(colNumToTableauPile(*tempGameState, i))->data;

                    if (isSafeToMoveToFoundations(*tempGameState, tempCard.suit, tempCard.value, false)) {
                        tableauToFoundation(*tempGameState, i);
                        tempCompletedMoves.offer(to_string(i) + "->f");

                        bool worked = attemptMove(*tempGameState, tempCompletedMoves);
                        destroyGameState(tempGameState);
                        delete tempGameState;
                        return worked;
                    } else {
                        destroyGameState(tempGameState);
                        delete tempGameState;
                    }
                }
            }

            //Waste to foundation movement
            if (canMoveWasteToFoundation(curGameState)) {
                struct gameState* tempGameState = new struct gameState;
                deepCopyGameStateStruct(&curGameState, tempGameState);
                movesSLL tempCompletedMoves = completedMoves;

                struct card tempCard = peekLast(&tempGameState->wasteHead)->data;

                if (isSafeToMoveToFoundations(*tempGameState, tempCard.suit, tempCard.value, true)) {
                    wasteToFoundations(*tempGameState);
                    tempCompletedMoves.offer("w->f");

                    bool worked = attemptMove(*tempGameState, tempCompletedMoves);
                    destroyGameState(tempGameState);
                    delete tempGameState;
                    return worked;
                } else {
                    destroyGameState(tempGameState);
                    delete tempGameState;
                }
            }
        }

        //Waste to foundation movement
        if (canMoveWasteToFoundation(curGameState)) {
            struct gameState* tempGameState = new struct gameState;
            deepCopyGameStateStruct(&curGameState, tempGameState);
            movesSLL tempCompletedMoves = completedMoves;

            wasteToFoundations(*tempGameState);
            tempCompletedMoves.offer("w->f");

            if (attemptMove(*tempGameState, tempCompletedMoves)) {
                destroyGameState(tempGameState);
                delete tempGameState;
                return true;
            } else {
                hashTable.insertItem(*tempGameState, (unsigned int)(numMovesForComputerToDo - tempCompletedMoves.getSize()));
                destroyGameState(tempGameState);
                delete tempGameState;
            }
        }

        //Tableau to tableau movement
        for (int i = 1; i < 8; i++) {
            for (int j = 1; j < 8; j++) {
                if (i != j  && completedMoves.peekLast().compare(to_string(j) + "->" + to_string(i)) && canMoveTableauToTableau(curGameState, i, j)) {
                    struct gameState* tempGameState = new struct gameState;
                    deepCopyGameStateStruct(&curGameState, tempGameState);
                    movesSLL tempCompletedMoves = completedMoves;

                    tableauToTableau(*tempGameState, i, j);
                    tempCompletedMoves.offer(to_string(i) + "->" + to_string(j));

                    if (attemptMove(*tempGameState, tempCompletedMoves)) {
                        destroyGameState(tempGameState);
                        delete tempGameState;
                        return true;
                    } else {
                        hashTable.insertItem(*tempGameState, (unsigned int)(numMovesForComputerToDo - tempCompletedMoves.getSize()));
                        destroyGameState(tempGameState);
                        delete tempGameState;
                    }
                }
            }
        }

        //Waste to tableau movement
        for (int i = 1; i < 8; i++) {
            if (canMoveWasteToTableau(curGameState, i)) {
                struct gameState* tempGameState = new struct gameState;
                deepCopyGameStateStruct(&curGameState, tempGameState);
                movesSLL tempCompletedMoves = completedMoves;

                wasteToTableau(*tempGameState, i);
                tempCompletedMoves.offer("w->" + to_string(i));

                if (attemptMove(*tempGameState, tempCompletedMoves)) {
                    destroyGameState(tempGameState);
                    delete tempGameState;
                    return true;
                } else {
                    hashTable.insertItem(*tempGameState, (unsigned int)(numMovesForComputerToDo - tempCompletedMoves.getSize()));
                    destroyGameState(tempGameState);
                    delete tempGameState;
                }
            }
        }

        //Turn card in stock
        if (canTurnCardInStock(curGameState)) {
            struct gameState* tempGameState = new struct gameState;
            deepCopyGameStateStruct(&curGameState, tempGameState);
            movesSLL tempCompletedMoves = completedMoves;

            turnCardInStock(*tempGameState);
            tempCompletedMoves.offer(".");

            if (attemptMove(*tempGameState, tempCompletedMoves)) {
                destroyGameState(tempGameState);
                delete tempGameState;
                return true;
            } else {
                hashTable.insertItem(*tempGameState, (unsigned int)(numMovesForComputerToDo - tempCompletedMoves.getSize()));
                destroyGameState(tempGameState);
                delete tempGameState;
            }
        }

        //Tableau to foundation
        if (!isSwitchFEnabled) {
            for (int i = 1; i < 8; i++) {
                if (canMoveTableauToFoundation(curGameState, i)) {
                    struct gameState* tempGameState = new struct gameState;
                    deepCopyGameStateStruct(&curGameState, tempGameState);
                    movesSLL tempCompletedMoves = completedMoves;

                    tableauToFoundation(*tempGameState, i);
                    tempCompletedMoves.offer(to_string(i) + "->f");

                    if (attemptMove(*tempGameState, tempCompletedMoves)) {
                        destroyGameState(tempGameState);
                        delete tempGameState;
                        return true;
                    } else {
                        hashTable.insertItem(*tempGameState, (unsigned int)(numMovesForComputerToDo - tempCompletedMoves.getSize()));
                        destroyGameState(tempGameState);
                        delete tempGameState;
                    }
                }
            }
        }

        //Reset Stock
        if (canResetStock(curGameState) && completedMoves.getSize() < 2 || canResetStock(curGameState) && completedMoves.peek(completedMoves.getSize() - 2).compare("r") && completedMoves.peek(completedMoves.getSize() - 1).compare(".")) {
            struct gameState* tempGameState = new struct gameState;
            deepCopyGameStateStruct(&curGameState, tempGameState);
            movesSLL tempCompletedMoves = completedMoves;

            resetStock(*tempGameState);
            tempCompletedMoves.offer("r");

            if (attemptMove(*tempGameState, tempCompletedMoves)) {
                destroyGameState(tempGameState);
                delete tempGameState;
                return true;
            } else {
                hashTable.insertItem(*tempGameState, (unsigned int)(numMovesForComputerToDo - tempCompletedMoves.getSize()));
                destroyGameState(tempGameState);
                delete tempGameState;
            }
        }

        return false;
    }
    return false;
}

/**
 * @author Blake Dunn
 * Checks if the game is in a winning state.
 * (no covered cards, at most 1 waste card, no stock cards)
 */
bool isInWinningState(struct gameState& curGameState) {
    bool isTableauAllUncovered = areAllCardsUncovered(&curGameState.tableauPile1)
        && areAllCardsUncovered(&curGameState.tableauPile2)
        && areAllCardsUncovered(&curGameState.tableauPile3)
        && areAllCardsUncovered(&curGameState.tableauPile4)
        && areAllCardsUncovered(&curGameState.tableauPile5)
        && areAllCardsUncovered(&curGameState.tableauPile6)
        && areAllCardsUncovered(&curGameState.tableauPile7);
    return isTableauAllUncovered && isEmpty(&curGameState.stockHead) && size(&curGameState.wasteHead) <= 1;
}

/**
 * @author Eva Kohl
 * This method checks to see if movement is possible between waste and tableau
 * @param curGameState
 * @param destColumn
 * @return true if move is possible, false otherwise
 */
bool canMoveWasteToTableau(struct gameState& curGameState, int destColumn) {
    numConfigsChecked++;
    struct singlyLinkedListNode* dest = colNumToTableauPile(curGameState, destColumn);
    if(checkKing(curGameState.wasteHead.data.value, isEmpty(dest))) return true;
    return isSuitColorDifferent(peekLast(dest)->data.suit, peekLast(&curGameState.wasteHead)->data.suit) &&
               isCardSmaller(peekLast(dest)->data.value, peekLast(&curGameState.wasteHead)->data.value) &&
               !isEmpty(&curGameState.wasteHead);
}

/**
 * @author Eva Kohl & Blake Dunn
 * @param curGameState
 * @param tableauColumn
 * @return true or false depending on if the move is possible
 */
bool canMoveTableauToFoundation(struct gameState &curGameState, int tableauColumn) {
    numConfigsChecked++;
    struct singlyLinkedListNode* src = colNumToTableauPile(curGameState, tableauColumn);

    if (isEmpty(src)) return false;

    switch (peekLast(src)->data.suit) {
        case 'c':
            return (valueToIndex(peekLast(&curGameState.foundationClubsHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&curGameState.foundationClubsHead) && valueToIndex(peekLast(src)->data.value) == 0);
            break;
        case 'd':
            return (valueToIndex(peekLast(&curGameState.foundationDiamondsHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&curGameState.foundationDiamondsHead) && valueToIndex(peekLast(src)->data.value) == 0);
            break;
        case 'h':
            return (valueToIndex(peekLast(&curGameState.foundationHeartsHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&curGameState.foundationHeartsHead) && valueToIndex(peekLast(src)->data.value) == 0);
            break;
        case 's':
            return (valueToIndex(peekLast(&curGameState.foundationSpadesHead)->data.value) + 1 ==
                valueToIndex(peekLast(src)->data.value) ||
                isEmpty(&curGameState.foundationSpadesHead) && valueToIndex(peekLast(src)->data.value) == 0);
            break;
        default:
            return false;
    }
}

/**
 * @author Eva Kohl
 * Valid moves:
 *              moving a king to an empty spot
 *              moving a card to to a card one larger and of a different color

 * @param curGameState
 * @param srcColumn
 * @param destColumn
 * @return true or false depending on if the move is possible
 */
bool canMoveTableauToTableau(struct gameState& curGameState, int srcColumn, int destColumn) {
    numConfigsChecked++;

    struct singlyLinkedListNode* src = colNumToTableauPile(curGameState, srcColumn);
    struct singlyLinkedListNode* dest = colNumToTableauPile(curGameState, destColumn);

    if (isEmpty(src)) {
        return false;
    }

    struct singlyLinkedListNode *srcFirstUncovered = peekFirstUncovered(src);

    char canPlaceKing = checkKing(srcFirstUncovered->data.value, isEmpty(dest));
    if (srcFirstUncovered->data.suit == src->next->data.suit && srcFirstUncovered->data.value == src->next->data.value && srcFirstUncovered->data.value == 'K' && isEmpty(dest)) {
        return false;
    } else if (canPlaceKing) {
        return true;
    } else if (srcFirstUncovered->data.suit == src->next->data.suit && srcFirstUncovered->data.value == src->next->data.value && srcFirstUncovered->data.value == 'K' && isEmpty(dest)) {
        return false;
    } else {
        while (srcFirstUncovered) {
            if (isCardSmaller(peekLast(dest)->data.value, srcFirstUncovered->data.value) &&
                    isSuitColorDifferent(peekLast(dest)->data.suit, srcFirstUncovered->data.suit)) {
                return true;
            } else {
                srcFirstUncovered = srcFirstUncovered->next;
            }
        }
        return false;
    }
}

/**
 * @author Blake Dunn
 * @param curGameState
 * @return true or false depending on if the move is possible
 */
bool canTurnCardInStock(struct gameState &curGameState) {
    numConfigsChecked++;
    return !isEmpty(&curGameState.stockHead);
}

/**
 * @author Blake Dunn
 * @param curGameState
 * @return true or false depending on if the move is possible
 */
bool canResetStock(struct gameState& curGameState) {
    numConfigsChecked++;
    if (!rules.unlimited && numTimesStockReset >= rules.limitNum || curGameState.stockHead.next) {
        return false;
    } else {
        return true;
    }
}

/**
 * @author Blake Dunn
 * @param curGameState
 * @return true or false depending on if the move is possible
 */
bool canMoveWasteToFoundation(struct gameState& curGameState) {
    numConfigsChecked++;

    if (isEmpty(&curGameState.wasteHead)) return false;

    switch (peekLast(&curGameState.wasteHead)->data.suit) {
        case 'c':
            return (valueToIndex(peekLast(&curGameState.foundationClubsHead)->data.value) + 1 ==
                valueToIndex(peekLast(&curGameState.wasteHead)->data.value) ||
                isEmpty(&curGameState.foundationClubsHead) && valueToIndex(peekLast(&curGameState.wasteHead)->data.value) == 0);
            break;
        case 'd':
            return (valueToIndex(peekLast(&curGameState.foundationDiamondsHead)->data.value) + 1 ==
                valueToIndex(peekLast(&curGameState.wasteHead)->data.value) ||
                isEmpty(&curGameState.foundationDiamondsHead) && valueToIndex(peekLast(&curGameState.wasteHead)->data.value) == 0);
            break;
        case 'h':
            return (valueToIndex(peekLast(&curGameState.foundationHeartsHead)->data.value) + 1 ==
                valueToIndex(peekLast(&curGameState.wasteHead)->data.value) ||
                isEmpty(&curGameState.foundationHeartsHead) && valueToIndex(peekLast(&curGameState.wasteHead)->data.value) == 0);
            break;
        case 's':
            return (valueToIndex(peekLast(&curGameState.foundationSpadesHead)->data.value) + 1 ==
                valueToIndex(peekLast(&curGameState.wasteHead)->data.value) ||
                isEmpty(&curGameState.foundationSpadesHead) && valueToIndex(peekLast(&curGameState.wasteHead)->data.value) == 0);
            break;
        default:
            return false;
    }
}

/**
 * @author Blake Dunn
 * Turning over the next cards. If there are cards remaining in the stock, we may turn over (either
 * 1 or 3, depending on the rules) stock cards and place them on top of the waste pile.
 */
void turnCardInStock(struct gameState& curGameState) {
    if (rules.cardsPerTurn == 1) {
        offerWithReference(&curGameState.wasteHead, poll(&curGameState.stockHead));
    } else if (rules.cardsPerTurn == 3) {
        for (int i = 0; i < 3; i++) {
            if (!isEmpty(&curGameState.stockHead)) {
                offerWithReference(&curGameState.wasteHead, poll(&curGameState.stockHead));
            }
        }
    }
}

/**
 * @author Eva Kohl
 * Moving the top waste card to the tableau. If the top waste card is a King, it may be moved
 * to an empty tableau column. Otherwise, the top waste card may be placed onto the bottom
 * card in a tableau column, if the waste card has rank one lower than, and the opposite color
 * of, the bottom card.
 */
void wasteToTableau(struct gameState& curGameState,int destColumn) {
    offerWithReference(colNumToTableauPile(curGameState, destColumn), poll(&curGameState.wasteHead));
}

/**
 * @author Eva Kohl
 * Moving a tableau card to the foundations. If the bottom card of a tableau column is an Ace,
 * it may be moved to the appropriate (empty) foundation. Otherwise, the bottom card of a tableau
 * column may be placed on top of the card with one lower rank, in the same suit, in the foundations.
 */
void tableauToFoundation(struct gameState& curGameState, int srcColumn) {
    struct singlyLinkedListNode* src = colNumToTableauPile(curGameState, srcColumn);
    switch (peekLast(src)->data.suit) {
        case 'c':
            offerWithReference(&curGameState.foundationClubsHead, poll(src));
            break;
        case 'd':
            offerWithReference(&curGameState.foundationDiamondsHead, poll(src));
            break;
        case 'h':
            offerWithReference(&curGameState.foundationHeartsHead, poll(src));
            break;
        case 's':
            offerWithReference(&curGameState.foundationSpadesHead, poll(src));
            break;
    }
    peekLast(src)->data.isCovered = 0;
}

/**
 * @author Eva Kohl
 * Moving tableau cards from one column to another. Any pile of cards, starting from any uncovered card,
 * and including all cards down to the bottom of the column, may be moved from one tableau column to
 * another column, if the top card in the pile could be moved to the destination tableau column as a
 * waste card.
 */
void tableauToTableau(struct gameState& curGameState, int srcColumn, int destColumn) {
    struct singlyLinkedListNode* src = colNumToTableauPile(curGameState, srcColumn);
    struct singlyLinkedListNode* dest = colNumToTableauPile(curGameState, destColumn);
    struct singlyLinkedListNode *srcFirstUncovered = peekFirstUncovered(src);

    char canPlaceKing = checkKing(srcFirstUncovered->data.value, isEmpty(dest));

    if (canPlaceKing) {
        offerWithReference(dest, pollFromMiddle(src, srcFirstUncovered));
        peekLast(src)->data.isCovered = 0;
        return;
    } else {
        while (srcFirstUncovered) {
            if (isCardSmaller(peekLast(dest)->data.value, srcFirstUncovered->data.value) &&
                    isSuitColorDifferent(peekLast(dest)->data.suit, srcFirstUncovered->data.suit)) {
                offerWithReference(dest, pollFromMiddle(src, srcFirstUncovered));
                peekLast(src)->data.isCovered = 0;
                return;
            } else {
                srcFirstUncovered = srcFirstUncovered->next;
            }
        }
    }
}

/**
 * @author Blake Dunn
 * Moving the top waste card to the foundations. If the top waste card is an Ace, it may be
 * moved to the appropriate (empty) foundation. Otherwise, the top waste card may be placed
 * on top of the card with one lower rank, in the same suit, in the foundations.
 */
void wasteToFoundations(struct gameState& curGameState) {
    switch (peekLast(&curGameState.wasteHead)->data.suit) {
        case 'c':
            offerWithReference(&curGameState.foundationClubsHead, poll(&curGameState.wasteHead));
            break;
        case 'd':
            offerWithReference(&curGameState.foundationDiamondsHead, poll(&curGameState.wasteHead));
            break;
        case 'h':
            offerWithReference(&curGameState.foundationHeartsHead, poll(&curGameState.wasteHead));
            break;
        case 's':
            offerWithReference(&curGameState.foundationSpadesHead, poll(&curGameState.wasteHead));
            break;
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
void resetStock(struct gameState& curGameState) {
    while (!isEmpty(&curGameState.wasteHead)) offerWithReference(&curGameState.stockHead, poll(&curGameState.wasteHead));
    numTimesStockReset++;
}

/**
 * @author Blake Dunn
 * converts a column number colNum into a a tableau pile
 */
struct singlyLinkedListNode* colNumToTableauPile(struct gameState& curGameState, int colNum) {
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
 * @author Eva Kohl
 * checks if a card is one lower then it's card to be placed on
 */
bool isCardSmaller(char bottomCard, char cardToBePlaced) {
    if (valueToIndex(bottomCard) != (valueToIndex(cardToBePlaced) + 1)) {
        return false;
    }
    return true;
}

/**
 * @author Eva Kohl
 * checks if two suits are of the opposite color
 */
bool isSuitColorDifferent(char suitOne, char suitTwo) {
    if (suitOne == 'h' || suitOne == 'd') {
        if (!(suitTwo == 's' || suitTwo == 'c')) {
            return false;
        }
    } else {
        if (!(suitTwo == 'h' || suitTwo == 'd')) {
            return false;
        }
    }
    return true;
}

/**
 * @author Eva Kohl Blake Dunn
 * returns 1 if valid king placement, 0 otherwise
 */
bool checkKing(char value, char isEmpty) {
    if (value == 'K' && isEmpty) {
        return true;
    }
    return false;
}

/**
 * @author Blake Dunn
 * creates a deep copy of a singlyLinkedList struct
 */
void deepCopySSLStruct(struct singlyLinkedListNode* src, struct singlyLinkedListNode* dest) {
    dest->next = 0;
    src = src->next;
    while (src) {
        offerWithCover(dest, src->data.suit, src->data.value, src->data.isCovered);
        src = src->next;
    }
}

/**
 * @author Blake Dunn
 * Creates a deep copy of a gameState struct
 */
void deepCopyGameStateStruct(struct gameState* src, struct gameState* dest) {
    deepCopySSLStruct(&src->tableauPile1, &dest->tableauPile1);
    deepCopySSLStruct(&src->tableauPile2, &dest->tableauPile2);
    deepCopySSLStruct(&src->tableauPile3, &dest->tableauPile3);
    deepCopySSLStruct(&src->tableauPile4, &dest->tableauPile4);
    deepCopySSLStruct(&src->tableauPile5, &dest->tableauPile5);
    deepCopySSLStruct(&src->tableauPile6, &dest->tableauPile6);
    deepCopySSLStruct(&src->tableauPile7, &dest->tableauPile7);

    deepCopySSLStruct(&src->foundationClubsHead, &dest->foundationClubsHead);
    deepCopySSLStruct(&src->foundationDiamondsHead, &dest->foundationDiamondsHead);
    deepCopySSLStruct(&src->foundationHeartsHead, &dest->foundationHeartsHead);
    deepCopySSLStruct(&src->foundationSpadesHead, &dest->foundationSpadesHead);

    deepCopySSLStruct(&src->wasteHead, &dest->wasteHead);
    deepCopySSLStruct(&src->stockHead, &dest->stockHead);
}

/**
 * @author Blake Dunn
 * Complete destroys the gameState struct except for dumby nodes
 */
void destroyGameState(struct gameState* curGameState) {
    destory(&curGameState->tableauPile1);
    destory(&curGameState->tableauPile2);
    destory(&curGameState->tableauPile3);
    destory(&curGameState->tableauPile4);
    destory(&curGameState->tableauPile5);
    destory(&curGameState->tableauPile6);
    destory(&curGameState->tableauPile7);

    destory(&curGameState->foundationClubsHead);
    destory(&curGameState->foundationDiamondsHead);
    destory(&curGameState->foundationHeartsHead);
    destory(&curGameState->foundationSpadesHead);

    destory(&curGameState->stockHead);
    destory(&curGameState->wasteHead);
}

/**
 * @author Blake Dunn
 * determines if it is safe to force moves to the foundations
 * under the following criteria:
 *
 * 1. The card is not being moved from the waste pile, unless
 * we are turning 1 card over at a time
 * 2. The current rank of the foundation suit we are adding to
 * is at least as large as the ranks of the foundation card of
 * the opposite color
 */
bool isSafeToMoveToFoundations(struct gameState& curGameState, char suit, char value, bool isFromWaste) {
    bool firstCondition = (!isFromWaste || rules.cardsPerTurn == 1);
    bool secondCondition;
    if (suit == 'c' || suit == 's') {
        if ((isEmpty(&curGameState.foundationHeartsHead) || isEmpty(&curGameState.foundationDiamondsHead)) && value != 'A') return false;
        secondCondition = ((isEmpty(&curGameState.foundationHeartsHead) || isEmpty(&curGameState.foundationDiamondsHead)) && value == 'A')
        || (valueToIndex(peekLast(&curGameState.foundationHeartsHead)->data.value) >= valueToIndex(value) - 1
            && valueToIndex(peekLast(&curGameState.foundationDiamondsHead)->data.value) >= valueToIndex(value) - 1);
    } else {
        if ((isEmpty(&curGameState.foundationClubsHead) || isEmpty(&curGameState.foundationSpadesHead)) && value != 'A') return false;
        secondCondition = ((isEmpty(&curGameState.foundationClubsHead) || isEmpty(&curGameState.foundationSpadesHead)) && value == 'A')
        || (valueToIndex(peekLast(&curGameState.foundationClubsHead)->data.value) >= valueToIndex(value) - 1
                && valueToIndex(peekLast(&curGameState.foundationSpadesHead)->data.value) >= valueToIndex(value) - 1);
    }

    return firstCondition && secondCondition;
}
