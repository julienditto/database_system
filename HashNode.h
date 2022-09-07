#ifndef HASH_NODE_H
#define HASH_NODE_H
#include "BoardGame.h"
#include<string>

class HashNode
{
private:
    HashNode *next;
    string key;
    BoardGame *boardGameInfo;

public:
    // Setters
    BoardGame* getBoardGameInfo() { return boardGameInfo; }
    HashNode* getNext() { return next; }
    string getKey() { return key; }

    friend class HashTable;

};

#endif
