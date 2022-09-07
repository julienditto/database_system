// Binary Search Tree ADT
// Created by A. Student
// Modified by: Ekaterina Kazantseva

#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE
#include "BinaryTree.h"

/*
 BinarySearchTree ADT is to be used to apply search, insert, remove, and delete on binary tree (in our case the 2 trees, titles tree and barcode tree)
 */

template<class ItemType>
class BinarySearchTree : public BinaryTree<ItemType>
{
private:
    // internal insert node: insert newNode in nodePtr subtree
    BinaryNode<ItemType>* _insert(BinaryNode<ItemType>* nodePtr, BinaryNode<ItemType>* newNode, int compare(ItemType i1, ItemType i2));
    
    // internal remove node: locate and delete target node under nodePtr subtree
    BinaryNode<ItemType>* _remove(BinaryNode<ItemType>* nodePtr, const ItemType target, bool & success, int compare(ItemType i1, ItemType i2));
    
    // delete target node from tree, called by internal remove node
    BinaryNode<ItemType>* deleteNode(BinaryNode<ItemType>* targetNodePtr);
    // remove the leftmost node in the left subtree of nodePtr
    BinaryNode<ItemType>* removeLeftmostNode(BinaryNode<ItemType>* nodePtr, ItemType & successor);
    
    // search for target node
    BinaryNode<ItemType>* findNode(BinaryNode<ItemType>* nodePtr, const ItemType & target, int compare(ItemType i1, ItemType i2)) const;
    
public:
    // insert a node at the correct location
    bool insert(const ItemType & newEntry, int compare(ItemType i1, ItemType i2));
    // remove a node if found
    bool remove(const ItemType & anEntry, int compare(ItemType i1, ItemType i2));
    // find a target node
    bool getEntry(const ItemType & target, ItemType & returnedItem, int compare(ItemType i1, ItemType i2)) const;
    
};


///////////////////////// public function definitions ///////////////////////////
//Inserting items within a tree
template<class ItemType>
bool BinarySearchTree<ItemType>::insert(const ItemType & newEntry, int compare(ItemType i1, ItemType i2))
{
    BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);
    this->rootPtr = _insert(this->rootPtr, newNodePtr, compare);
    this->count = this->count + 1;
    return true;
}

//Removing items within a tree
template<class ItemType>
bool BinarySearchTree<ItemType>::remove(const ItemType & target, int compare(ItemType i1, ItemType i2))
{
    bool isSuccessful = false;
    this->rootPtr = _remove(this->rootPtr, target, isSuccessful, compare);
    return isSuccessful;
}

//Finding entries within a tree
template<class ItemType>
bool BinarySearchTree<ItemType>::getEntry(const ItemType& anEntry, ItemType & returnedItem, int compare(ItemType i1, ItemType i2)) const
{
    if (findNode(this->rootPtr, anEntry, compare) != 0) {
        returnedItem = findNode(this->rootPtr, anEntry, compare)->getItem();
        if (compare(returnedItem, anEntry) == 0)
            return true;
    }
    return false;
}

//////////////////////////// private functions ////////////////////////////////////////////

//Implementation of the insert operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_insert(BinaryNode<ItemType>* nodePtr,
                                                          BinaryNode<ItemType>* newNodePtr, int compare(ItemType i1, ItemType i2))
{
    if (nodePtr == 0) {
        nodePtr = newNodePtr;
    }
    else {
        if (compare(newNodePtr->getItem(), nodePtr->getItem()) == -1)
            nodePtr->setLeftPtr(_insert(nodePtr->getLeftPtr(), newNodePtr, compare));
        else if (compare(newNodePtr->getItem(), nodePtr->getItem()) == 1)
            nodePtr->setRightPtr(_insert(nodePtr->getRightPtr(), newNodePtr, compare));
        else {
            nodePtr->setRightPtr(_insert(nodePtr->getRightPtr(), newNodePtr, compare));
            nodePtr->getItem()->incrementDuplicateCount();
        }
    }
    return nodePtr;
}

//Implementation for the search operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::findNode(BinaryNode<ItemType>* nodePtr,
                                                           const ItemType & target, int compare(ItemType i1, ItemType i2)) const
{
    BinaryNode<ItemType>* pWalk = nodePtr;
    while (pWalk != 0) {
        if (compare(pWalk->getItem(),target) == 0)
            return pWalk;
        else {
            if (compare(pWalk->getItem(), target) == -1)
                pWalk = pWalk->getRightPtr();
            else if (compare(pWalk->getItem(), target) == 1)
                pWalk = pWalk->getLeftPtr();
        }
    }
    return pWalk;
}


//Implementation of the remove operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_remove(BinaryNode<ItemType>* nodePtr,
                                                          const ItemType target,
                                                          bool & success, int compare(ItemType i1, ItemType i2))

{
    if (nodePtr == 0)
    {
        success = false;
        return 0;
    }
    if (compare(nodePtr->getItem(), target) == 0) {
        if (nodePtr->getItem()->getDuplicateCount() != 0) {
            nodePtr->getItem()->setDuplicateCount(0);
        }
        nodePtr = deleteNode(nodePtr);
        success = true;
    }
    else if (compare(nodePtr->getItem(), target) == -1) {
        nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), target, success, compare));
        if (nodePtr->getItem()->getName() == target->getName()) {
            nodePtr->getItem()->decrementDuplicateCount();
        }
    }
    else
    {
        nodePtr->setLeftPtr(_remove(nodePtr->getLeftPtr(), target, success, compare));
        
    }
    return nodePtr;
}
//Implementation of the delete operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::deleteNode(BinaryNode<ItemType>* nodePtr)
{
    if (nodePtr->isLeaf())
    {
        delete nodePtr;
        nodePtr = 0;
        return nodePtr;
    }
    else if (nodePtr->getLeftPtr() == 0)
    {
        BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getRightPtr();
        delete nodePtr;
        nodePtr = 0;
        return nodeToConnectPtr;
    }
    else if (nodePtr->getRightPtr() == 0)
    {
        BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getLeftPtr();
        delete nodePtr;
        nodePtr = 0;
        return nodeToConnectPtr;
    }
    else
    {
        ItemType newNodeValue;
        nodePtr->setRightPtr(removeLeftmostNode(nodePtr->getRightPtr(), newNodeValue));
        nodePtr->setItem(newNodeValue);
        return nodePtr;
    }
}

//Implementation to remove the left leaf
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::removeLeftmostNode(BinaryNode<ItemType>* nodePtr,
                                                                     ItemType & successor)
{
    if (nodePtr->getLeftPtr() == 0)
    {
        successor = nodePtr->getItem();
        return deleteNode(nodePtr);
    }
    else
    {
        nodePtr->setLeftPtr(removeLeftmostNode(nodePtr->getLeftPtr(), successor));
        return nodePtr;
    }
}

#endif

