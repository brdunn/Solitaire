#include <stdlib.h>
#include <stdio.h>
#include "singlyLinkedList.h"


/**
 * @author Blake Dunn
 * adds a node to the linked list that is at the memory location referenced by toAdd
 */
void offerWithReference(struct singlyLinkedListNode* head, struct singlyLinkedListNode* toAdd) {
    struct singlyLinkedListNode* cur = head;
    while (cur->next) cur = cur->next;
    cur->next = toAdd;
    while(toAdd->next)toAdd = toAdd->next;
    toAdd->next = 0;
}

/**
 * @author Blake Dunn
 * adds a node to the front of the linked list
 */
void offerToFront(struct singlyLinkedListNode* head, char suit, char value, int isCovered) {
    struct singlyLinkedListNode* temp = (struct singlyLinkedListNode*) malloc(sizeof(struct singlyLinkedListNode)); 
    temp->data.suit = suit;
    temp->data.value = value;
    temp->next = head->next;
    head->next = temp;
}

/**
 * @author Blake Dunn
 * @author Eva Kohl
 * adds a node to the front of the linked list
 */
void offerWithCover(struct singlyLinkedListNode* head, char suit, char value, int isCovered) {
    struct singlyLinkedListNode* cur = head;
    while (cur->next) cur = cur->next;
    struct singlyLinkedListNode* temp = (struct singlyLinkedListNode*) malloc(sizeof(struct singlyLinkedListNode));
    temp->data.suit = suit;
    temp->data.value = value;
    temp->data.isCovered = isCovered;
    temp->next = 0;
    cur->next = temp;
}

/**
 * @author Blake Dunn
 * removes the last element of the linked list and returns it.  If there are no items in the list, a 0
 * pointer is returned
 */
struct singlyLinkedListNode* poll(struct singlyLinkedListNode* head) {
    if (!head->next) return 0;
    struct singlyLinkedListNode* cur = head;
    while (cur->next && cur->next->next) cur = cur->next;
    struct singlyLinkedListNode* temp = cur->next;
    cur->next = 0;
    return temp;
}

/**
 * @author Eva Kohl
 * removes the first element of the linked list and returns it.  If there are no items in the list, a 0
 * pointer is returned
 */
struct singlyLinkedListNode* pollFromFront(struct singlyLinkedListNode* head) {
    if (!head->next) return 0;
    struct singlyLinkedListNode* temp =  head->next;
    head->next = temp->next;
    return temp;
}

/**
 * @author Blake Dunn
 * returns the size of the singlylinked list pointed to by the head node
 */
int size(struct singlyLinkedListNode* head) {
    int size = 0;
    struct singlyLinkedListNode* cur = head;
    while (cur->next) {
        cur = cur->next;
        size++;
    }
    return size;
}

/**
 * @author Blake Dunn
 * Returns 1 if the linked list pointed to by head is empty, 0 otherwise
 */
char isEmpty(struct singlyLinkedListNode* head) {
    if (!head->next) return 1;
    return 0;
}

/**
 * @author Blake Dunn
 * returns a pointer to the last element of the linked list
 */
struct singlyLinkedListNode* peekLast(struct singlyLinkedListNode* head) {
    struct singlyLinkedListNode* cur = head;
    while (cur->next) cur = cur->next;
    return cur;
}

/**
 * @author Blake Dunn
 * returns a pointer to the last element of the linked list
 */
struct singlyLinkedListNode* peekFirst(struct singlyLinkedListNode* head) {
    return head->next;
}

/**
 * @author Eva Kohl
 * returns a pointer to the first uncovered element in the list
 */
struct singlyLinkedListNode* peekFirstUncovered(struct singlyLinkedListNode* head) {
    struct singlyLinkedListNode* cur = head->next;
    while (cur->next && cur->data.isCovered) cur = cur->next;
    return cur;
}

/**
 * @author Blake Dunn
 * returns the values at the specified index.  No error checking is done
 */
struct singlyLinkedListNode* peekAtIndex(struct singlyLinkedListNode* head, int index) {
    int curIndex = 0;
    struct singlyLinkedListNode* curNode = head->next;

    while (curIndex < index) {
        curNode = curNode->next;
        curIndex++;
    }
    return curNode;
}

/**
 * @author Eva Kohl
 * removes to item and everything after it in the list, a 0
 * pointer is returned
 */
struct singlyLinkedListNode* pollFromMiddle(struct singlyLinkedListNode* head, struct singlyLinkedListNode* toRemove) {
    if (!head->next) return 0;
    struct singlyLinkedListNode* cur = head;
    while(cur->next && ((cur->next->data.suit != toRemove->data.suit) || (cur->next->data.value != toRemove->data.value)))cur =cur->next;
    cur->next = 0;
    return toRemove;
}

/**
 * @author Blake Dunn
 * checks if the list has any covered cards.  returns 1 if all cards are uncovered.
 * 0 otherwise.
 */
char areAllCardsUncovered(struct singlyLinkedListNode* head) {
    if (isEmpty(head)) return 1;
    struct singlyLinkedListNode* cur = head->next;
    while (cur) {
        if (cur->data.isCovered) return 0;
        cur = cur->next;
    }
    return 1;
}

/**
 * @author Blake Dunn
 * destorys the SLL and frees associated memory
 */
void destory(struct singlyLinkedListNode* head) {
    struct singlyLinkedListNode* cur = head->next;
    while (cur) {
        struct singlyLinkedListNode* next = cur->next;
        free(cur);
        cur = next;
    }
    head->next = 0;
}