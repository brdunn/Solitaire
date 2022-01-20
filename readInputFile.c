#include <stdlib.h>
#include <string.h>
#include "readInputFile.h"
#include "singlyLinkedList.h"


/**
 * Number of lines read from the setup file
 */
int readLines = 1;

/**
 * Implements a deck of playing cards
 */
struct deck fullDeck;

/**
 * Holds the number of cards to be turned over from the stock at a time and the number of times the stock
 * can be reset.
 */
struct rulesData rules;

/**
 * 1 if the file is valid, 0 otherwise
 */
char inputFileIsValid = 1;

/**
 * the number of covered cards after reading the file
 */
int coveredCards;

/**
 * holds information related to the stock
 */
struct stockData stock;

/**
 * holds all of the information related to the state of the game
 */
struct gameState gameState;


/**
 * @author Blake Dunn
 * Resets the readin gof the inputFile to the beginnng state
 */
void resetReadInputFile() {
    for (int i = 0; i < NUM_CARDS_IN_SUIT; i++) {
        fullDeck.clubs[i].isCovered = 0;
        fullDeck.diamonds[i].isCovered = 0;
        fullDeck.hearts[i].isCovered = 0;
        fullDeck.spades[i].isCovered = 0;
    }

    readLines = 1;
}


/**
* @author Blake Dunn
* @author Eva Kohl
* reads a game setup from either a file or stdin.  If the file is valid,
* the function returns 0.  Otherwise, -1.
*/
void readInputFile(FILE* inputFile) {

    processRules(inputFile);

    processFoundations(inputFile);

    coveredCards = processTableau(inputFile);

    stock = processStock(inputFile);

    processMoves(inputFile);

    checkForMissingCards();

}

/**
 * @author Blake Dunn
 * @author Eva Kohl
 * prints off the results for the check executable
 */
void printCheckResults() {
    if (inputFileIsValid) {
        printf("Input file is valid\n%d covered cards\n%d stock cards\n%d waste cards\n", coveredCards, stock.stockCard,
               stock.wasteCard);
    } else {
        fprintf(stderr, "\n");
        exit(-1);
    }
}

/**
 * @author Blake Dunn
 * Checks the deck to see is there are any cards that are missing from the setup file
 */
void checkForMissingCards() {
    for (int i = 0; i < NUM_CARDS_IN_SUIT; i++) {
        char cardValue = indexToValue(i);

        if (!fullDeck.clubs[i].isCovered) {
            if (inputFileIsValid) {
                fprintf(stderr, "Missing cards: ");
                inputFileIsValid = 0;
            }
            fprintf(stderr, "%cc ", cardValue);
        }
        if (!fullDeck.diamonds[i].isCovered) {
            if (inputFileIsValid) {
                fprintf(stderr, "Missing cards: ");
                inputFileIsValid = 0;
            }
            fprintf(stderr, "%cd ", cardValue);
        }
        if (!fullDeck.hearts[i].isCovered) {
            if (inputFileIsValid) {
                fprintf(stderr, "Missing cards: ");
                inputFileIsValid = 0;
            }
            fprintf(stderr, "%ch ", cardValue);
        }
        if (!fullDeck.spades[i].isCovered) {
            if (inputFileIsValid) {
                fprintf(stderr, "Missing cards: ");
                inputFileIsValid = 0;
            }
            fprintf(stderr, "%cs ", cardValue);
        }
    }
}

/**
* @author Blake Dunn
* processes the rules section and returns the appropriate data structure
*/
void processRules(FILE *inputFile) {

    consumeWhiteSpaceAndComments(inputFile);

    char rulesHeader[7];
    fgets(rulesHeader, 7, inputFile);
    if (strcmp(rulesHeader, "RULES:")) {
        lineError("RULES:");
    }

    consumeWhiteSpaceAndComments(inputFile);
    for (int i = 0; i < 5; i++) {
        fgetc(inputFile);
    }
    rules.cardsPerTurn = fgetc(inputFile) - '0';

    consumeWhiteSpaceAndComments(inputFile);
    if (fgetc(inputFile) == 'u') {
        for (int i = 0; i < 8; i++) {
            fgetc(inputFile);
        }
        rules.unlimited = 1;
    } else {
        for (int i = 0; i < 5; i++) {
            fgetc(inputFile);
        }
        rules.unlimited = 0;
        rules.limitNum = fgetc(inputFile) - '0';
    }
}

