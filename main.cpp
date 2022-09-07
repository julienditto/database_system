// Board Game Data Manager
// Created by: Julien Ditto
// Modified by: Ekaterina Kazantseva, Jerry Chan

#include <iostream>
#include <fstream>
#include "BoardGame.h"
#include <cmath>
#include <string>
#include "HashTable.h"
#include "BinarySearchTree.h"
#include "StackADT.h"
using namespace std;

// Constants for the menu choices
const char INSERT_CHOICE = 'A',
DELETE_CHOICE  = 'B',
UNDO_DELETE_CHOICE = 'C',
SEARCH_MANAGER_CHOICE = 'D',
LIST_MANAGER_CHOICE = 'E',
WRITE_FILE_CHOICE = 'F',
STATISTICS_CHOICE = 'G',
QUIT_CHOICE = 'H',
SHOW_HELP_MENU_CHOICE = 'I';

// Function prototypes
void buildFromFile(string filename, BinarySearchTree<BoardGame*> &treeA, BinarySearchTree<BoardGame*> &treeB, HashTable &t, BoardGame *gamePtr);
void insertManager(BinarySearchTree<BoardGame*> &treeA, BinarySearchTree<BoardGame*> &treeB, HashTable &hashTable, BoardGame *insertGamePtr);
void deleteManager(BinarySearchTree<BoardGame*> &treeA, BinarySearchTree<BoardGame*> &treeB, HashTable &hashTable, Stack<BoardGame*>& deleteStack, BoardGame &dummyGame);
void searchManager(BinarySearchTree<BoardGame*> &treeA, BinarySearchTree<BoardGame*> &treeB, HashTable &hashTable, BoardGame &dummyGame, string searchChoice);
bool parseBarcode(BinarySearchTree<BoardGame*> &treeB, BoardGame* gamePtr);
void rehash(HashTable &t);
bool undoDelete(BinarySearchTree<BoardGame*>& treeA, BinarySearchTree<BoardGame*>& treeB, HashTable& hashTable, Stack<BoardGame*>& deleteStack);
void listManager (BinarySearchTree<BoardGame*>& treeA, BinarySearchTree<BoardGame*>& treeB, HashTable& hashTable, string display);

void getValue (string &line, string &n);
void findTabs(string &l);
int hashSize (string filename);
bool isPrime(int n);
int getNextPrime(int n);
string printName (BoardGame* b);
string printBarCode (BoardGame* b);
int compareByBarCode(BoardGame* b1, BoardGame* b2);
int compareByName(BoardGame* b1, BoardGame* b2);
int compareByNameAndDuplicateCount(BoardGame* b1, BoardGame* b2);
int compareByNameAndBarCode(BoardGame* b1, BoardGame* b2);
void intro();
char menu();

