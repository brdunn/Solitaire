#ifndef MOVESSLL_HPP
#define MOVESSLL_HPP

#include <iostream>
#include <string>

using namespace std;

struct movesSLLNode {
    string move;
    struct movesSLLNode* next = 0;
};

class movesSLL {
    private:
        struct movesSLLNode* head;
        int size;
        
    public:
        movesSLL();
        movesSLL(const movesSLL& src);
        ~movesSLL();

        bool offer(string move);
        string peek(int index);
        string peekLast();
        friend ostream& operator<<(ostream& os, movesSLL num);
        int getSize();
};

#endif