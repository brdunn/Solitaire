#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

struct card {
    //1 discovered, 0 undiscovered
    int isCovered = 0;
    char suit;
    char value;
};

struct singlyLinkedListNode {
    struct card data;
    struct singlyLinkedListNode* next = 0;
};

void offerWithReference(struct singlyLinkedListNode* head, struct singlyLinkedListNode* toAdd);
void offerToFront(struct singlyLinkedListNode* head, char suit, char value, int isCovered);
void offerWithCover(struct singlyLinkedListNode* head, char suit, char value, int isCovered);
struct singlyLinkedListNode* poll(struct singlyLinkedListNode* head);
struct singlyLinkedListNode* pollFromFront(struct singlyLinkedListNode* head);
int size(struct singlyLinkedListNode* head);
char isEmpty(struct singlyLinkedListNode* head);
struct singlyLinkedListNode* peekFirst(struct singlyLinkedListNode* head);
struct singlyLinkedListNode* peekLast(struct singlyLinkedListNode* head);
struct singlyLinkedListNode* peekFirstUncovered(struct singlyLinkedListNode* head);
struct singlyLinkedListNode* peekAtIndex(struct singlyLinkedListNode* head, int index);
struct singlyLinkedListNode* pollFromMiddle(struct singlyLinkedListNode* head, struct singlyLinkedListNode* toRemove);
char areAllCardsUncovered(struct singlyLinkedListNode* head);
void destory(struct singlyLinkedListNode* head);

#endif