#ifndef WINNABLE_HPP
#define WINNABLE_HPP

#include "movesSLL.hpp"

bool attemptMove(struct gameState& curGameState, movesSLL completedMoves);
bool isInWinningState(struct gameState& curGameState);
bool canMoveTableauToFoundation(struct gameState& curGameState, int tableauColumn);
bool canMoveTableauToTableau(struct gameState& curGameState, int srcColumn, int destColumn);
bool canTurnCardInStock(struct gameState& curGameState);
bool canResetStock(struct gameState& curGameState);
bool canMoveWasteToFoundation(struct gameState& curGameState);
void turnCardInStock(struct gameState& curGameState);
void wasteToTableau(struct gameState& curGameState,int destColumn);
void tableauToFoundation(struct gameState& curGameState, int srcColumn);
void wasteToFoundations(struct gameState& curGameState);
void resetStock(struct gameState& curGameState);
struct singlyLinkedListNode* colNumToTableauPile(struct gameState& curGameState, int colNum);
bool canMoveWasteToTableau(struct gameState& curGameState, int destColumn);
bool isCardSmaller(char bottomCard, char cardToBePlaced);
bool isSuitColorDifferent(char suitOne, char suitTwo);
bool checkKing(char value, char isEmpty);
void tableauToTableau(struct gameState& curGameState, int srcColumn, int destColumn);
void deepCopySSLStruct(struct singlyLinkedListNode* src, struct singlyLinkedListNode* dest);
void deepCopyGameStateStruct(struct gameState* src, struct gameState* dest);
void destroyGameState(struct gameState* curGameState);
bool isSafeToMoveToFoundations(struct gameState& curGameState, char suit, char value, bool isFromWaste);

#endif
