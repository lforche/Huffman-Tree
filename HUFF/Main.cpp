/* This is the file with the main file for Lab 2 where a Huffman Tree that is used to encode and decode files is created.
  Name: Lindsey Forche
  Class: EECS 2510-001 (Non-linear Data Structures)
  Due Date: 3/17/2020 */

#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <locale>
#include <iostream>
#include "HUFF.h"

using namespace std;

int main(int argc, char* argv[])
{
    // This is the main class where the command line is used to call the methods
    HUFF* h = new HUFF;                                             // Calls the constructor

    if (argc > 1)                                                   
    {
        // First checks for the name of the program and the mode that the user wants
        // The first value (argv[0]) will be the name of the program, 
        // so the next value inputted will be the mode that the user wants to conduct the program in
        string mode = argv[1];    

        // No time is needed to be displayed for the help function
        // so if any of the following are the modes inputted, then only the help function will be displayed
        if ((mode == "-h") || (mode == "-?") || (mode == "-help"))
        {
            h->displayHelp();
        }
        // If it is not the display help function but it is one of the four other modes, then the elapsed time should be displayed
        else
        {
            double startClock = clock();                            // Start the clock

            // Check what mode the user wants to use
            // Then check how many files they have included
            if (mode == "-e")                                       
            {
                // This is the encode mode
                // It only requires an input file                
                if (argc > 2)
                {
                    string inputFile = argv[2];
                    string outputFile;
                    // If it has an input and output file, then it will set the output file given as the output file
                    // If it does not have an output file, then first the index of the input file's extension needs to be found
                    // If there is an extension, then the extension is replaced with ".huf", and that is the name of the new output file
                    // Otherwise, ".huf" needs to be added to the input file's name, and that is the name of the new output file
                    if (argc > 3)
                        outputFile = argv[3];
                    else
                    {
                        outputFile = inputFile;
                        int indexExtension = outputFile.find_last_of(".");
                        if (indexExtension > -1)
                            outputFile.replace(indexExtension, 4, ".huf");
                        else
                            outputFile += ".huf";
                    }
                    // Check that the input file and output file are not the same
                    // Then use the input and output file to encode the input file
                    if (inputFile != outputFile)
                        h->encodeFile(inputFile, outputFile);
                    // IF the two files are the same, then an error is printed out
                    else
                    {
                        cout << "The input and output files cannot be the same" << endl;
                        return 0;
                    }
                }
                // If there was less than the program name, the mode, and the input files, then an error will display
                else
                {
                    cout << "The correct usage of -e is: HUFF -e file1 [file2]" << endl;
                    return 0;
                }
            }
            else if (mode == "-d")
            {
                // This is the decode mode                
                if (argc > 3)
                {
                    // It requires an input and output file
                    // If it has an input and output file, then it will set the output file given as the output file
                    string inputFile = argv[2];
                    string outputFile = argv[3];
                    // Check that the input file and output file are not the same
                    // Then use the input and output file to encode the input file
                    if (inputFile != outputFile)
                        h->decodeFile(inputFile, outputFile);
                    // IF the two files are the same, then an error is printed out
                    else
                    {
                        cout << "The input and output files cannot be the same" << endl;
                        return 0;
                    }
                }
                // If there was less than the program name, the mode, and two files, then an error will display
                else
                {
                    cout << "The correct usage of -d is: HUFF - d File1 File2" << endl;
                    return 0;
                }

            }
            else if (mode == "-t")
            {
                // This is the tree builder mode
                if (argc > 2)
                {
                    // It only requires an input file
                    string inputFile = argv[2];
                    string outputFile;
                    // If it has an input and output file, then it will set the output file given as the output file
                    // If it does not have an output file, then first the index of the input file's extension needs to be found
                    // If there is an extension, then the extension is replaced with ".htree", and that is the name of the new output file
                    // Otherwise, ".htree" needs to be added to the input file's name, and that is the name of the new output file
                    if (argc > 3)
                        outputFile = argv[3];
                    else
                    {
                        outputFile = inputFile;
                        int indexExtension = outputFile.find_last_of(".");
                        if (indexExtension > -1)
                            outputFile.replace(indexExtension, 4, ".htree");
                        else
                            outputFile += ".htree";
                    }
                    // Check that the input file and output file are not the same
                    if (inputFile != outputFile)
                        h->makeTreeBuilder(inputFile, outputFile);
                    // IF the two files are the same, then an error is printed out
                    else
                    {
                        cout << "The input and output files cannot be the same" << endl;
                        return 0;
                    }
                }
                // If there was less than the program name, the mode, and two files, then an error will display
                else
                {
                    cout << "The correct usage of -t is: HUFF -t file1 [file2]" << endl;
                    return 0;
                }
            }
            else if (mode == "-et")
            {
                // This is the encode with tree builder file mode
                if (argc > 3)
                {
                    // It only requires an input and tree builder file
                    string inputFile = argv[2];
                    string treeFile = argv[3];
                    string outputFile;

                    // If it has an input, tree, and output file, then it will set the output file given as the output file
                    // If it does not have an output file, then first the index of the input file's extension needs to be found
                    // If there is an extension, then the extension is replaced with ".huf", and that is the name of the new output file
                    // Otherwise, ".huf" needs to be added to the input file's name, and that is the name of the new output file
                    if (argc > 4)
                        outputFile = argv[4];
                    else
                    {
                        outputFile = inputFile;
                        int indexExtension = outputFile.find_last_of(".");
                        if (indexExtension > -1)
                            outputFile.replace(indexExtension, 4, ".huf");
                        else
                            outputFile += ".huf";
                    }
                    // Check that all of the files are not the same
                    // Then use the input, tree, and output file to encode the input file
                    if ((inputFile != outputFile) && (inputFile != treeFile) && (treeFile != outputFile))
                        h->encodeFileWithTree(inputFile, treeFile, outputFile);
                    // IF the two files are the same, then an error is printed out
                    else
                    {
                        cout << "The input, tree, and output files cannot be the same" << endl;
                        return 0;
                    }
                }
                // If there was less than the program name, the mode, and two files, then an error will display
                else
                {
                    cout << "The correct usage of -e is:  HUFF -et file1 file2 [file3]" << endl;
                    return 0;
                }
            }
            // If the mode inputted was not one of the five modes, then an error will display, the display help function is shown, and the program is exited
            else
            {
                cout << "That is not a mode." << endl;
                h->displayHelp();
                return 0;
            }
            double time = clock() - startClock;                     // Find the elapsed time
            double totalTime = time / (double)CLOCKS_PER_SEC;       // Convert the clock ticks found using clock() to seconds
            // The following sets the time to be displayed with 3 decimal places after the decimal point and commas when appropriate
            cout.setf(ios::fixed);
            cout.setf(ios::showpoint);
            cout.precision(3);
            cout.imbue(locale(""));
            // Print out the total elapsed time, the total bytes in, and the total bytes out
            cout << "Time: " << totalTime << " seconds. " << h->totalIn << " bytes in / " << h->totalOut << " bytes out" << endl;
        }
    }
    // If there was not at least the name of the program, then an error will display
    else
        cout << "For any of the modes, there needs to be at least the name of the program, one mode, and the input file. Some modes require more files." << endl;
    return 0; // exit the program
}