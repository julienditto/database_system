// Node for a binary tree
// Created by A. Student
// Modified by: Ekaterina Kazantseva

#ifndef _BINARY_NODE
#define _BINARY_NODE

/*
 A BinaryNode object is used to build a binary tree. It contains a BoardGame object, and pointers to left and right childen.
 */

template<class ItemType>
class BinaryNode
{
private:
    ItemType              item;         // Data portion
    BinaryNode<ItemType>* leftPtr;        // Pointer to left child
    BinaryNode<ItemType>* rightPtr;        // Pointer to right child
    
public:
    // constructors
    BinaryNode(const ItemType & anItem)    {item = anItem; leftPtr = 0; rightPtr = 0;}
    BinaryNode(const ItemType & anItem,
               BinaryNode<ItemType>* left,
               BinaryNode<ItemType>* right) {item = anItem; leftPtr = left; rightPtr = right;}
    // setters
    void setItem(const ItemType & anItem) {item = anItem;}
    void setLeftPtr(BinaryNode<ItemType>* left) {leftPtr = left;}
    void setRightPtr(BinaryNode<ItemType>* right) {rightPtr = right;}
    // getters
    ItemType getItem() const     {return item;}
    BinaryNode<ItemType>* getLeftPtr() const  {return leftPtr;}
    BinaryNode<ItemType>* getRightPtr() const {return rightPtr;}
    bool isLeaf() const {return (leftPtr == 0 && rightPtr == 0);}
    
};

#endif