//created by Julien Ditto
int main()
{
    HashTable hashTable (hashSize("board_games.txt"));
    BinarySearchTree<BoardGame*> namesTree;
    BinarySearchTree<BoardGame*> barCodeTree;
    Stack<BoardGame*> deleteStack;
    BoardGame *gamePtr = nullptr;
    BoardGame dummyGame;
    BoardGame *insertGamePtr = nullptr;
    string displayChoice = "NULL";
    buildFromFile("board_games.txt", namesTree, barCodeTree, hashTable, gamePtr);
    char choice;
    intro();
    while ((choice = menu()) != QUIT_CHOICE)
    {
        switch (choice)
        {
            case INSERT_CHOICE:
            {
                cin.clear();
                cin.clear();
                cin.clear();
                insertManager(namesTree, barCodeTree, hashTable, insertGamePtr);
                break;
            }
            case DELETE_CHOICE:
            {
                cin.clear();
                cin.clear();
                cin.clear();
                deleteManager(namesTree, barCodeTree, hashTable, deleteStack, dummyGame);
                break;
            }
            case UNDO_DELETE_CHOICE:
            {
                cin.clear();
                cin.clear();
                cin.clear();
                undoDelete(namesTree, barCodeTree, hashTable, deleteStack);
                break;
            }
            case SEARCH_MANAGER_CHOICE:
            {
                searchManager(namesTree, barCodeTree, hashTable, dummyGame ,"NULL");
                break;
            }
            case LIST_MANAGER_CHOICE:
            {
                listManager(namesTree, barCodeTree, hashTable, displayChoice);
                break;
            }
            case WRITE_FILE_CHOICE:
            {
                cin.clear();
                cin.clear();
                cin.clear();
                namesTree.preOrder();
                BoardGame* ptr = nullptr;
                while (deleteStack.getCount() != 0) {
                    deleteStack.pop(ptr);
                    delete ptr;
                }
                break;
            }
            case STATISTICS_CHOICE:
            {
                cin.clear();
                cin.clear();
                cin.clear();
                cout << "\n Statistics\n";
                cout << "=======\n";
                cout << "Load Factor: " << hashTable.getLoadFactor() << "%" << endl;
                cout << "Collision Number: " << hashTable.getCollisionNum();
                break;
            }
            case SHOW_HELP_MENU_CHOICE:
            {
                cin.clear();
                cin.clear();
                cin.clear();
                intro();
            }
        }
    }
    dummyGame.setName("Dummy Game");
    if (insertGamePtr)
        insertGamePtr = nullptr;
    gamePtr = nullptr;
    return 0;
}

/**~*~*
 The intro function displays the menu
 *~**/
//Created by Julien Ditto
void intro()
{
    // Display the menu and get the user's choice.
    cout << "\nWhat do you want to do?\n\t"
    << INSERT_CHOICE
    << " - Insert a boardgame into the database\n\t"
    << DELETE_CHOICE
    << " - Delete a list of boardgames from the database\n\t"
    << UNDO_DELETE_CHOICE
    << " - Undo the last deleted boardgame\n\t"
    << SEARCH_MANAGER_CHOICE
    << " - Search for a boardgame in the database\n\t"
    << LIST_MANAGER_CHOICE
    << " - Display a boardgame from the database\n\t"
    << WRITE_FILE_CHOICE
    << " - Save data to file\n\t"
    << STATISTICS_CHOICE
    << " - Display statistics\n\t"
    << QUIT_CHOICE
    << " - Quit the program\n\t"
    << SHOW_HELP_MENU_CHOICE
    << " - Show help menu\n";
}

/**~*~*
 The menu function gets the user's choice, which is assigned to the
 reference parameter.
 *~**/
//Created by Julien Ditto
char menu()
{
    bool valid = false;
    string test;
    char choiceCstring[2];
    unsigned long length = 0;
    char choice = '\0';
    while(valid == false)
    {
        cin.clear();
        cin.clear();
        cin.clear();
        cout << "Enter your choice from the menu: " << endl;
        cin >> test;
        cin.clear();
        cin.clear();
        cin.clear();
        length = test.length();
        if (length != 1)
        {
            cout << "Invalid entry. Enter a Choice from the menu. " << endl;
        }
        else
        {
            strcpy(choiceCstring, test.c_str());
            choice = choiceCstring[0];
            if (choice >= 'A' && choice <= 'I')
                valid = true;
            else
                cout << "Invalid entry." << endl;
        }
    }
    return choice;
}

/*****************************************************************************
 Delete manager: delete boardgame objects from the trees and hash table until the user enters Q to quit
 deleting
 Input Parameter: title tree, barcode tree, hashtable, and stack to store deleted objects (for undo delete funcion)
 *****************************************************************************/
