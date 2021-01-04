/* This is the file with all of the methods for Lab 2 where a Huffman Tree that is used to encode and decode files is created.
  Name: Lindsey Forche
  Class: EECS 2510-001 (Non-linear Data Structures)
  Due Date: 3/17/2020 */

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream> 
#include "HUFF.h"

using namespace std;

HUFF::HUFF()
{
    // This method is the constructor
    // It establishes that the root node is null
    root = NULL; // This creates a null node when the tree is created
}

void HUFF::makeTreeBuilder(string inputFile, string outputFile)
{
    // This method creates the tree from an input file and then puts it in an output file
    // IT reads in the input file character by character, finds the frequency of each of the characters, 
    // then finds the two characters with the smallest frequencies and creates a parent node for these two characters
    // This repeats until there is only one root node held in the first spot of nodes array
    // As the tree is being created, the locations of the two smalles nodes is documented and then added to the output file
    ifstream file;
    file.open(inputFile, ios::binary);

    unsigned char c; // unsigned character that is read in from the file
    unsigned int charFreq[256] = { 0 }; // frequency array
    node* nodes[256]; // node array
    unsigned int min1Weight, min2Weight; // the two smallest weights of the two smallest nodes
    
    // Check that the input file is open
    // Otherwise say an error and exit the program
    if (!file.is_open())
    {
        cout << "Unable to open file";
        return;
    }
    // Get each character from the file, increase the frequency for the char by 1, and increase the total number of bytes in by 1
    while (file.peek() != -1)
    {
        c = file.get();
        charFreq[c]++;
        totalIn++;
    }
    // Create 256 nodes for the 256 ascii characters
    // For each of the nodes, add the char's frequency and set the left and right child to nullptr
    // Then there should be 256 not connected nodes
    for (unsigned int a = 0; a < 256; a++)
    {
        nodes[a] = new node;
        nodes[a]->symbol = (unsigned char)a;
        nodes[a]->weight = charFreq[a];
        nodes[a]->left = nullptr;
        nodes[a]->right = nullptr;
    }
    // Close the input file and open the output file
    file.close();
    ofstream outFile;
    outFile.open(outputFile, ios::trunc); // Clear the output file before adding anything in case the name of the output file was already used
    outFile.close();
    outFile.open(outputFile, ios::binary);
    // Check that the output file is open
    // Otherwise say an error and exit the program
    if (!outFile.is_open())
    {
        cout << "Unable to open file";
        return;
    }
    // Find the two smallest weighted nodes in the node array
    // Put the locations of the two smallest weighted nodes in the node array in the output file
    for (unsigned int c = 0; c < 255; c++)
    {
        unsigned char min1Location, min2Location; 
        //set min1Weight and min2Weight to a big number
        min1Weight = 0x7fffffff; 
        min2Weight = 0x7fffffff;

        node* min1Node = nodes[0];
        node* min2Node = nodes[0];

        // find min1Weight by iterating through the non-null node pointer array and seeing if there is anything smaller than min1Weight
        for (unsigned int d = 0; d < 256; d++)
        {
            if ((nodes[d] != nullptr) && (nodes[d]->weight < min1Weight))
            {
                min1Weight = nodes[d]->weight;
                min1Node = nodes[d];
                min1Location = d;
            }
        }
        // find min2Weight by iterating through the non-null node pointer array and seeing if there is anything smaller than min2Weight that is not the same location as min1Location
        for (unsigned int e = 0; e < 256; e++)
        {
            if ((nodes[e] != nullptr) && (nodes[e]->weight < min2Weight) && (e != min1Location))
            {
                min2Weight = nodes[e]->weight;
                min2Node = nodes[e];
                min2Location = e;
            }
        }
        // Create a parent node for the two mins with a null value as a symbol and a weight that is the sum of the two mins
        node* parent = new node;
        parent->symbol = '\0';
        parent->weight = min1Weight + min2Weight;
        // find the location in the node array to replace the node pointer with the parent and make the other node pointer null
        // Compare the locations of the two mins
        // If min1Location is less then add its location to the 510 byte tree builder file first 
        //      and add min1Node as the left child of the parent node 
        //      and add min2Node as the right child of the parent node
        //      and put the parent node in the location of the min1Node
        //      and make the location of the min2Node null
        // Increase the total output bytes by 1 everytime a char is added to the output file
        if (min1Location < min2Location)
        {
            outFile.put(min1Location);
            totalOut++;
            outFile.put(min2Location);
            totalOut++;

            parent->left = min1Node;
            parent->right = min2Node;
            nodes[min1Location] = parent;
            nodes[min2Location] = nullptr;
        }
        // Otherwise, min2Location should be added to the 510 byte tree builder file first
        //      and add min2Node as the left child of the parent node 
        //      and add min1Node as the right child of the parent node
        //      and put the parent node in the location of the min2Node
        //      and make the location of the min1Node null
        // Increase the total output bytes by 1 everytime a char is added to the output file
        else
        {
            outFile.put(min2Location);
            totalOut++;
            outFile.put(min1Location);
            totalOut++;

            parent->left = min2Node;
            parent->right = min1Node;
            nodes[min2Location] = parent;
            nodes[min1Location] = nullptr;
        }
    }
    // The root of the tree should be in nodes[0] now
    root = nodes[0];
    outFile.close(); // close the output file
}
void HUFF::encodeFile(string inputFile, string outputFile)
{
    // This method takes in the input file and encodes it and puts the tree and the encoded information into the output file
    // It uses the makeTreeBuilder method to create the tree and adds that tree to the output file
    // Then it reads the input file again and encodes it by creating stings of bit strings that then convert into 8 bits/an unsigned char
    // The unsigned char is then added to the output file after the tree builder instructions
    // This is continued until there are no more characters to read in the input file
    // Then if there is anything left in the string of bit strings, padding bits are added by looking for a bit string that is longer than what you need
    // which is then appended to the string of bit strings and turned into an unsigned char
    makeTreeBuilder(inputFile, outputFile); // Use the make tree builder method to make the tree from an input file
    totalIn = 0; // Set the total number of bytes into 0
    
    string* stringTable = makeBitStringTable(root); // Use the make bit string table method to create a bit string array with the tree information from the make tree builder function
    
    ofstream encodedFile;
    encodedFile.open(outputFile, ios::binary | ios::app);
    // Check that the encoded file is open
    // Otherwise say an error and exit the program
    if (!encodedFile.is_open())
    {
        cout << "Unable to open file";
        return;
    }

    ifstream file;
    file.open(inputFile, ios::binary);
    // Check that the input file is open
    // Otherwise say an error and exit the program
    if (!file.is_open())
    {
        cout << "Unable to open file";
        return;
    }
    unsigned char input; // Char from input file
    unsigned char output = 0; // Char that will be in the encoded file
    string saveBits; // To be able to create a complete char, the bit strings are sometimes saved to this string to allow for a char of 8 bits to be created
    unsigned int bitNum = 0; // Number of bits added to the char
    unsigned char P2[] = { 128, 64, 32, 16, 8, 4, 2, 1 }; // Powers of 2
    // The input file will be encoded 
    while (file.peek() != -1)
    {
        // First get the char from the input file
        input = file.get();
        // Get the bit string associated with the input char
        string bitString = stringTable[input]; 
        // Add the bit String of the current char to the string that may have bit strings from previous iterations
        saveBits += bitString;
        // When the size of the string that has current and previous bit strings is greater than 7, 
        // which means there are at least 8 string chars to be converted to bits, then those values can be converted to bits
        // else the next iteration will occur to get another bit string
        if (saveBits.size() > 7)
        {
            for (unsigned int b = 0; b < 9; b++)
            {
                // If there have been 8 bits added to the char, then add the char to the output file, 
                // set the output file to 0, set the bitNum to 0, and erase the first 8 values of the saveBits string
                // Also, add 1 to the total bytes out
                if (bitNum > 7)
                {
                    encodedFile.put(output);
                    totalOut++;
                    output = 0;
                    bitNum = 0;
                    saveBits.erase(0, 8);
                }
                // If there have not been 8 bits added to the char yet, then look at the next character in the saveBits string
                else
                {
                    // If the value in the saveBits string is a 1, then or the output character with the powers of 2 
                    // If the value in the saveBits string is a 0, nothing needs to be done since the output char is set to 0 before any manipulation is done
                    // Finally increment the number of bits added to the output char
                    if (saveBits[b] == '1')
                        output |= P2[b];
                    bitNum++;
                }
            }
        }
        // Increment the total bytes in after every char
        totalIn++; 
    }
    // Most likely saveBits will have some characters leftover, so padding bits might be needed
    if ((saveBits.size() > 0) && (saveBits.size() <= 8))
    {
        // First check how many padding bits are needed
        unsigned int paddingBits = 8 - saveBits.size();
        string longestString;
        // Find a bit string longer than the number of paddings bits you need
        for (unsigned int i = 0; i < 256; i++)
        {
            if (stringTable[i].length() > paddingBits)
                longestString = stringTable[i];
        }
        // Go through the bit string that was just found only the number of times that you need for padding
        for (unsigned int j = 0; j < paddingBits; j++)
        {
            // If the character in the bit string is a 1, then add a 1 to the saveBits bit string
            // Otherwise, add a zero
            if (longestString[j] == '1')
                saveBits += "1";
            else
                saveBits += "0";
        }
        // Now go through the saveBits string one last time, to add one more character to the output file
        for (unsigned int k = 0; k < 9; k++)
        {
            // If there have been 8 bits added to the char, then add the char to the output file, 
            // set the output file to 0, set the bitNum to 0, and erase the first 8 values of the saveBits string
            // Also, add 1 to the total bytes out
            if (bitNum > 7)
            {
                encodedFile.put(output);
                totalOut++;
                output = 0;
                bitNum = 0;
                saveBits.erase(0, 8);
            }
            // If there have not been 8 bits added to the char yet, then look at the next character in the saveBits string
            else
            {
                // If the value in the saveBits string is a 1, then or the output character with the powers of 2 
                // If the value in the saveBits string is a 0, nothing needs to be done since the output char is set to 0 before any manipulation is done
                // Finally increment the number of bits added to the output char
                if (saveBits[k] == '1')
                    output |= P2[k];
                bitNum++;
            }
        }
    }
    file.close(); // Close the input file
    encodedFile.close(); // Close the output file
}
void HUFF::encodeFileWithTree(string inputFile, string TreeFile, string outputFile)
{
    // This method takes in the input file and tree file and encodes it and puts the tree and the encoded information into the output file
    // It uses the tree file to create the tree and adds that tree to the output file
    // Then it reads the input file again and encodes it by creating stings of bit strings that then convert into 8 bits/an unsigned char
    // The unsigned char is then added to the output file after the tree builder instructions
    // This is continued until there are no more characters to read in the input file
    // Then if there is anything left in the string of bit strings, padding bits are added by looking for a bit string that is longer than what you need
    // which is then appended to the string of bit strings and turned into an unsigned char
    ifstream tFile;
    unsigned char a;
    unsigned char treeInstructions[510] = { 0 };
    unsigned int count = 0;
    node* nodes[256] = { 0 };
    tFile.open(TreeFile, ios::binary);
    // Check that the tree file is open
    // Otherwise say an error and exit the program
    if (!tFile.is_open())
    {
        cout << "Unable to open file";
        return;
    }

    ofstream encodedFile;
    encodedFile.open(outputFile, ios::trunc); // Clear the output file before adding anything in case the name of the output file was already used
    encodedFile.close();
    encodedFile.open(outputFile, ios::binary | ios::app);
    // Check that the encoded file is open
    // Otherwise say an error and exit the program
    if (!encodedFile.is_open())
    {
        cout << "Unable to open file";
        return;
    }
    // Get each character from the file, place the chars in an array to keep track of the order, increase the frequency for the char by 1, and increase the total number of bytes in and out by 1
    for (unsigned int c = 0; c < 510; c++)
    {
        a = tFile.get();
        encodedFile.put(a);
        treeInstructions[count] = a;
        count++;
        totalOut++;
    }
    tFile.close();

    // Use the instructions that were represented by the first 510 unsigned chars of the input file to build the tree
    for (unsigned int d = 0; d < 510; d = d + 2)
    {
        int min1, min2;
        // Take the first two values in the array of the tree instructions
        min1 = treeInstructions[d];
        min2 = treeInstructions[d + 1];
        // Make those two values into nodes
        if (nodes[min1] == NULL)
        {
            nodes[min1] = new node;
            nodes[min1]->symbol = treeInstructions[d];
        }

        if (nodes[min2] == NULL)
        {
            nodes[min2] = new node;
            nodes[min2]->symbol = treeInstructions[d + 1];
        }
        // Make a parent node for the two instructions
        // Make the left child the first value 
        // Make the right child the second value
        // Put the parent in the location of the first min
        // Make the location of the second min null
        node* parent = new node;
        parent->left = nodes[min1];
        parent->right = nodes[min2];
        nodes[min1] = parent;
        nodes[min2] = nullptr;
    }
    root = nodes[0];

    ifstream file;
    file.open(inputFile, ios::binary);
    // Check that the encoded file is open
    // Otherwise say an error and exit the program
    if (!file.is_open())
    {
        cout << "Unable to open file";
        return;
    }
    string* stringTable = makeBitStringTable(root); // Use the make bit string table method to create a bit string array with the tree information from the make tree builder function

    unsigned char input; // Char from input file
    unsigned char output = 0; // Char that will be in the encoded file
    string saveBits; // To be able to create a complete char, the bit strings are sometimes saved to this string to allow for a char of 8 bits to be created
    unsigned int bitNum = 0; // Number of bits added to the char
    unsigned char P2[] = { 128, 64, 32, 16, 8, 4, 2, 1 }; // Powers of 2
    // The input file will be encoded 
    while (file.peek() != -1)
    {
        // First get the char from the input file
        input = file.get();
        // Get the bit string associated with the input char
        string bitString = stringTable[input];
        // Add the bit String of the current char to the string that may have bit strings from previous iterations
        saveBits += bitString;
        // When the size of the string that has current and previous bit strings is greater than 7, 
        // which means there are at least 8 string chars to be converted to bits, then those values can be converted to bits
        // else the next iteration will occur to get another bit string
        if (saveBits.size() > 7)
        {
            for (unsigned int b = 0; b < 9; b++)
            {
                // If there have been 8 bits added to the char, then add the char to the output file, 
                // set the output file to 0, set the bitNum to 0, and erase the first 8 values of the saveBits string
                // Also, add 1 to the total bytes out
                if (bitNum > 7)
                {
                    encodedFile.put(output);
                    totalOut++;
                    output = 0;
                    bitNum = 0;
                    saveBits.erase(0, 8);
                }
                // If there have not been 8 bits added to the char yet, then look at the next character in the saveBits string
                else
                {
                    // If the value in the saveBits string is a 1, then or the output character with the powers of 2 
                    // If the value in the saveBits string is a 0, nothing needs to be done since the output char is set to 0 before any manipulation is done
                    // Finally increment the number of bits added to the output char
                    if (saveBits[b] == '1')
                        output |= P2[b];
                    bitNum++;
                }
            }
        }
        // Increment the total bytes in after every char
        totalIn++;
    }
    // Most likely saveBits will have some characters leftover, so padding bits might be needed
    if ((saveBits.size() > 0) && (saveBits.size() <= 8))
    {
        // First check how many padding bits are needed
        unsigned int paddingBits = 8 - saveBits.size();
        string longestString;
        // Find a bit string longer than the number of paddings bits you need
        for (unsigned int i = 0; i < 256; i++)
        {
            if (stringTable[i].length() > paddingBits)
                longestString = stringTable[i];
        }
        // Go through the bit string that was just found only the number of times that you need for padding
        for (unsigned int j = 0; j < paddingBits; j++)
        {
            // If the character in the bit string is a 1, then add a 1 to the saveBits bit string
            // Otherwise, add a zero
            if (longestString[j] == '1')
                saveBits += "1";
            else
                saveBits += "0";
        }
        // Now go through the saveBits string one last time, to add one more character to the output file
        for (unsigned int k = 0; k < 9; k++)
        {
            // If there have been 8 bits added to the char, then add the char to the output file, 
            // set the output file to 0, set the bitNum to 0, and erase the first 8 values of the saveBits string
            // Also, add 1 to the total bytes out
            if (bitNum > 7)
            {
                encodedFile.put(output);
                totalOut++;
                output = 0;
                bitNum = 0;
                saveBits.erase(0, 8);
            }
            // If there have not been 8 bits added to the char yet, then look at the next character in the saveBits string
            else
            {
                // If the value in the saveBits string is a 1, then or the output character with the powers of 2 
                // If the value in the saveBits string is a 0, nothing needs to be done since the output char is set to 0 before any manipulation is done
                // Finally increment the number of bits added to the output char
                if (saveBits[k] == '1')
                    output |= P2[k];
                bitNum++;
            }
        }
    }
    file.close(); // Close the input file
    encodedFile.close(); // Close the output file
}
void HUFF::decodeFile(string inputFile, string outputFile)
{
    // This method reads in the input file to create the tree which is then used to decode the input file and places the decoded information in the output file
    // IT reads in the input file character by character, creates the tree by reading the first 510 unsigned characters of the input file, which are the instructions on building the tree,
    // then it uses the tree to decode the rest of the characters
    unsigned char a;
    unsigned char treeInstructions[510] = { 0 };
    unsigned int count = 0;
    node* nodes[256] = { 0 };
    ifstream file;
    file.open(inputFile, ios::binary);
    // Check that the outputfile file is open
    // Otherwise say an error and exit the program
    if (!file.is_open())
    {
        cout << "Unable to open file";
        return;
    }
    ofstream outFile;
    outFile.open(outputFile, ios::trunc); // Clear the output file before adding anything in case the name of the output file was already used
    outFile.close();
    outFile.open(outputFile, ios::binary);
    // Check that the output file is open
    // Otherwise say an error and exit the program
    if (!outFile.is_open())
    {
        cout << "Unable to open file";
        return;
    }
    // Get each character from the file, place the chars in an array to keep track of the order, increase the frequency for the char by 1, and increase the total number of bytes in by 1
    for (int i = 0; i < 510; i++)
    {
        a = file.get();
        treeInstructions[count] = a;
        count++;
        totalIn++;
    }
    // Use the instructions that were represented by the first 510 unsigned chars of the input file to build the tree
    for (unsigned int b = 0; b < 510; b = b + 2)
    {
        int min1, min2;
        // Take the first two values in the array of the tree instructions
        min1 = treeInstructions[b];
        min2 = treeInstructions[b + 1];
        // Make those two values into nodes
        if (nodes[min1] == NULL)
        {
            nodes[min1] = new node;
            nodes[min1]->symbol = treeInstructions[b];
        }

        if (nodes[min2] == NULL)
        {
            nodes[min2] = new node;
            nodes[min2]->symbol = treeInstructions[b + 1];
        }
        // Make a parent node for the two instructions
        // Make the left child the first value 
        // Make the right child the second value
        // Put the parent in the location of the first min
        // Make the location of the second min null
        node* parent = new node;
        parent->left = nodes[min1];
        parent->right = nodes[min2];
        nodes[min1] = parent;
        nodes[min2] = nullptr;
    }
    root = nodes[0]; // The root should be in nodes[0] now
    unsigned char P2[] = { 128, 64, 32, 16, 8, 4, 2, 1 }; // Powers of 2
    // The rest of the file will be looked at to decode the file
    while (file.peek() != -1)
    {
        // First get the character and increase the total bytes in by 1 everytime
        a = file.get();
        totalIn++;
        for (unsigned int j = 0; j < 8; j++)
        {
            // For each character, check if the root is null. If so, break out of the method
            if (root == nullptr) break;
            // If the current character and-ed with the powers of two are true, and the right child is not null, then move right
            if (a & P2[j])
            {
                if (root->right != nullptr)
                    root = root->right;
            }
            // If the current character and-ed with the powers of two are false, and the left child is not null, then move left
            else
            {
                if (root->left != nullptr)
                    root = root->left;
            }
            // If the current character has no children, then put the current node's symbol into the output file, increase the total bytes out by 1, and set the root back to nodes[0]
            if ((root->left == nullptr) && (root->right == nullptr))
            {
                outFile.put(root->symbol);
                totalOut++;
                root = nodes[0];
            }
        }
    }
    file.close(); // close the input file
    outFile.close(); // close the output file
}
void HUFF::displayHelp() 
{
    // This method displays the five modes that the program supports with cout statements
    cout << "The five modes that are supported include: " << endl;
    cout << "Mode 1 - Show help : HUFF -h" << endl;
    cout << "                     HUFF -?" << endl;
    cout << "                     HUFF -help" << endl;
    cout << "Mode 2 - Encode Directly from Input File : HUFF -e file1 [file2]" << endl;
    cout << "Mode 3 - Decode : HUFF -d file1 file2" << endl;   
    cout << "Mode 4 - Create A Tree-Building File : HUFF -t file1 [file2]" << endl;
    cout << "Mode 5 - Encoding with a Specified Tree-Builder : HUFF -et file1 file2 [file3]" << endl;
}