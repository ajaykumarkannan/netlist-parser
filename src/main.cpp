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
	int DEBUG = 0;
	if(argc < 2) {
		cout << "Usage: ./main.out filename" << endl;
		return -2;
	}

	if(argc == 3) if (atoi(argv[2]) == 1) DEBUG = 1;

	unsigned int Nedges = 0;			// Contains the number of edges/components
	unsigned int Nnodes = 0;			// Contains the number of nodes
	vector<node> nodeList (10);

	string line;			// Contains the current line processed
	ifstream in; 			// ifstream to read file	

	// Open the desired file
	in.open(argv[1]);
	if(!in){
		cerr << "Cannot open file for input.\n";
		return -1;
	}
	
	bool valid = false;
	genericC *genPtr;
	
	while(in.good()){
		valid = false;
		string label, param1, param2;	// Holds label and parameters
		unsigned int n1, n2;			// Holds the value of nodes
		float temp;

		stringstream ss (stringstream::in | stringstream::out);			// Used to process the string
		getline(in, line);			// Read file line by line
		ss << line;
		char first = line[0];			// First character tells us what component it is
		
		// Using string stream to convert line of data into components
		ss >> label >> n1 >> n2 >> param1 >> param2;
		
		switch(first){
			case 'r':
			case 'R':
				// Adding resistor
				genPtr = new genericC;
				genPtr->setParameters(convert(param1), resistor);
				genPtr->setNodes(n1, n2);
				genPtr->setLabel(label);
				valid = true;
				break;
			case 'v':
			case 'V':
				// Add voltage supply
				if(param1 == "ac" || param1 == "AC") temp = 0;
				else if (param1 == "dc" || param1 == "DC") temp = 1;
				else {
					temp = 1;
					param2 = param1;
				}
				
				genPtr = new genericC;
				genPtr->setParameters(atof(param2.c_str()), temp, voltageSrc);
				genPtr->setNodes(n1, n2);
				genPtr->setLabel(label);

				valid = true;
				break;
			case 'i':
			case 'I':
				// Add current supply
				if(param1 == "ac" || param1 == "AC") temp = 0;
				else if (param1 == "dc" || param1 == "DC") temp = 1;
				else {
					temp = 1;
					param2 = param1;
				}

				genPtr = new genericC;
				genPtr->setParameters(atof(param2.c_str()), temp, currentSrc);
				genPtr->setNodes(n1, n2);
				genPtr->setLabel(label);

				valid = true;
				break;
			case '.':
				// Special case
				break;
			default:
				break;
		}
		if(valid){
			Nedges++;
			if(n1 > Nnodes) Nnodes = n1;
			if(n2 > Nnodes) Nnodes = n2;
			// Insert into node list 
			if(nodeList.size() < Nnodes) nodeList.resize(Nnodes*2);
			nodeList[n1].insertSrc((genericC *) genPtr);
			nodeList[n2].insertSink((genericC *) genPtr);
		}
	}

	cout << "Done parsing\n";
	Nnodes = Nnodes + 1;
	cout << "Number of Nodes = " << Nnodes << ", Number of edges = " << Nedges << endl;

	/*
	int *nodes = NULL;
	int total = Nnodes * Nnodes;
	int *adjacency = new int[total];
	for (int i = 0; i < total; i++) adjacency[i] = 0;

	voltageSource *vs;
	vs = hN->topVS();
	while(vs != NULL){
		// vs->printAll();
		nodes = vs->getNodes();
		adjacency[nodes[0]*Nnodes + nodes[1]] |= 2;
		adjacency[nodes[1]*Nnodes + nodes[0]] |= 4;
		vs = vs->getNext();
	}

	currentSource *is;
	is = hN->topI();

	while(is != NULL){
		// is->printAll();
		nodes = is->getNodes();
		adjacency[nodes[0]*Nnodes + nodes[1]] |= 8;
		adjacency[nodes[1]*Nnodes + nodes[0]] |= 16;
		is = is->getNext();
	}

	resistor *r;
	r = hN->topR();

	while(r != NULL){
		// r->printAll();
		nodes = r->getNodes();
		adjacency[nodes[0]*Nnodes + nodes[1]] |= 1;
		adjacency[nodes[1]*Nnodes + nodes[0]] |= 1;
		r = r->getNext();
	}

	*/

	if(DEBUG){
		/*
		for (int i = 0; i < total; i++){
			cout << adjacency[i] << " ";
			if((i+1) % Nnodes == 0) cout << endl;
		}
		cout << endl;
		*/

		unsigned int nLSize = nodeList.size();
		cout << "Size of nodelist is " << nLSize << endl << endl;
		genericC *ptr;
		int *node;
		cout << "Sparse Adjacency Matrix with no connections eliminated - \n";
		cout << "Current node: list of nodes connected as sinks | list of nodes connected as sources\n";
		for (unsigned int i = 0; i < nLSize ; i++){
			if(nodeList[i].getSrcList() == NULL && nodeList[i].getSinkList() == NULL) continue;
			cout << i << ": ";
			ptr = nodeList[i].getSrcList();
			while(ptr != NULL){
				node = ptr->getNodes();
				cout << node[1] << " "; 
				ptr = ptr->getSrcNext();
			}

			cout << "| ";
			ptr = nodeList[i].getSinkList();

			while(ptr != NULL){
				node = ptr->getNodes();
				cout << node[0] << " "; 
				ptr = ptr->getSinkNext();
			}
			cout << endl;
		}
	}	
	// delete [] adjacency;

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