//Created by Julien Ditto
//Modified by Ekaterina Kazantseva to push deleted items onto the delete stack (to be implemented for undo delete)
void deleteManager(BinarySearchTree<BoardGame*> &treeA, BinarySearchTree<BoardGame*> &treeB, HashTable &hashTable, Stack<BoardGame*>& deleteStack, BoardGame &dummyGame)
{
    BoardGame *boardGame = &dummyGame;
    BoardGame *returnedGame;
    string barcode = "NULL";
    cout << "\n Delete\n";
    cout << "=======\n";
    while(barcode != "Q")
    {
        cin.clear();
        cin.clear();
        cin.clear();
        cin.clear();
        cout << "\nEnter the barcode of the board game that you want to delete (or Q to stop deleting) : \n";
        cin >> barcode;
        cin.clear();
        cin.clear();
        cin.clear();
        cin.clear();
        cin.ignore();
        if(barcode != "Q")
        {
            (*boardGame).setBarCode(barcode);
            if(treeB.getEntry(boardGame, returnedGame, compareByBarCode))
            {
                boardGame->setName(returnedGame->getName());
                if (treeA.remove(boardGame, compareByNameAndBarCode) && treeB.remove(boardGame, compareByBarCode))
                {
                    BoardGame *ptr = nullptr;
                    hashTable.searchHash(barcode, ptr);
                    deleteStack.push(ptr);
                    hashTable.deleteElem(barcode);
                    cout << "The Board Game: " << *ptr << " has been deleted!\n";
                }
                else
                    cout << "Delete was not succesful" << endl;
            }
            else
                cout << "The Board Game was not found in this list. " << endl;
        }
    }
    cout << "___________________END DELETE SECTION_____\n";
}

/*****************************************************************************
 Search manager: searches the trees and hash table for an BoardGame object
 Input Parameter: title tree, barcode tree, hashtable, string which will show which of the data structures the user wants to search from.
 Note: Since the we are giving the user the freedom to insert titles exactly how they like, without input validation, we prefer to make the user to be exact when they are searching for boardgames by title. Meaning, the ASCII values of the name (title) of the boardgame that the user is searching for has to match the ASCII value of the name of a boardgame in the title tree, for a boardgame to be found.
 *****************************************************************************/
//Created by Julien Ditto
//Modified by Ekaterina Kazantseva to show duplicate objects when searching the title tree
void searchManager(BinarySearchTree<BoardGame*> &treeA, BinarySearchTree<BoardGame*> &treeB, HashTable &hashTable, BoardGame &dummyGame, string searchChoice)
{
     string target = "NULL";
     cout << "\n Search\n";
     cout <<   "=======\n";
     BoardGame *boardgame = &dummyGame;
     BoardGame *returnedGame;
     cin.ignore();
     while(searchChoice != "Q" && target != "Q")
     {
         cout << "Enter \"T\" if you would like to search the title tree, \"B\" if you would like to search the barcode tree, \"H\" if you would like to search the hash table, or \"Q\" to quit the search manager." << endl;
         cin.clear();
         cin.clear();
         cin.clear();
         cin.clear();
         cin >> searchChoice;
         cin.clear();
         cin.clear();
         cin.clear();
         if (searchChoice != "Q")
         {
             if (searchChoice == "T")
             {
                 cin.ignore();
                 cout << "\nEnter a boardgame name (or Q to stop searching) : \n";
                 cin.clear();
                 cin.clear();
                 cin.clear();
                 getline(cin, target);
                 cin.clear();
                 cin.clear();
                 cin.clear();
                 boardgame->setName(target);
                 if(target != "Q")
                 {
                     if(treeA.getEntry(boardgame, returnedGame, compareByName)) {
                         cout << "The Board Game was found: " << endl << *returnedGame << endl;
                         while (returnedGame->getDuplicateCount() != 0) {
                             boardgame->setDuplicateCount(returnedGame->getDuplicateCount() - 1);
                             treeA.getEntry(boardgame, returnedGame, compareByNameAndDuplicateCount);
                             cout << *returnedGame << endl;
                         }
                     }
                     else
                         cout << "Board Game \"" << target << "\" was not found in this list." << endl;
                 }
             }
             else if (searchChoice == "B")
             {
                 cin.ignore();
                 cin.clear();
                 cin.clear();
                 cin.clear();
                 cout << "\nEnter a barcode (or Q to stop searching) : \n";
                 getline(cin, target);
                 cin.clear();
                 cin.clear();
                 cin.clear();
                 cin.clear();
                 boardgame->setBarCode(target);
                 
                 if(target != "Q")
                 {
                     if(treeB.getEntry(boardgame, returnedGame, compareByBarCode))
                         cout << "The Board Game was found: " << *returnedGame << endl;
                     else
                         cout << "The Board Game with the barcode " << target << " was not found in this list." << endl;
                 }
             }
             else if (searchChoice == "H")
             {
                 cin.ignore();
                 cin.clear();
                 cin.clear();
                 cin.clear();
                 cout << "\nEnter a barcode to search the hash table (or Q to stop searching) : \n";
                 getline(cin, target);
                 cin.clear();
                 cin.clear();
                 cin.clear();
                 cin.clear();
                 if (target != "Q")
                 {
                     if (hashTable.searchHash(target, returnedGame))
                         cout << "The Board Game was found in the hash table: " << *returnedGame << endl;
                     else
                         cout << "The Board Game with the barcode: " << target << " was not found in the hash table." << endl;
                 }
             }
             else
             {
                 cin.ignore();
                 cin.clear();
                 cin.clear();
                 cin.clear();
                 cout << "Invalid Entry" << endl;
             }
         }
     }
     cout << "___________________END SEARCH SECTION _____\n";
}

