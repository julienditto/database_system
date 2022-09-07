// Binary tree abstract base class
// Created by A. Student
// Modified by: Ekaterina Kazantseva

/*
BinaryTree ADT is used to create two trees in our project. One tree is to contain BoardGame objects sorted by titles and the other tree is to contain BoardGame objects to be sorted by barcodes.
*/
#ifndef _BINARY_TREE
#define _BINARY_TREE

#include "BinaryNode.h"
#include "Queue.h"
#include <iomanip>
#include <fstream>

template<class ItemType>
class BinaryTree
{
protected:
    BinaryNode<ItemType>* rootPtr;        // ptr to root node
    int count;                            // number of nodes in tree
    
public:
    // "admin" functions
    BinaryTree() {rootPtr = 0; count = 0;}
    BinaryTree(const BinaryTree<ItemType> & tree){ }
    virtual ~BinaryTree() { ofstream outFile("output.txt"); destroyTree(rootPtr, outFile); outFile.close();}
    
    // common functions for all binary trees
    bool isEmpty() const    {return count == 0;}
    int size() const        {return count;}
    void clear()            {destroyTree(rootPtr); rootPtr = 0; count = 0;}
    void preOrder() const {ofstream outFile("output.txt"); _preorder(rootPtr, outFile);}
    void inOrder() const  {_inorder(rootPtr);}
    void postOrder() const{_postorder(rootPtr);}
    void breadthFirst() const;
    void print(string print(ItemType i)) const { int i = 1; _print(i, this->rootPtr, print);}
    
    // abstract functions to be implemented by derived class
    virtual bool insert(const ItemType & newData, int compare(ItemType i1, ItemType i2)) = 0;
    virtual bool remove(const ItemType & data, int compare(ItemType i1, ItemType i2)) = 0;
    virtual bool getEntry(const ItemType & anEntry, ItemType & returnedItem, int compare(ItemType i1, ItemType i2)) const = 0;
    
private:
    // delete all nodes from the tree
    void destroyTree(BinaryNode<ItemType>* nodePtr, ofstream &out);
    
    // internal traverse
    void _preorder(BinaryNode<ItemType>* nodePtr, ofstream &out) const;
    void _inorder(BinaryNode<ItemType>* nodePtr) const;
    void _postorder(BinaryNode<ItemType>* nodePtr) const;
    void _print(int i, BinaryNode<ItemType>* nodePtr, string print(ItemType i)) const;
};

//Destroy the entire tree
template<class ItemType>
void BinaryTree<ItemType>::destroyTree(BinaryNode<ItemType>* nodePtr, ofstream &out)
{
    if(nodePtr) {
        destroyTree(nodePtr->getLeftPtr(), out);
        destroyTree(nodePtr->getRightPtr(), out);
        out << *(nodePtr->getItem());
        delete nodePtr;
    }
}

//Preorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_preorder(BinaryNode<ItemType>* nodePtr, ofstream &out) const
{
    if (nodePtr)
    {
        ItemType item = nodePtr->getItem();
        out << *item;
        _preorder(nodePtr->getLeftPtr(), out);
        _preorder(nodePtr->getRightPtr(), out);
    }
}

//Inorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_inorder(BinaryNode<ItemType>* nodePtr) const
{
    if(nodePtr) {
        _inorder(nodePtr->getLeftPtr());
        ItemType item = nodePtr->getItem();
        cout << *item;
        _inorder(nodePtr->getRightPtr());
    }
}

//Postorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_postorder(BinaryNode<ItemType>* nodePtr) const
{
    if(nodePtr) {
        _postorder(nodePtr->getLeftPtr());
        _postorder(nodePtr->getRightPtr());
        ItemType item = nodePtr->getItem();
        cout << *item;
    }
}
//Breadth-first traversal
template<class ItemType>
void BinaryTree<ItemType>::breadthFirst() const {
    if (rootPtr == 0)
        return;
    Queue<BinaryNode<ItemType> *> q;
    q.enqueue(rootPtr);
    BinaryNode<ItemType> *curr;
    ItemType item;
    while (!q.isEmpty()) {
        q.dequeue(curr);
        item = curr->getItem();
        cout << *item;
        if (curr->getLeftPtr() != 0) {
            q.enqueue(curr->getLeftPtr());
        }
        if (curr->getRightPtr() != 0) {
            q.enqueue(curr->getRightPtr());
        }
    }
}
//print BT as an indented list
template <class ItemType>
void BinaryTree<ItemType>::_print(int i, BinaryNode<ItemType>* nodePtr, string print(ItemType i)) const {
    cout << right << setw(5*i) << "Level " << i << ". " << print(nodePtr->getItem()) << endl;
    i = i + 1;
    if (nodePtr->getRightPtr() != 0)
        _print(i, nodePtr->getRightPtr(), print);
    if (nodePtr->getLeftPtr() != 0)
        _print(i, nodePtr->getLeftPtr(), print);
}
#endif
