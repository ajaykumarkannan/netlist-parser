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
#include <omp.h>
#include "../include/graph.hpp"
#ifndef STRDEF
#include <string>
#endif
#define m_iUseableProcessors	1

using namespace std;

// Global variables	
vector<node> nodeList (10);

/* This function is to convert a string value for resistance into
 * a float.
 */ 
float convert(string res);

/* This function is to merge two nodes
 */
void shortcircuit(int n1, int n2);

int main(int argc, char **argv){
	int DEBUG = 0;
	if(argc < 2) {
		cout << "Usage: ./main.out filename" << endl;
		return -2;
	}

	if(argc == 3) if (atoi(argv[2]) == 1) DEBUG = 1;

	unsigned int Nedges = 0;			// Contains the number of edges/components
	unsigned int Nnodes = 0;			// Contains the number of nodes
	unsigned int nodeTemp;

	string line;			// Contains the current line processed
	ifstream in; 			// ifstream to read file	

	// Open the desired file
	in.open(argv[1]);
	if(!in){
		cerr << "Cannot open file for input.\n";
		return -1;
	}

	int tid; 				// Thread id
	int nThreads;				// Number of threads
	bool valid = false;
	genericC *genPtr;

	// omp_set_num_threads(m_iUseableProcessors);

#pragma omp parallel private(genPtr, tid, line) shared(in, nThreads) reduction(+: Nedges)
	{
		Nnodes = 0;
		valid = false;
		string label, param1, param2;	// Holds label and parameters
		unsigned int n1, n2;			// Holds the value of nodes
		float temp;
		int nTemp = 0;
		while(in.good()){
			stringstream ss (stringstream::in | stringstream::out);			// Used to process the string

#pragma omp critical		
			{
				if(in.good()) {
					getline(in, line);			// Read file line by line
					valid = true;
				}
				else {
					valid = false;
				}
			}

			if(valid){
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
						valid = false;
						break;
					default:
						valid = false;
						break;
				}

				if(!valid) continue; 

				Nedges++;
				if(n1 > n2){
					if(n1 > Nnodes){
						nTemp = n1;
					}
					else nTemp = Nnodes;
				}
				else{
					if(n2 > Nnodes){
						nTemp = n2;	
					}
					else nTemp = Nnodes;
				}
#pragma omp critical
				{
					Nnodes = nTemp;
					// Insert into node list 
					if(nodeList.size() < Nnodes) nodeList.resize(Nnodes*2);
					nodeList[n1].insertSrc(genPtr);
					nodeList[n2].insertSink(genPtr);
				}
			}
		}
#pragma omp critical
		if(Nnodes > nodeTemp) nodeTemp = Nnodes;

	}

	Nnodes = nodeTemp;
	cout << "Done parsing\n";
	Nnodes = Nnodes + 1;
	cout << "Number of Nodes = " << Nnodes << ", Number of edges = " << Nedges << endl;

	shortcircuit(0,1);
	if(DEBUG){
		unsigned int nLSize = nodeList.size();
		cout << "Size of nodelist is " << nLSize << endl << endl;
		genericC *ptr;
		int *node;
		gParam *params;
		cout << "Sparse Adjacency Matrix with no connections eliminated - \n";
		cout << "Current node: list of nodes connected as sinks | list of nodes connected as sources\n";
		for (unsigned int i = 0; i < nLSize ; i++){
			if(nodeList[i].getSrcList() == NULL && nodeList[i].getSinkList() == NULL) continue;
			cout << i << ": ";
			ptr = nodeList[i].getSrcList();
			while(ptr != NULL){
				node = ptr->getNodes();
				params = ptr->getParameters();
				cout << node[1] << ":" << params[0] << " "; 
				ptr = ptr->getSrcNext();
			}

			cout << "| ";
			ptr = nodeList[i].getSinkList();

			while(ptr != NULL){
				node = ptr->getNodes();
				params = ptr->getParameters();
				cout << node[0] << ":" << params[0] << " "; 
				ptr = ptr->getSinkNext();
			}
			cout << endl;
		}
	}	

	in.close();
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

void shortcircuit(int n1, int n2){
	genericC *ptr = new genericC;

	// Represents short circuit
	ptr->setParameters(0, resistor);
	ptr->setNodes(n1, n2);
	ptr->setLabel("rsc");
	nodeList[n1].insertSrc(ptr);
	nodeList[n2].insertSink(ptr);
}
