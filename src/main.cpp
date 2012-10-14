/****************************************************************
 * This code is for the project for the course MA451 - Parallel
 * Computing in IIT Guwahati.
 *
 * This is the main file. It will parse the text input from an 
 * ascii file containing a circuit netlist. 
 ***************************************************************/ 

#include <iostream>
#include <string>
#include <fstream>
#include "../include/graph.hpp"

using namespace std;

int main(int argc, char **argv){
	if(argc < 2) {
		cout << "Usage: ./main.out filename" << endl;
		return -2;
	}

	string line;
	ifstream in; 	

	// Open the desired file
	in.open(argv[1]);
	if(!in){
		cerr << "Cannot open file for input.\n";
		return -1;
	}

	while(in.good()){
		getline(in, line);			// Read line by line
		char first = line[0];			// First character tells us what component it is
		switch(first){
			case 'r':
			case 'R':
				// Add resistor
				break;
			case 'v':
			case 'V':
				// Add voltage supply
				break;
			case '.':
				// Special case
				break;
			default:
				break;
		}
	}
	return 0;
}
