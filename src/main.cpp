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

	/* Headnode contains pointers to a linked list of each kind of structure
	 * such as resistor, voltage source, capacitor, etc.
	 */ 
	headNode *hN;
	hN = new headNode();
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

	while(in.good()){
		string label, param1, param2;	// Holds label and parameters
		unsigned int n1, n2;			// Holds the value of nodes
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

				// Insert into node list 
				if(nodeList.size() < Nnodes) nodeList.resize(Nnodes*2);
				nodeList[n1].insertSrc((genericC *) newRes);
				nodeList[n2].insertSink((genericC *) newRes);
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

				// Insert into node list 
				if(nodeList.size() < Nnodes) nodeList.resize(Nnodes*2);
				nodeList[n1].insertSrc((genericC *) vsNew);
				nodeList[n2].insertSink((genericC *) vsNew);
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

				// Insert into node list 
				if(nodeList.size() < Nnodes) nodeList.resize(Nnodes*2);
				nodeList[n1].insertSrc((genericC *) iNew);
				nodeList[n2].insertSink((genericC *) iNew);
				break;
			case '.':
				// Special case
				break;
			default:
				break;
		}
	}

	cout << "Done parsing\n";
	Nnodes = Nnodes + 1;
	cout << "Number of Nodes = " << Nnodes << ", Number of edges = " << Nedges << endl;
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

	if(DEBUG){
		for (int i = 0; i < total; i++){
			cout << adjacency[i] << " ";
			if((i+1) % Nnodes == 0) cout << endl;
		}
		cout << endl;

		unsigned int nLSize = nodeList.size();
		cout << "Size of nodelist is " << nLSize << endl << endl;
		genericC *ptr;
		for (unsigned int i = 0; i < nLSize ; i++){
			if(nodeList[i].getSrcList() == NULL && nodeList[i].getSinkList() == NULL) continue;
			cout << "Node " << i << ":\n";
			cout << "Source List:\n";
			ptr = nodeList[i].getSrcList();
			while(ptr != NULL){
				ptr->printAll();
				ptr = ptr->getSrcNext();
			}

			cout << "Sink List: \n";
			ptr = nodeList[i].getSinkList();

			while(ptr != NULL){
				ptr->printAll();
				ptr = ptr->getSinkNext();
			}
			cout << endl;
		}
	}	
	delete [] adjacency;

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

