/****************************************************************
 * This code is for the project for the course MA451 - Parallel
 * Computing in IIT Guwahati.
 *
 * This is the main file. It will parse the text input from an 
 * ascii file containing a circuit netlist. 
 ***************************************************************/ 

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "../include/graph.hpp"
#ifndef STRDEF
#include <string>
#endif

using namespace std;

/* This function is to convert a string value for resistance into
 * a float.
 */ 
float convert(string res);

int main(int argc, char **argv){
	if(argc < 2) {
		cout << "Usage: ./main.out filename" << endl;
		return -2;
	}

	/* Headnode contains pointers to a linked list of each kind of structure
	 * such as resistor, voltage source, capacitor, etc.
	 */ 
	headNode *hN;
	hN = new headNode();

	string line;			// Contains the current line processed
	ifstream in; 			// ifstream to read file	

	// Open the desired file
	in.open(argv[1]);
	if(!in){
		cerr << "Cannot open file for input.\n";
		return -1;
	}

	while(in.good()){
		string label, param1, param2;	// Holds label and parameters
		int n1, n2;			// Holds the value of nodes

		stringstream ss (stringstream::in | stringstream::out);			// Used to process the string
		getline(in, line);			// Read file line by line
		ss << line;
		char first = line[0];			// First character tells us what component it is
		switch(first){
			case 'r':
			case 'R':
				// Adding resistor
				// Using string stream to convert line of data into components
				ss >> label >> n1 >> n2 >> param1 ;

				// Creating new resistor object
				resistor *newRes;
				newRes = new resistor;
				newRes->setParameters(convert(param1));
				newRes->setNodes(n1, n2);
				newRes->setLabel(label);

				// Insert into headnode
				hN->insert(newRes);
				break;
			case 'v':
			case 'V':
				// Add voltage supply
				// Using string stream to convert line of data into components
				ss >> label >> n1 >> n2 >> param1 >> param2;
				float temp;
				if(param1 == "dc" || param1 == "DC") temp = 1;
				else temp = 0;

				// Creating new voltageSource object
				voltageSource *vsNew;
				vsNew = new voltageSource;
				vsNew->setParameters(atof(param2.c_str()), temp);
				vsNew->setNodes(n1, n2);
				vsNew->setLabel(label);

				// Insert into headNode
				hN->insert(vsNew);
				break;
			case '.':
				// Special case
				break;
			default:
				break;
		}
	}

	voltageSource *vs;
	vs = hN->topVS();

	while(vs != NULL){
		vs->printAll();
		vs = vs->getNext();
	}

	resistor *r;
	r = hN->topR();

	while(r != NULL){
		r->printAll();
		r = r->getNext();
	}
	return 0;
}

float convert(string res){
	float output = 0;
	output = atof(res.c_str());
	char last = res[res.length()-1];
	switch(last){
		case 'g':
		case 'G':
			output *= 1000;
		case 'm':
		case 'M':
			output *= 1000;
		case 'k':
		case 'K':
			output *= 1000;
			break;
		default:
			break;
	}
	return output;
}

