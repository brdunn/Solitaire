
#include "cSwitchHashTable.hpp"
#include "singlyLinkedList.h"
#include "readInputFile.h"
#include <iostream>

using namespace std;

/**
 * @author Eva Kohl
 * A hashtable that stores the number of moves attempted from game states.
 * Size of the hashtable is size.
 */
cSwitchHashTable::cSwitchHashTable(unsigned int size) {
    numBuckets = size;
    table = new unsigned int[size];
    for(int i = 0; i < size; i++) {
        table[i]= 0;
    }
}

/**
 * @author Eva Kohl
 * Inserts the number of moves tried to the hashtable at index of the hash of key
 */
void cSwitchHashTable::insertItem(struct gameState key, unsigned int movesTried) {

    int index = hash(key);

    if (table[index] != 0 && table[index] < movesTried) {
        table[index] = movesTried;
    } else if (!table[index]) {
        table[index] = movesTried;
    }
}

/**
 * @author Eva Kohl
 * Returns the item that is located at the hash of key
 */
unsigned int cSwitchHashTable::get(struct gameState& key) {
    int index = hash(key);
    return table[index];
}

/**
 * @author Eva Kohl
 * Returns the number of populated entries in the hash table
 */
unsigned int cSwitchHashTable::numberOfEntries() {
    unsigned int count = 0;
    for (int i = 0; i < numBuckets; i++) {
        if(table[i]) count++;
    }
    return count;
}

/**
 * @author Eva Kohl
 * Returns true if an entry exists at the index that is equal to the hash of key
 */
bool cSwitchHashTable::ifEntryExists(struct gameState& key) {
    int index = hash(key);
    if (!table[index]) {
        return false;
    }
    return true;
}

/**
 * @author Eva Kohl
 * removes an enty in the hashtable that key hashes to
 */
void cSwitchHashTable::deleteItem(struct gameState key) {
    int index = hash(key);
    table[index] = 0;
}

/**
 * @author Eva Kohl
 * prints out the hashtable to cout
 */
void cSwitchHashTable::displayTable() {
    for (int i = 0; i < numBuckets; i++) {
        cout << table[i] <<"\n";
    }
}

/**
 * @author Eva Kohl
 * prints out only the populated buckets in the hashtable to cout
 */
void cSwitchHashTable::displayOnlyFull() {
    for (int i = 0; i < numBuckets; i++) {
        if(table[i]) cout << table[i] <<"\n";
    }
}

/**
 * @author Eva Kohl
 * Hashes a game configuration into a key value for the hash table
 * This hash design is based off the idea that the hash table is at least 10,000,000
 * buckets large. This means that each piece of relevant game configuration
 * information will have it's own location in the number .
 */
unsigned int cSwitchHashTable::hash(struct gameState game) {


    unsigned int coveredCount7 =0;
    unsigned int coveredCount6 =0;
    unsigned int coveredCount5 =0;
    unsigned int coveredCount4 =0;
    unsigned int coveredCount3 =0;
    unsigned int coveredCount2 =0;

    unsigned int wasteSize = (size(&game.wasteHead) % 10) * 1000000;
    unsigned int stockSize = (size(&game.stockHead) % 10) * 100000;

    if(!isEmpty(&game.tableauPile7))coveredCount7 = (size(&game.tableauPile7) - size(peekFirstUncovered(&game.tableauPile7))) * 10000;

    if(!isEmpty(&game.tableauPile6)) coveredCount6 = (size(&game.tableauPile6) - size(peekFirstUncovered(&game.tableauPile6))) * 1000;

    if(!isEmpty(&game.tableauPile5)) coveredCount5 = (size(&game.tableauPile5) - size(peekFirstUncovered(&game.tableauPile5))) * 100;

    if(!isEmpty(&game.tableauPile4)) coveredCount4 = (size(&game.tableauPile4) - size(peekFirstUncovered(&game.tableauPile4))) * 10;

    if(!isEmpty(&game.tableauPile3)) coveredCount3 = (size(&game.tableauPile3) - size(peekFirstUncovered(&game.tableauPile3)));

    if(!isEmpty(&game.tableauPile2)) coveredCount2 = (size(&game.tableauPile2) - size(peekFirstUncovered(&game.tableauPile2)));

    unsigned int twoAndThree = coveredCount3 + coveredCount2 * 5;

    return ((wasteSize + stockSize + coveredCount7 +coveredCount6+ coveredCount5 + coveredCount4 + twoAndThree) %
           numBuckets);
}

