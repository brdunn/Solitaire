# Solitaire Development

## Development Setup

### Dependencies
The following are requirements to build and run solitaire:
* git

* gcc

* g++

* make

### Building
```shell
# download source
git clone https://git.linux.iastate.edu/brdunn/cs327evablake.git

#navigate to project directory
cd cs327evablake

# build project
make

# run check for valid input file
./check infile.txt #where inflie.txt is a game configuration file

# read game configuration and run valid moves
./advance infile.txt -x -m N -o outfile.txt #where infile is a game configuration file, N is the number of moves to complete, and outfile.txt is the file to write the result to

# see if a game configuration is winnable
./winnable infile.txt -v -m N -f -c #where infile is a game configurationfile and N is the max number of moves allowed to reach a winning state

# play solatire with a texted-based user interface
./game -f infile.txt #where infile is the name of a file in the excahnge format

OR

./game -s seed -1 -3 -l limit #where seed is a random number generator seed and limit is the number of times the stock can be reset
```

## Testing
```shell
#give execution permissions to the tester
chmod +x autoTest.sh

#run tests
./autoTest.sh
```


## Implementation Credits
### readInputFile.c
* int readInputFile(int argc, char *arg[]) - Blake Dunn & Eva Kohl

* struct rulesData processRules(FILE *inputFile) - Blake Dunn

* void processFoundations(FILE *inputFile) - Blake Dunn

* int processTableau(FILE *inputFile) - Eva Kohl & Blake Dunn

* struct stockData processStock(FILE *inputFile) - Eva Kohl & Blake Dunn

* void processMoves(FILE *inputFile) - Blake Dunn

* void consumeWhiteSpaceAndComments(FILE *) - Blake Dunn

* char getNextValuableChar(FILE *inputFile) - Eva Kohl

* void lineError(char expectedText[]) - Blake Dunn

* void populateDeckWithFoundation(char topCard, char suit) - Blake Dunn

* void checkForDoubles(char cardValue, char cardSuit, struct card suit[]) - Blake Dunn

* void checkForMissingCards() - Blake Dunn

* int suitToIndex(char cardValue) - Blake Dunn

* char indexToSuit(int index) - Blake Dunn

* void printCheckResults() - Eva Kohl Blake Dunn

### check.c
* int main(int argc, char *arg[]) - Eva Kohl Blake Dunn

### advance.c
* int main(int argc, char *argv[]) - Blake Dunn Eva Kohl

* void completeMoves(FILE *inputFile) - Blake Dunn

* void wasteToFoundations() - Blake Dunn

* int checkIfCardIsSmaller(char bottomCard, char cardToBePlaced) - Eva Kohl

* int checkIfSuitColorIsDifferent(char suitOne, char suitTwo) - Eva Kohl

* char checkKing(char value, char isEmpty, int srcColumn, int destColumn) - Eva Kohl Blake Dunn

* void wasteToTableau() - Eva Kohl

* void tableauToFoundation() - Eva Kohl

* void tableauToTableau() - Eva Kohl

* void turnCardinStock() - Blake Dunn

* void resetStock() - Blake Dunn

* void printMovesResults() - Eva Kohl Blake Dunn

* void printPile(FILE* outFile) - Eva Kohl Blake Dunn

* void printFromTab(struct singlyLinkedListNode *tabPile, FILE* outFile) - Eva Kohl

* void printTableauForXSwitch(FILE* outFile) - Eva Kohl

* void moveError(char errorMessage[]) - Blake Dunn

* struct singlyLinkedListNode* colNumToTableauPile(int colNum) - Blake Dunn

### singlyLinkedList.c
* int offerWithReference(struct singlyLinkedListNode* head, struct singlyLinkedListNode* toAdd) - Blake Dunn

* int offerToFront(struct singlyLinkedListNode* head, char suit, char value, int isCovered) - Blake Dunn

* int offerWithCover(struct singlyLinkedListNode* head, char suit, char value, char isCovered) - Eva Kohl Blake Dunn

* struct singlyLinkedListNode* poll(struct singlyLinkedListNode* head) - Blake Dunn

* struct singlyLinkedListNode* pollFromFront(struct singlyLinkedListNode* head) - Eva Kohl

* int size(struct singlyLinkedListNode* head) - Blake Dunn

* char isEmpty(struct singlyLinkedListNode* head) - Blake Dunn

* struct singlyLinkedListNode* peekLast(struct singlyLinkedListNode* head) - Blake Dunn

* struct singlyLinkedListNode* peekFirst(struct singlyLinkedListNode* head) - Blake Dunn

* struct singlyLinkedListNode* peekFirstUncovered(struct singlyLinkedListNode* head) - Eva Kohl

* struct singlyLinkedListNode* pollFromMiddle(struct singlyLinkedListNode* head, struct singlyLinkedListNode* toRemove) - Eva Kohl

* char areAllCardsUncovered(struct singlyLinkedListNode* head) - Blake Dunn

### winnable.cpp
* int main(int argc, char *argv[]) - Blake Dunn

* void attemptMove() - Blake Dunn

* bool isInWinningState() - Blake Dunn

* bool canMoveWasteToTableau(struct gameState& curGameState, int destColumn) - Eva Kohl

* bool canMoveTableauToFoundation(struct gameState& curGameState, int tableauColumn) - Eva Kohl Blake Dunn

* bool canMoveTableauToTableau(struct gameState& curGameState, int srcColumn, int destColumn) - Eva Kohl

* bool canTurnCardInStock(struct gameState& curGameState) - Blake Dunn

* bool canResetStock(struct gameState& curGameState) - Blake Dunn

