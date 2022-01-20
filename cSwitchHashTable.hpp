#ifndef CSWITCHHASHTABLE_HPP
#define CSWITCHHASHTABLE_HPP

#include <string>

#include "singlyLinkedList.h"
#include "readInputFile.h"


using namespace std;



/**
 * @author Eva Kohl
 * BIG idea:
 * want to create a hash that should maps one configuration to a particular bucket
 * this hash table is going to be built specifically to accommodate the gameState obj
 */

class cSwitchHashTable {
    private:
        int numBuckets;
        unsigned int maxMovesTried;
        unsigned int *table;

    public:
        cSwitchHashTable(unsigned int size);
        void insertItem(struct gameState key, unsigned int movesTried);
        void deleteItem(struct gameState key);
        unsigned int get(struct gameState& key);
        void displayTable();
        void displayOnlyFull();
        bool ifEntryExists(struct gameState& key);
        unsigned int numberOfEntries();

    private:
        unsigned int hash(struct gameState game);

};

#endif