/**********************************************************************
 parseBarcode: Asks user to enter a barcode to be stored in Boardgame class and then inserted into the hashtable, and 2 trees by insert manager. Provides error checking to ignore duplicates, alphabetical letters, and punctuation.
 Input Parameter: barcode tree and pointer to boardgame object.
 **********************************************************************/
//Created by Julien Ditto
bool parseBarcode(BinarySearchTree<BoardGame*> &treeB, BoardGame *insertGamePtr)
{
     string testBarcode = insertGamePtr->getBarCode();
     bool done = false;
     bool flag = true;
     bool first = true;

     while (testBarcode != "Q" && done == false)
     {
         bool success = true;
         unsigned long length = 0;
         length = testBarcode.length();
         char *charTest = nullptr;
         charTest = new char [length + 1];
         if (length != 12)
         {
             cout << "\n Please enter a valid entry (only 12 digit integers) or Q to stop inserting: " << endl;
             success = false;
             first = false;
             cin.clear();
             cin.clear();
             cin.clear();
             cin >> testBarcode;
             cin.clear();
             cin.clear();
             cin.clear();
         }
         else
         {
             strcpy(charTest, testBarcode.c_str());
             unsigned long i = 0;
             while (i < length)
             {
                 if (!(isdigit(charTest[i])))
                 {
                     i = length;
                     cout << "\n Please enter a valid entry (only 12 digit integers) or Q to stop inserting: " << endl;
                     success = false;
                     first = false;
                     cin.clear();
                     cin.clear();
                     cin.clear();
                     cin >> testBarcode;
                     cin.clear();
                     cin.clear();
                     cin.clear();
                 }
                 i++;
             }
         }
         if (success)
         {
             if (!first)
                 insertGamePtr->setBarCode(testBarcode);
             if(treeB.getEntry(insertGamePtr, insertGamePtr, compareByBarCode))
             {
                 cout << insertGamePtr->getBarCode() << " is already in the list. Enter another barcode or Q to stop inserting: \n";
                 first = false;
                 cin.clear();
                 cin.clear();
                 cin.clear();
                 cin >> testBarcode;
                 cin.clear();
                 cin.clear();
                 cin.clear();
             }
             else
             {
                 insertGamePtr->setBarCode(testBarcode);
                 done = true;
             }
         }
         if (testBarcode == "Q")
             flag = false;
         delete [] charTest;
         charTest = nullptr;
     }
     return flag;
}

/**********************************************************************
 Insert manager: inserts an object into both trees and the hash table
 Input Parameter: Title tree, barcode tree, hashtable, pointer to BoardGame object
 Note: We did not want to implement input validation for names (titles) of boardgames
 because we want the user to have the freedom to create titles exactly how they like.
 **********************************************************************/
