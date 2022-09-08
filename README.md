# database_system

A C++ system that reads entries from a file into a database created by a hash table and 2 binary search trees. Each entry has a barcode which is used as a primary key for the hash table and one binary search tree. The second binary search tree uses a title as a primary key. The database uses dynamic memory  and can output it's contents to a file. Operations such as search, insert, list, and delete are also supported.