* bool canMoveWasteToFoundation(struct gameState& curGameState) - Blake Dunn

* void turnCardInStock(struct gameState& curGameState) - Blake Dunn

* void wasteToTableau(struct gameState& curGameState,int destColumn) - Eva Kohl

* void tableauToFoundation(struct gameState& curGameState, int srcColumn) - Eva Kohl

* void tableauToTableau(struct gameState& curGameState, int srcColumn, int destColumn) - Eva Kohl

* void wasteToFoundations(struct gameState& curGameState) - Blake Dunn

* void resetStock(struct gameState& curGameState) - Blake Dunn

* struct singlyLinkedListNode* colNumToTableauPile(struct gameState& curGameState, int colNum) - Blake Dunn

* bool isCardSmaller(char bottomCard, char cardToBePlaced) - Eva Kohl

* bool isSuitColorDifferent(char suitOne, char suitTwo) - Eva Kohl

* bool checkKing(char value, char isEmpty) - Eva Kohl Blake Dunn

* void deepCopySSLStruct(struct singlyLinkedListNode* src, struct singlyLinkedListNode* dest) - Blake Dunn

* void deepCopyGameStateStruct(struct gameState* src, struct gameState* dest) - Blake Dunn

* void destroyGameState(struct gameState* curGameState) - Blake Dunn

* bool isSafeToMoveToFoundations(struct gameState& curGameState, char suit, char value, bool isFromWaste) - Blake Dunn

### movesSLL.cpp
* movesSLL::movesSLL() - Blake Dunn

* movesSLL::movesSLL(const movesSLL& src) - Blake Dunn

* movesSLL::~movesSLL() - Blake Dunn

* bool movesSLL::offer(string move) - Blake Dunn

* string movesSLL::peekLast() - Eva Kohl

* string movesSLL::peek(int index) - Blake Dunn

* ostream& operator<<(ostream& out, movesSLL num) - Blake Dunn

* int movesSLL::getSize() - Blake Dunn

### cSwitchHashTable.cpp

* cSwitchHashTable::cSwitchHashTable(unsigned int size) - Eva Kohl

* void cSwitchHashTable::insertItem(struct gameState key, unsigned int movesTried) - Eva Kohl

* unsigned int cSwitchHashTable::get(struct gameState& key) - Eva Kohl

* unsigned int cSwitchHashTable::numberOfEntries() - Eva Kohl

* bool cSwitchHashTable::ifEntryExists(struct gameState& key) - Eva Kohl

* void cSwitchHashTable::deleteItem(struct gameState key) - Eva Kohl

* void cSwitchHashTable::displayTable() - Eva Kohl

* void cSwitchHashTable::displayOnlyFull() - Eva Kohl

* unsigned int cSwitchHashTable::hash(struct gameState game) - Eva Kohl

## game.cpp

int main(int argc, char *argv[]) - Blake Dunn

void propagateDeckRandomly() - Blake Dunn

void dealCards(card* deck) - Blake Dunn

void shuffle(card* deck) - Blake Dunn

void swapCards(card* card1, card* card2) - Blake Dunn

long chooseRandomlyBetween(long a, long b) - Blake Dunn

double randomNum() - Blake Dunn

void initGame() - Blake Dunn

void drawFullGame() - Blake Dunn

void processUserMoves() - Eva Kohl

void undoLastMove() - Eva Kohl

void restartGame() - Eva Kohl

void gameWonScreen() - Eva Kohl

bool gameHasBeenWon() - Eva Kohl

void completeMove(char scr, char dest, bool calledFromUndoMove) - Eva Kohl

void tableauToWasteForUndo(int scr) - Eva Kohl

void tableauToTableauForUndo(int scr, int dest) - Eva Kohl

void foundationToTableau(char scr, int dest) - Eva Kohl

void foundationToWaste(char scr) - Eva Kohl

void unResetStock() - Eva Kohl

void unFlipCard() - Eva Kohl

void displayBackdrop() - Eva Kohl

void displayInstructions() - Blake Dunn

void displayFoundations() - Blake Dunn

void displayWaste() - Blake Dunn

void displayTableau() - Blake Dunn

void displayString(std::string toDisplay, int startX, int startY, uint16_t bg, 
uint16_t fg) - Blake Dunn

void displayString(std::string toDisplay, int startX, int startY) - Blake Dunn

void displayCard(char suit, char value, int startX, int startY) - Blake Dunn

uint32_t suitToUnicode(char value) - Blake Dunn

void setLastMove(bool flipCard, bool resetStock, char src, char dest, char value) - Eva Kohl

struct singlyLinkedListNode* colNumToTableauPile(struct gameState& curGameState, int 
colNum) - Blake Dunn

void wasteToFoundations() - Blake Dunn

int isCardSmaller(char bottomCard, char cardToBePlaced, int srcColumn, int 
destColumn) - Eva Kohl

int checkIfSuitColorIsDifferent(char suitOne, char suitTwo, int srcColumn, int 
destColumn) - Eva Kohl

char checkKing(char value, char isEmpty, int srcColumn, int destColumn) - Eva Kohl Blake Dunn

void wasteToTableau(int destColumn) - Eva Kohl

void tableauToFoundation(int srcColumn) - Eva Kohl

void tableauToTableau(int srcColumn, int destColumn) - Eva Kohl

void turnCardinStock() - Blake Dunn

void resetStock() - Blake Dunn

void saveStateToFile() - Blake Dunn Eva Kohl

void printTableau(FILE *outFile) - Blake Dunn Eva Kohl

struct singlyLinkedListNode* colNumToTableauPile(int colNum) - Blake Dunn