/**
* @author Blake Dunn
* processes the foundations section
*/
void processFoundations(FILE *inputFile) {
    consumeWhiteSpaceAndComments(inputFile);

    char header[13];
    fgets(header, 13, inputFile);
    if (strcmp(header, "FOUNDATIONS:")) {
        lineError("FOUNDATIONS:");
    }

    consumeWhiteSpaceAndComments(inputFile);

    populateDeckWithFoundation(fgetc(inputFile), 'c');
    fgetc(inputFile);
    consumeWhiteSpaceAndComments(inputFile);

    populateDeckWithFoundation(fgetc(inputFile), 'd');
    fgetc(inputFile);
    consumeWhiteSpaceAndComments(inputFile);

    populateDeckWithFoundation(fgetc(inputFile), 'h');
    fgetc(inputFile);
    consumeWhiteSpaceAndComments(inputFile);

    populateDeckWithFoundation(fgetc(inputFile), 's');
    fgetc(inputFile);
    consumeWhiteSpaceAndComments(inputFile);
}

/**
 * @author Eva Kohl
 * This is a method for placing a card on the correct Tableau Pile
 */
void addToTableau(int pile, char suit, char value, int isCovered) {
    switch (pile) {
        case 1:
            offerWithCover(&gameState.tableauPile1, suit, value, isCovered);
            break;
        case 2:
            offerWithCover(&gameState.tableauPile2, suit, value, isCovered);
            break;
        case 3:
            offerWithCover(&gameState.tableauPile3, suit, value, isCovered);
            break;
        case 4:
            offerWithCover(&gameState.tableauPile4, suit, value, isCovered);
            break;
        case 5:
            offerWithCover(&gameState.tableauPile5, suit, value, isCovered);
            break;
        case 6:
            offerWithCover(&gameState.tableauPile6, suit, value, isCovered);
            break;
        case 7:
            offerWithCover(&gameState.tableauPile7, suit, value, isCovered);
            break;
    }
}

/**
* @author Blake Dunn
* Fills all of the deck in the deck that have values less than topCard in the given suit[]
*/
void populateDeckWithFoundation(char topCard, char suit) {
    if (topCard != '_') {
        int cardIndexInDeck = valueToIndex(topCard);
        for (int i = 0; i < NUM_CARDS_IN_SUIT; i++) {
            if (cardIndexInDeck >= i) {
                switch (suit) {
                    case 'c':
                        fullDeck.clubs[i].isCovered = 1;
                        offerWithCover(&gameState.foundationClubsHead, 'c', indexToValue(i), 0);
                        break;
                    case 'd':
                        fullDeck.diamonds[i].isCovered = 1;
                        offerWithCover(&gameState.foundationDiamondsHead, 'd', indexToValue(i), 0);
                        break;
                    case 'h':
                        fullDeck.hearts[i].isCovered = 1;
                        offerWithCover(&gameState.foundationHeartsHead, 'h', indexToValue(i), 0);
                        break;
                    case 's':
                        fullDeck.spades[i].isCovered = 1;
                        offerWithCover(&gameState.foundationSpadesHead, 's', indexToValue(i), 0);
                        break;
                }
            } else {
                switch (suit) {
                    case 'c':
                        fullDeck.clubs[i].isCovered = 0;
                        break;
                    case 'd':
                        fullDeck.diamonds[i].isCovered = 0;
                        break;
                    case 'h':
                        fullDeck.hearts[i].isCovered = 0;
                        break;
                    case 's':
                        fullDeck.spades[i].isCovered = 0;
                        break;
                }
            }
        }
    }
}

/**
 * @author Eva Kohl
 * This method processes the tableau section of the file
 */