//Created by Julien Ditto
void insertManager(BinarySearchTree<BoardGame*> &treeA, BinarySearchTree<BoardGame*> &treeB, HashTable &hashTable, BoardGame *insertGamePtr)
{
    string bcode, name, brand, lang;
    double price;
    bool flag = true;
    string answer = "NULL";
    cout << "\n Insert\n";
    cout <<   "=======\n";
    while (flag)
    {
        insertGamePtr = new BoardGame();
        cin.clear();
        cin.clear();
        cin.clear();
        cout << "Enter the a 12 digit number for the barcode: " << endl;
        cin >> bcode;
        cin.clear();
        cin.clear();
        cin.clear();
        insertGamePtr->setBarCode(bcode);
        flag = parseBarcode(treeB, insertGamePtr);
        cin.ignore();
        if (flag)
        {
            cout << "Enter the title of the boardgame: " << endl;
            getline(cin, name);
            cin.clear();
            cin.clear();
            cin.clear();
            insertGamePtr->setName(name);
            cout << "Enter the brand name: " << endl;
            getline(cin, brand);
            cin.clear();
            cin.clear();
            cin.clear();
            insertGamePtr->setBrand(brand);
            cout << "Enter the price of the boardgame (must be a double): " << endl;
            while(1)
            {
                if(cin >> price)
                    break;
                else
                {
                    cout << "Not a valid input! Please enter a double." << endl;
                    cin.clear();
                    while(cin.get() != '\n');
                }
            }
            cin.ignore();
            insertGamePtr->setPrice(price);
            cin.clear();
            cin.clear();
            cin.clear();
            cout << "Enter the language of the boardgame: " << endl;
            cin >> lang;
            cin.clear();
            cin.clear();
            cin.clear();
            cin.ignore();
            insertGamePtr->setLanguage(lang);
            treeA.insert(insertGamePtr, compareByName);
            treeB.insert(insertGamePtr, compareByBarCode);
            hashTable.insertHash(insertGamePtr->getBarCode(), insertGamePtr);
            cout << "The board game was successfully inserted into the list! If you want to insert another board game into the list, enter \"Y\" otherwise enter anything else." << endl;
            cin >> answer;
            if (answer != "Y")
                flag = false;
        }
        else
        {
            flag = false;
            delete insertGamePtr;
            insertGamePtr = nullptr;
        }
    }
    cout << "___________________END INSERT SECTION _____\n";
}

 //Populates 2 BSTs and a hashtable from input file
 //receives input file name, 2 BSTs and a hashtable
//////////////////////////////////////////////////////
//-------Created by Ekaterina Kazantseva---------//
/////////////////////////////////////////////////////
void buildFromFile(string filename, BinarySearchTree<BoardGame*> &treeA, BinarySearchTree<BoardGame*> &treeB, HashTable &t, BoardGame *gamePtr)
{
     ifstream inFile;
     string bcode, name, line, brand, price, lang;
     double p = 0;
     inFile.open(filename);
     if (!inFile)
     {
         cout << "Error opening the input file: \"" << filename << "\"" << endl;
         exit(EXIT_FAILURE);
     }
     cout << "Building database from file..." << endl;
     while (inFile >> bcode)
     {
         inFile.ignore(256, '\t');
         getline(inFile, line);
         int offset;
         findTabs(line);
         getValue(line, name);
         findTabs(line);
         getValue(line, brand);
         findTabs(line);
         offset = (int) line.find('\t', 0);
         price = line.substr(0, offset);
         line = line.substr(offset);
         p = stod(price);
         findTabs(line);
         lang = line;
         gamePtr = new BoardGame(bcode, name, brand, lang, p, 0);
         treeA.insert(gamePtr, compareByName);
         treeB.insert(gamePtr, compareByBarCode);
         t.insertHash(bcode, gamePtr);
         if (t.getLoadFactor() > 75)
         {
             cout << "REHASHING" << endl<< "Old load factor: ";
             cout << fixed << showpoint << setprecision(2) << t.getLoadFactor() << "%" << endl;
             cout << "Old Collision Number: " << t.getCollisionNum() << endl;
             t.print();
             rehash(t);
             cout << endl <<"New load factor: " << fixed << showpoint << setprecision(2) << t.getLoadFactor() << "%" << endl;
             cout << "New Collision Number: " << t.getCollisionNum() << endl;
             t.print();
         }
     }
    cout << endl << "Complete HashTable" << endl;
    cout << "Load Factor: " <<fixed << showpoint << setprecision(2)<< t.getLoadFactor() << "%" << endl;
    cout << "Collision Number: " << t.getCollisionNum() << endl;
    t.print();
     inFile.close();
 }

