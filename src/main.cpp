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
	int Nedges = 0; 			// Contains the number of edges/components
	int Nnodes = 0;			// Contains the number of nodes

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
		float temp;

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
				Nedges++;
				if(n1 > Nnodes) Nnodes = n1;
				if(n2 > Nnodes) Nnodes = n2;
				break;
			case 'v':
			case 'V':
				// Add voltage supply
				// Using string stream to convert line of data into components
				ss >> label >> n1 >> n2 >> param1 >> param2;
				if(param1 == "ac" || param1 == "AC") temp = 0;
				else if (param1 == "dc" || param1 == "DC") temp = 1;
				else {
					temp = 1;
					param2 = param1;
				}

				// Creating new voltageSource object
				voltageSource *vsNew;
				vsNew = new voltageSource;
				vsNew->setParameters(atof(param2.c_str()), temp);
				vsNew->setNodes(n1, n2);
				vsNew->setLabel(label);

				// Insert into headNode
				hN->insert(vsNew);
				Nedges++;
				if(n1 > Nnodes) Nnodes = n1;
				if(n2 > Nnodes) Nnodes = n2;
				break;
			case 'i':
			case 'I':
				// Add current supply
				// Using string stream to convert line of data into components
				ss >> label >> n1 >> n2 >> param1 >> param2;
				if(param1 == "ac" || param1 == "AC") temp = 0;
				else if (param1 == "dc" || param1 == "DC") temp = 1;
				else {
					temp = 1;
					param2 = param1;
				}

				// Creating new currentSource object
				currentSource *iNew;
				iNew = new currentSource;
				iNew->setParameters(atof(param2.c_str()), temp);
				iNew->setNodes(n1, n2);
				iNew->setLabel(label);

				// Insert into headNode
				hN->insert(iNew);
				Nedges++;
				if(n1 > Nnodes) Nnodes = n1;
				if(n2 > Nnodes) Nnodes = n2;
				break;
			case '.':
				// Special case
				break;
			default:
				break;
		}
	}

	Nnodes = Nnodes +1;
	int adjacency[Nnodes][Nnodes];
	int *nodes = NULL;
	cout << Nnodes << " " << Nedges << endl;
	for (int i = 0; i < Nnodes; i++){
		for(int j = 0; j < Nnodes; j++) adjacency[i][j] = 0;
	}

	voltageSource *vs;
	vs = hN->topVS();

	while(vs != NULL){
		vs->printAll();
		nodes = vs->getNodes();
		adjacency[nodes[0]][nodes[1]] |= 2;
		adjacency[nodes[1]][nodes[0]] |= 4;
		vs = vs->getNext();
	}

	currentSource *is;
	is = hN->topI();

	while(is != NULL){
		is->printAll();
		nodes = is->getNodes();
		adjacency[nodes[0]][nodes[1]] |= 2;
		adjacency[nodes[1]][nodes[0]] |= 4;
		is = is->getNext();
	}

	resistor *r;
	r = hN->topR();

	while(r != NULL){
		r->printAll();
		nodes = r->getNodes();
		adjacency[nodes[0]][nodes[1]] |= 1;
		adjacency[nodes[1]][nodes[0]] |= 1;
		r = r->getNext();
	}
	for (int i = 0; i < Nnodes; i++){
		for(int j = 0; j < Nnodes; j++) {
			cout << adjacency[i][j] << " ";
		}
		cout << endl;
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