int processTableau(FILE *inputFile) {
    int covered = 0;

    consumeWhiteSpaceAndComments(inputFile);

    char header[9];
    fgets(header, 9, inputFile);
    if (strcmp(header, "TABLEAU:")) {
        lineError("TABLEAU:");
    }

    consumeWhiteSpaceAndComments(inputFile);
    char cur = fgetc(inputFile);

    for (int column = 7; column > 0; column--) {
        int isCovered = 1;
        char lastSuit = '0';
        int lastPosition = -1;
        while (cur != '#' && cur != '\n' && cur != '\r') {
            if (cur == '|') {
                isCovered = 0;
            } else if (isCovered) {
                covered++;
                char value = cur;
                char suit = fgetc(inputFile);
                if (suit == 'c') {
                    checkForDoubles(value, suit, fullDeck.clubs);
                } else if (suit == 'd') {
                    checkForDoubles(value, suit, fullDeck.diamonds);
                } else if (suit == 'h') {
                    checkForDoubles(value, suit, fullDeck.hearts);
                } else if (suit == 's') {
                    checkForDoubles(value, suit, fullDeck.spades);
                }

                addToTableau(column, suit, value, isCovered);
            } else {
                char value = cur;
                char suit = fgetc(inputFile);

                if (lastPosition != valueToIndex(value) + 1 && lastPosition != -1) {
                    lineError("a lower valued card");
                }

                if (suit == 'h' || suit == 'd') {
                    if (lastSuit != '0' && lastSuit != 'c' && lastSuit != 's') {
                        lineError("a club or spade card");
                    }
                } else if (suit == 'c' || suit == 's') {
                    if (lastSuit != '0' && lastSuit != 'h' && lastSuit != 'd') {
                        lineError("a heart or diamond card");
                    }
                }

                lastPosition = valueToIndex(value);
                lastSuit = suit;

                if (suit == 'c') {
                    checkForDoubles(value, suit, fullDeck.clubs);
                } else if (suit == 'd') {
                    checkForDoubles(value, suit, fullDeck.diamonds);
                } else if (suit == 'h') {
                    checkForDoubles(value, suit, fullDeck.hearts);
                } else if (suit == 's') {
                    checkForDoubles(value, suit, fullDeck.spades);
                }

                addToTableau(column, suit, value, isCovered);

            }
            cur = getNextValuableChar(inputFile);
        }
        ungetc(cur, inputFile);
        consumeWhiteSpaceAndComments(inputFile);
        cur = fgetc(inputFile);

    }
    ungetc(cur, inputFile);

    return covered;
}

/**
* @author Blake Dunn
* Checks if the card with value cardValue and suit cardSuit has already been isCovered
*/
void checkForDoubles(char cardValue, char cardSuit, struct card suit[]) {
    int cardIndexInDeck = valueToIndex(cardValue);

    if (suit[cardIndexInDeck].isCovered) {
        if (inputFileIsValid) {
            fprintf(stderr, "Duplicated cards: ");
            inputFileIsValid = 0;
        }
        fprintf(stderr, "%c%c ", cardValue, cardSuit);
    } else {
        suit[cardIndexInDeck].isCovered = 1;
    }
}

/**
 * @author Eva Kohl
 * helper method that trims only spaces
 */
char getNextValuableChar(FILE *inputFile) {
    char currentChar = fgetc(inputFile);
    while (' ' == currentChar || 'd' == currentChar || 'h' == currentChar || 's' == currentChar || 'c' == currentChar) {
        currentChar = fgetc(inputFile);
    }
    return currentChar;
}

/**
 * @author Eva Kohl
 * @param inputFile
 *
 * This method takes in an input file and processes the stock portion of the text.
 */
struct stockData processStock(FILE *inputFile) {
    //cur is here in order to check the value of the chars that currently being cycled through
    char cur;
    int inStock = 0;
    struct stockData stock;
    stock.stockCard = 0;
    stock.wasteCard = 0;
    consumeWhiteSpaceAndComments(inputFile);

    char header[7];
    fgets(header, 7, inputFile);
    if (strcmp(header, "STOCK:")) {
        printf("%s\n", header);
        lineError("STOCK:");
    }