//Created by Ekaterina Kazantseva
//Strips a passed by reference line from tab spaces before a non-tab value is found
void findTabs (string &line)
{
    while (line.find('\t', 0) == 0)
    {
        line = line.substr(1);
    }
}

//Created by Ekaterina Kazantseva
//Gets value before tab spaces
void getValue (string &line, string &n)
{
    int offset = 0;
    line = line.substr(offset);
    offset = (int)line.find('\t', 0);
    n = line.substr(0, offset);
    line = line.substr(offset);
}

//Created by Ekaterina Kazantseva
//Determines hashtable size
//To demonstrate rehashing, the size is 2.5 times smaller than there are objects
int hashSize (string filename)
{
    ifstream inFile;
    string line;
    int size = 0;
    inFile.open(filename);
    while (getline(inFile, line))
    {
        size++;
    }
    size = size/2;
    size = getNextPrime(size);
    inFile.close();
    return size;
}

//Created by Ekaterina Kazantseva
//Gets a prime number bigger than the passed argument
int getNextPrime(int n)
{
    if (n <= 1)
        return 2;
    int prime = n;
    bool found = false;
    while (!found)
    {
        prime++;
        if (isPrime(prime))
            found = true;
    }
    return prime;
}

//Created by Ekaterina Kazantseva
//Checks if a number is prime
bool isPrime(int n)
{
    if (n <= 1) return false;
    if (n <= 3) return true;
    
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

//Created by Ekaterina Kazantseva
//Functions to use with function pointers
//for a title BST and a barcode BST
int compareByName(BoardGame* b1, BoardGame* b2)
{
    if (b1->getName() > b2->getName())
        return 1;
    else if (b1->getName() < b2->getName())
        return -1;
    return 0;
}

//Created by Ekaterina Kazantseva
int compareByBarCode(BoardGame* b1, BoardGame* b2)
{
    if (b1->getBarCode() > b2->getBarCode())
        return 1;
    else if (b1->getBarCode() < b2->getBarCode())
        return -1;
    return 0;
}

//Created by Ekaterina Kazantseva
int compareByNameAndDuplicateCount(BoardGame* b1, BoardGame* b2)
{
    if (b1->getName() > b2->getName())
        return 1;
    else if (b1->getName() < b2->getName())
        return -1;
    else if (b1->getName() == b2->getName() && b1->getDuplicateCount() == b2->getDuplicateCount())
    {
        return 0;
    }
    else
        return -1;
}

//Created by Ekaterina Kazantseva
int compareByNameAndBarCode(BoardGame* b1, BoardGame* b2) {
    if (b1->getName() > b2->getName())
        return 1;
    else if (b1->getName() < b2->getName())
        return -1;
    else if (b1->getName() == b2->getName() && b1->getBarCode() == b2->getBarCode())
        return 0;
    return -1;
}

string printName (BoardGame* b)
{
    return b->getName();
}

string printBarCode (BoardGame* b)
{
    return b->getBarCode();
}
//Created by Ekaterina Kazantseva
//Modified by Chris Yan to change implementation using hashnode struct (original) to hashnode class (new)
//Creates a new hashtable 2x actual size of the old one
//the new hashtable is then assigned to the old one
//old data gets discarded in the process
void rehash(HashTable &t)
{
    int size = 2 * t.getCurrSize();
    BoardGame* gamePtr;
    HashNode* nodePtr;
    HashTable newTable(getNextPrime(size));
    HashNode** data = t.getData();
    for (int i = 0; i < t.getSize(); i++)
    {
        if (data[i] != nullptr)
        {
            gamePtr = data[i]->getBoardGameInfo();
            newTable.insertHash(gamePtr->getBarCode(), gamePtr);
            nodePtr = data[i]->getNext();
            while (nodePtr != nullptr)
            {
                gamePtr = nodePtr->getBoardGameInfo();
                newTable.insertHash(gamePtr->getBarCode(), gamePtr);
                nodePtr = nodePtr->getNext();
            }
        }
    }
    t = newTable;
    data = newTable.getData();
    HashNode* nextNodePtr;
    for (int i = 0; i < t.getSize(); i++)
    {
        if (data[i] != nullptr)
        {
            nodePtr = data[i];
            nextNodePtr = nodePtr->getNext();
            delete nodePtr;
            data[i] = nullptr;
            while (nextNodePtr != nullptr)
            {
                nodePtr = nextNodePtr;
                nextNodePtr = nextNodePtr->getNext();
                delete nodePtr;
            }
        }
    }
}

/************
 undoDelete
 - parameter: HashTable, BinarySearchTree, BinarySearchTree, Stack<BoardGame>
 - deleted games are stored in deleteStack
 - pop games from Stack and insert back in hash and the 2 BST
 *************/
//algorithm outline provided by Jerry Chan
//implemented/integrated by Ekaterina Kazantseva
bool undoDelete(BinarySearchTree<BoardGame*>& treeA, BinarySearchTree<BoardGame*>& treeB, HashTable& hashTable, Stack<BoardGame*>& deleteStack)
{
    BoardGame* game = nullptr;
    
    if (deleteStack.getCount() == 0)
    {
        cout << "\nSorry, there is nothing to undelete!\n";
        return false;
    }
    else
    {
        deleteStack.pop(game);
        treeA.insert(game, compareByName);
        treeB.insert(game, compareByBarCode);
        hashTable.insertHash(game->getBarCode(), game);
        cout << "Undo delete on " << game->getName() << " was successful!" << endl;
    }
    return true;
}

//Created by Julien Ditto
//Displays data in the order of title, barcode, or unsorted hash table based on what the user wants to see
void listManager (BinarySearchTree<BoardGame*>& treeA, BinarySearchTree<BoardGame*>& treeB, HashTable& hashTable, string display)
{
    cout << "\n List\n";
    cout << "=======\n";
    while (display != "Q")
    {
        cout << "Enter \"H\" to see unsorted hash list, \"B\" to see boardgames sorted by barcodes, \"T\" to see boardgames sorted by titles, \"I\" to see the tree indented based on the level of each node, or \"Q\" to quit the list manager" << endl;
        cin.clear();
        cin.clear();
        cin.clear();
        cin.clear();
        cin >> display;
        if(display == "H")
        {
            cout << "\n Hash Table Data\n";
            cout << "=======\n";
            cout << "Unsorted Hash Table:" << endl;
            hashTable.print();
            cout << endl;
        }
        else if (display == "B")
        {
            cout << "\n Sorted Barcode Data\n";
            cout << "=======\n";
            cout  << endl << "In Order Barcode Tree:" << endl << endl;
            treeB.inOrder();
            cout << endl;
        }
        else if (display == "T")
        {
            cout << "\n Sorted Title Data\n";
            cout << "=======\n";
            cout  << endl << "In Order Title Tree:" << endl << endl;
            treeA.inOrder();
            cout << endl;
        }
        else if (display == "I")
        {
            cout << "\n Indented Print\n";
            cout << "=======\n";
            cout << "Indented Names Tree: " << endl << endl;
            treeA.print(printName);
            cout << endl;
            cout << "Indented Barcode Tree: " << endl << endl;
            treeB.print(printBarCode);
            cout << endl;
        }
        else
            if (display != "Q")
                cout << "Invalid Entry" << endl;
    }
}

