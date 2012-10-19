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
	headNode *main;					// Main headnode
	headNode *hN;					// Head node for each thread
	headNode *localhN;				// Local head node array

	genericC *ptr;

	main = new headNode();
	unsigned int Nedges = 0;			// Contains the number of edges/components
	unsigned int Nnodes = 0;			// Contains the number of nodes
	unsigned int nodeTemp = 0;
	vector<node> nodeList (10);

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

	// omp_set_num_threads(m_iUseableProcessors);

#pragma omp parallel private(ptr, tid, line, hN, Nnodes) shared(in, nThreads, localhN, main) reduction(+: Nedges)
	{
		ptr = NULL;
		Nnodes = 0;
		tid = omp_get_thread_num();
		string label, param1, param2;	// Holds label and parameters
		unsigned int n1, n2;			// Holds the value of nodes
		float temp;

#pragma omp single
		{
			nThreads = omp_get_num_threads();
			cout << nThreads << " threads" << endl;
			localhN = new headNode[nThreads];
		}

		cout << tid << endl;
		hN = &localhN[tid];

		while(in.good()){
			bool valid = true;
			stringstream ss (stringstream::in | stringstream::out);			// Used to process the string

#pragma omp critical		
			{
				if(in.good()) getline(in, line);			// Read file line by line
				else {
					valid = false;
				}
			}

			if(valid){
				ss << line;
				char first = line[0];			// First character tells us what component it is
				switch(first){
					case 'r':
					case 'R':
						// Adding resistor
						// Using string stream to convert line of data into components
						ss >> label >> n1 >> n2 >> param1;

						// Creating new resistor object
						resistor *newRes;
						newRes = new resistor;
						newRes->setParameters(convert(param1));
						newRes->setNodes(n1, n2);
						newRes->setLabel(label);

						// Insert into headnode
						hN->insert(newRes);
						Nedges++;
						ptr = (genericC *) newRes;
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
						ptr = (genericC *) vsNew;
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
						ptr = (genericC *)iNew;
						break;
					case '.':
						// Special case
						valid = false;
						break;
					default:
						valid = false;
						break;
				}

#pragma omp critical
				{					if(valid){
											 if(n1 > Nnodes) Nnodes = n1;
											 if(n2 > Nnodes) Nnodes = n2;
											 if(nodeList.size() < Nnodes) nodeList.resize(Nnodes*2);
											 // Insert into node list 
											 nodeList[n1].insertSrc(ptr);
											 nodeList[n2].insertSink(ptr);
										 }
				}
			}
		}

#pragma omp single
		{
			main = &localhN[0];
			for(int i = 1; i < nThreads; i++)
			{
				main->merge(localhN[i]);
			}
		}
#pragma omp critical
		if(Nnodes > nodeTemp) nodeTemp = Nnodes;

	}

	Nnodes = nodeTemp;
	cout << "Done parsing\n";
	Nnodes = Nnodes + 1;
	cout << "Number of Nodes = " << Nnodes << ", Number of edges = " << Nedges << endl;

	if(DEBUG){
		unsigned int nLSize = nodeList.size();
		cout << "Size of nodelist is " << nLSize << endl << endl;
		genericC *ptr;
		int *node;
		cout << "Sparse Adjacency Matrix with no connections eliminated. The printed format is: \n";
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