    consumeWhiteSpaceAndComments(inputFile);
    while ((cur = getNextValuableChar(inputFile)) != 'M') {
        if (cur == '\n') {
            //Do nothing
        } else if (cur == '#') {
            ungetc(cur, inputFile);
        } else if (cur == '|') {
            inStock = 1;
        } else if (inStock) {
            stock.stockCard++;

            char value = cur;
            char suit = fgetc(inputFile);
            if (suit == 'c') {
                checkForDoubles(value, suit, fullDeck.clubs);
                offerToFront(&gameState.stockHead, 'c', value, 0);
            } else if (suit == 'd') {
                checkForDoubles(value, suit, fullDeck.diamonds);
                offerToFront(&gameState.stockHead, 'd', value, 0);
            } else if (suit == 'h') {
                checkForDoubles(value, suit, fullDeck.hearts);
                offerToFront(&gameState.stockHead, 'h', value, 0);
            } else if (suit == 's') {
                checkForDoubles(value, suit, fullDeck.spades);
                offerToFront(&gameState.stockHead, 's', value, 0);
            }
        } else {
            stock.wasteCard++;

            char value = cur;
            char suit = fgetc(inputFile);
            if (suit == 'c') {
                checkForDoubles(value, suit, fullDeck.clubs);
                offerWithCover(&gameState.wasteHead, 'c', value, 0);
            } else if (suit == 'd') {
                checkForDoubles(value, suit, fullDeck.diamonds);
                offerWithCover(&gameState.wasteHead, 'd', value, 0);
            } else if (suit == 'h') {
                checkForDoubles(value, suit, fullDeck.hearts);
                offerWithCover(&gameState.wasteHead, 'h', value, 0);
            } else if (suit == 's') {
                checkForDoubles(value, suit, fullDeck.spades);
                offerWithCover(&gameState.wasteHead, 's', value, 0);
            }
        }
        consumeWhiteSpaceAndComments(inputFile);
    }
    ungetc(cur, inputFile);
    return stock;
}

/**
* @author Blake Dunn
* Ignored for part1 of the assignment
*/
void processMoves(FILE *inputFile) {
    consumeWhiteSpaceAndComments(inputFile);

    char header[7];
    fgets(header, 7, inputFile);
    if (strcmp(header, "MOVES:")) {
        lineError("MOVES:");
    }

    consumeWhiteSpaceAndComments(inputFile);
}

/**
* @author Blake Dunn
* Consumes whitespace characters in a FILE*
*/
void consumeWhiteSpaceAndComments(FILE *inputFile) {

    int curChar;

    for (;;) {

        curChar = fgetc(inputFile);

        if (' ' == curChar) continue;

        if ('\t' == curChar) continue;
        if ('\n' == curChar) {
            readLines++;
            continue;
        }
        if ('\r' == curChar) {
            readLines++;
            continue;
        }
        if ('#' == curChar) {
            while ((curChar = fgetc(inputFile)) != '\n');

            readLines++;
            continue;
        }

        if (EOF == curChar) return;
        break;
    }
    ungetc(curChar, inputFile);

}

/**
* @author Blake Dunn
* Throws an error and exits the program if a formatting error happens
*/
void lineError(const char* expectedText) {
    fprintf(stderr, "Error near line %d: expecting '%s'\n", readLines, expectedText);
    exit(-1);
}

/**
 * @author Blake Dunn
 * Takes a card value and converts it to the index of the array that would store it using the following mapping:
 * A 2 3 4 5 6 7 8 9 T  J   Q   K
 * 0 1 2 3 4 5 6 7 8 9  10  11  12 
 */
int valueToIndex(char cardValue) {
    int index;
    if (cardValue == 'A') {
        index = 0;
    } else if (cardValue == 'T') {
        index = 9;
    } else if (cardValue == 'J') {
        index = 10;
    } else if (cardValue == 'Q') {
        index = 11;
    } else if (cardValue == 'K') {
        index = 12;
    } else {
        index = cardValue - '0' - 1;
    }
    return index;
}

/**
 * @author Blake Dunn
 * Takes a card index is an array and converts it to the val the card has using the following mapping:
 * A 2 3 4 5 6 7 8 9 T  J   Q   K
 * 0 1 2 3 4 5 6 7 8 9  10  11  12
 */
char indexToValue(int index) {
    char cardValue;
    switch (index) {
        case 0:
            cardValue = 'A';
            break;
        case 9:
            cardValue = 'T';
            break;
        case 10:
            cardValue = 'J';
            break;
        case 11:
            cardValue = 'Q';
            break;
        case 12:
            cardValue = 'K';
            break;
        default:
            cardValue = index + 1 + '0';
            break;
    }
    return cardValue;
}
