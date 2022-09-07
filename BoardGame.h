// BoardGame Class
// Created by: Ekaterina Kazantseva

#ifndef BoardGame_h
 #define BoardGame_h
 #include <iomanip>
 #include <iostream>
#include <string>
 using namespace std;

/*
 BoardGame object:
 Data from an input file as well as from user input is first stored in the private members of the BoardGame object.
 */
 class BoardGame
 {
 private:
 string barCode;
 string name;
 string brand;
 string language;
 double price;
 int duplicateCount;
 public:
 BoardGame() {barCode = ""; name = ""; brand = ""; language= ""; price = 0.0; duplicateCount = 0;}
 BoardGame(string bc, string n, string br, string l, double p, int dc)
 {
    barCode = bc; name = n; brand = br; language= l; price = p; duplicateCount = dc;
 }
     ~BoardGame() { cout << "Destructor: " << name << " is deleted!" << endl; };
     void incrementDuplicateCount() {duplicateCount++;}
     void decrementDuplicateCount() {duplicateCount--;}
     void setBarCode(string bc) { barCode = bc; }
     void setName(string n) {name = n;}
     void setBrand(string br) {brand = br;}
     void setLanguage(string l) {language = l;}
     void setPrice (double p) {price = p;}
     void setDuplicateCount(int dc) {duplicateCount = dc;}
     
     string getBarCode() const {return barCode;}
     string getName() const {return name;}
     string getBrand() const {return brand;}
     string getLanguage() const {return language;}
     double getPrice() const {return price;}
     int getDuplicateCount() const {return duplicateCount;}
     friend ostream& operator<<(ostream &out, const BoardGame &c);
     bool operator==(const BoardGame &);
     bool operator<(const BoardGame &right);
     bool operator>(const BoardGame &right);
     string operator[](string key);
     string returnKeyValue (string key);
 };
 
 //**************************************************
 // Overloaded stream output operator
 //**************************************************
 ostream& operator<<(ostream &out, const BoardGame &c) {
     int n = 0;
 out << c.barCode;
 out << string(2, '\t') << c.name;
     n = (6 - ((int)c.name.size())/4);
 out << string(n, '\t') << c.brand;
     n = (6 - ((int)c.brand.size())/4);
 out << string(n, '\t') << fixed << showpoint << setprecision(2) << c.price;
 out << "\t\t" << c.language;
 return out << endl;
 }
 
 //**************************************************
 // Overloaded comparison operator
 //**************************************************
 bool BoardGame::operator==(const BoardGame &right) {
 if (name == right.name)
 return true;
 return false;
 }
 
 //**************************************************
 // Overloaded comparison operator
 //**************************************************
 bool BoardGame::operator<(const BoardGame &right) {
 if (name < right.name)
 return true;
 return false;
 }
 
 bool BoardGame::operator>(const BoardGame &right) {
 if (name > right.name)
 return true;
 return false;
 }
 
 
 string BoardGame::returnKeyValue (string key) {
 if (key == "barCode")
 return barCode;
 if (key == "name")
 return name;
 return "";
 }

#endif /* BoardGame_h */

