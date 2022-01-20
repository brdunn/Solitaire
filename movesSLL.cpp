#include "movesSLL.hpp"
#include <iostream>

using namespace std;

/**
 * @author Blake Dunn
 * Constructs an empty movesSLL
 */
movesSLL::movesSLL() {
    head = new struct movesSLLNode;
    size = 0;
}

/**
 * @author Blake Dunn
 * Copy constructor that makes deep copies of the contents of the class
 */
movesSLL::movesSLL(const movesSLL& src) {
    head = new struct movesSLLNode;
    size = 0;

    struct movesSLLNode* cur = src.head->next;
    while (cur) {
        offer(cur->move);
        cur = cur->next;
    }
}

/**
 * @author Blake Dunn
 * destorys a movesSLL and frees all associated memory
 */
movesSLL::~movesSLL() {
    struct movesSLLNode* cur = head;
    struct movesSLLNode* next;
    while (cur) {
        next = cur->next;
        delete cur;
        cur = next;
    }
}

/**
 * @author Blake Dunn
 * adds a move to the end of a movesSLL
 */
bool movesSLL::offer(string move) {
    struct movesSLLNode* cur = head;
    while (cur->next) cur = cur->next;
    struct movesSLLNode* newNode = new struct movesSLLNode;
    newNode->move = move;
    newNode->next = 0;
    cur->next = newNode;
    size++;
    return true;
}

/**
 * @author Eva Kohl
 * Gets the last move from from the list but doesn't change the list
 */
string movesSLL::peekLast() {
    struct movesSLLNode *cur = head;
    while (cur->next) cur = cur->next;
    return cur->move;
}

/**
 *
 * @author Blake Dunn
 * Peeks returns the move at index index of the list
 * No error checking
 */
string movesSLL::peek(int index) {
    int curIndex = 0;
    struct movesSLLNode *cur = head->next;
    while (cur && curIndex < index) {
        cur = cur->next;
        curIndex++;
    }
    return cur->move;
}

/**
 * @author Blake Dunn
 * overrides the << operator to print the list one move per line
 */
ostream& operator<<(ostream& out, movesSLL num) {
    struct movesSLLNode* cur = num.head->next;
    while (cur) {
        out << cur->move << "\n";
        cur = cur->next;
    }
    return out;
}

/**
 * @author Blake Dunn
 * Returns the size of the movesSLL
 */
int movesSLL::getSize() {
    return size;
}