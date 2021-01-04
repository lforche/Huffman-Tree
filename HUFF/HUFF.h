/* This is the file with the header for Lab 2 where a Huffman Tree that is used to encode and decode files is created.
  Name: Lindsey Forche
  Class: EECS 2510-001 (Non-linear Data Structures)
  Due Date: 3/17/2020 */

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream> 

#pragma once
using namespace std;

class HUFF
{
public:
	HUFF(); 	// Constructor
	// Methods that are called by the command line
	void makeTreeBuilder(string inputFile, string outputFile);
	void encodeFile(string inputFile, string outputFile);
	void encodeFileWithTree(string inputFile, string TreeFile, string outputFile);
	void decodeFile(string inputFile, string outputFile);
	void displayHelp();
	// Total Bytes In and Total Bytes Out variables
    unsigned int totalIn = 0;
    unsigned int totalOut = 0;

private:
	// A node structure created that has a symbol, weight, left and right child pointers
    struct node {
		unsigned char symbol = '\0';
		unsigned int weight = 0;
		node* left = nullptr;
		node* right = nullptr;
	};
	node* root; // Node pointer for the root of the tree that will be referenced in the methods

	// The traverse method is used to get the bit strings of the tree created
	// It is an inorder recursive method that uses a node, string, and string array to create the bit strings
	// It adds a "0" to the bit string if it follows a left child path
	// It adds a "1" to the bit string if it follows a right child path
	void traverse(node* node, string str, string table[])
	{
		if (node == nullptr) return;
		if (node->left != nullptr)
			traverse(node->left, str + "0", table);
		if ((node->left == nullptr) && (node->right == nullptr))
			table[node->symbol] = str;
		if (node->right != nullptr)
			traverse(node->right, str + "1", table);
	}

	// The makeBitString method uses the traverse method to fill the bit string table
	// Then it returns the string array that is then used 
	string* makeBitStringTable(node* root)
	{
	    static string stringTable[256];
		string bitString;
		traverse(root, bitString, stringTable);
		return stringTable;
	}
};
