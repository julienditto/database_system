// HashTable Class
// Created by: Chris Yan
// Modified by: Ekaterina Kazantseva

#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <string>
#include "BoardGame.h"
#include "HashNode.h"
using namespace std;

/*
 HashTable ADT takes a unique key from an object and creates an address (index) for the object and stores the object
 in an array using the generated index. The object can then be found at a constant time complexity by using this unique key. In our case, the key we used for our object is a barcode.
 */

class HashTable
{
private:
    int size;
    int seed;
    int currSize;
    int collisionNum;
    int hash(string key);
    int hashRandSeed(string key);
    HashNode **data;
    
public:
    HashTable(int s);
    ~HashTable();
    void insertHash(string key, BoardGame *elem);
    bool searchHash(string key, BoardGame* &elem);
    void deleteElem(string key);
    void deleteHash();
    void print(); //added print function
    double getLoadFactor();
    int getCollisionNum();
    int getCurrSize() { return currSize;}
    int getSize () {return size;}
    HashNode **getData() {return data;} //added getter for data
    HashTable &operator=(const HashTable &right); //added overloaded operator
};

// Constructor
HashTable::HashTable(int s)
{
    this->seed = 31; //used to calculate hashValue in order to minimize collision
    this->size = s;
    this->currSize = 0;
    collisionNum = 0;
    this->data = new HashNode*[size];
    for(int i = 0; i < size; i++)
        this->data[i] = nullptr;
}

// Destructor calls deleteHash to clean up
HashTable::~HashTable()
{
    for(unsigned int i = 0; i < this->size; i++)
    {
        HashNode* tmpData = data[i];
        
        HashNode* tmp;
        while(tmpData != nullptr)
        {
            tmp = tmpData;
            tmpData = tmpData->next;
            delete tmp->boardGameInfo;
            delete tmp;
        }
        data[i] = nullptr;
    }
    currSize = 0;
    collisionNum = 0;
    delete [] data;
}

// Load factor
double HashTable::getLoadFactor() {return (double)currSize / (double)size * 100;}

// Collision number
int HashTable::getCollisionNum() {return collisionNum;}


// Inserts an item into the hash table
// Created by Chris
// Modified by Ekaterina
void HashTable::insertHash(string key, BoardGame* elem)
{
    int hashValue = hashRandSeed(key);
    HashNode* node = new HashNode;
    node->key = key;
    node->boardGameInfo = elem;
    node->next = nullptr;
    
    if(this->data[hashValue] == nullptr)
    {
        data[hashValue] = node;
        currSize++;
    }
    else
    {
        HashNode* prevNode = data[hashValue];
        HashNode* tmpNode = prevNode->next;
        while(prevNode != nullptr)
        {
            if(prevNode->key == key)
                break;
            else if (tmpNode == nullptr)
            {
                collisionNum++;
                prevNode->next = node;
                break;
            }
            else if (tmpNode != nullptr)
            {
                collisionNum++;
                tmpNode = tmpNode->next;
                prevNode = prevNode->next;
            }
        }
    }
}

// Search
bool HashTable::searchHash(string key, BoardGame* &elem){
    int hashValue = hashRandSeed(key);
    if(this->data[hashValue] != nullptr) {
        HashNode* tmpNode = data[hashValue];
        while(tmpNode != nullptr)
        {
            if(tmpNode->key == key)
            {
                break;
            }
            else
                tmpNode = tmpNode->next;
        }
        if(tmpNode == nullptr)
            return false;
        else {
            elem = tmpNode->boardGameInfo;
            return true;
            
        }
        
    }
    return false;
    
}

// Delete entire hash table
void HashTable::deleteHash()
{
    for(unsigned int i = 0; i < this->size; i++)
    {
        HashNode* tmpData = data[i];
        
        HashNode* tmp;
        while(tmpData != nullptr)
        {
            tmp = tmpData;
            tmpData = tmpData->next;
            delete tmp;
        }
        data[i] = nullptr;
    }
    currSize = 0;
    collisionNum = 0;
}

// Hash uses random seed to minimize collision
int HashTable::hashRandSeed(string key)
{
    unsigned long hashValue = 0;
    for(int i = 0; i < key.size(); i++)
        hashValue = hashValue * this->seed + key[i];
    
    return (int)(hashValue % this->size);
}

//////////////////////////////////////////////////////
//-------Created by Ekaterina Kazantseva---------//
/////////////////////////////////////////////////////
/*
 deletes an item from the hash table and rearranges nodes if necesarry (due to chaining method of hash table)
 */
void HashTable::deleteElem(string key)
{
    int hashValue = hashRandSeed(key);

    HashNode* currNode = *(&(data[hashValue]));
    HashNode* nextNode = currNode->next;
    
    if(currNode->key == key)
    {
        if (nextNode == nullptr) {
            currSize--;
        }
        nextNode = currNode;
        data[hashValue] = currNode->next;
        delete nextNode;
    }
    else {
        while(nextNode->key != key) {
            currNode = currNode->next;
            nextNode = nextNode->next;
        }
        if (nextNode->next != nullptr)
            currNode->next = nextNode->next;
        else if (nextNode->next == nullptr)
            currNode->next = nullptr;
        delete nextNode;
    }
    
}

//////////////////////////////////////////////////////
//-------Created by Ekaterina Kazantseva---------//
/////////////////////////////////////////////////////
//Prints unsorted data from hash table
void HashTable::print()
{
    int counter = 1;
    for (int i = 0; i < size; i++)
    {
        if (data[i] != nullptr)
        {
            cout << setw(3) << counter << ". " << *(data[i]->boardGameInfo);
            HashNode* ptr = data[i];
            ptr = ptr->next;
            ++counter;
            while (ptr != nullptr)
            {
                cout << setw(3) <<counter << ". " << *(ptr->boardGameInfo);
                ptr = ptr->next;
                ++counter;
            }
        }
    }
}


//////////////////////////////////////////////////////
//-------Created by Ekaterina Kazantseva---------//
/////////////////////////////////////////////////////
//Overloaded assignment operator to use in rehashing
HashTable& HashTable::operator=(const HashTable& right)
{
    int s = right.size;
    this->deleteHash();
    data = new HashNode*[s];
    size = s;
    collisionNum = 0;
    for(int i = 0; i < size; i++)
        this->data[i] = nullptr;
    for (int i = 0; i < size; i++)
    {
        if (right.data[i] != nullptr)
        {
            this->insertHash(right.data[i]->key, right.data[i]->boardGameInfo);
            HashNode* ptr = right.data[i];
            ptr = ptr->next;
            while (ptr != nullptr)
            {
                this->insertHash(ptr->key, ptr->boardGameInfo);
                ptr = ptr->next;
            }
        }
    }
    return *this;
}

//////////////////////////////////////////////////////
//-------Created by Ekaterina Kazantseva---------//
/////////////////////////////////////////////////////
//Another version of a hash function
int HashTable::hash(string key)
{
    long long int hashKey = 0;
    for (int i = 0; i < key.size(); i++)
    {
        hashKey += pow(stol(key.substr(i, i+1)), 2);
    }
    return (int)(hashKey%size);
}



#endif
