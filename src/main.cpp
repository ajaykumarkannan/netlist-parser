/****************************************************************
 * This code is for the project for the course MA451 - Parallel
 * Computing in IIT Guwahati.
 *
 * This is the main file. It will parse the text input from an 
 * ascii file containing a circuit netlist. 
 ***************************************************************/ 

#include <iostream>
#include <deque>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "../include/graph.hpp"
#ifndef STRDEF
#include <string>
#endif
#define m_iUseableProcessors	2
#define DEFAULTSIZE		100

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/microsec_time_clock.hpp>

class TestTimer
{
	public:
		TestTimer(const std::string & name) : name(name),
		start(boost::date_time::microsec_clock<boost::posix_time::ptime>::local_time())
	{
	}

		~TestTimer()
	{
		using namespace std;
		using namespace boost;

		posix_time::ptime now(date_time::microsec_clock<posix_time::ptime>::local_time());
		posix_time::time_duration d = now - start;

		cout << name << " completed in " << d.total_milliseconds() / 1000.0 <<
			" seconds" << endl;
	}

	private:
		std::string name;
		boost::posix_time::ptime start;
};

using namespace std;

// Global variables	
vector<node> nodeList (100);
// deque<node> nodeList (100);
// node *nodeList;

/* This function is to convert a string value for resistance into
 * a float.
 */ 
float convert(string res);

/* This function is to merge two nodes
 */
void shortcircuit(int n1, int n2);

int main(int argc, char **argv){

	int DEBUG = 0;		// Debug flag
	if(argc < 2) {
		cout << "Usage: ./main.out filename (number of nodes - Optional)" << endl;
		return -2;
	}

	if(argc == 3) {
		if (atoi(argv[2]) == 1) {
			DEBUG = 1;
			nodeList.reserve(DEFAULTSIZE);
			// nodeList = new node[DEFAULTSIZE];
		}
		else if(atoi(argv[2]) > 1) {
			// nodeList = new node[atoi(argv[2])]; 
			nodeList.reserve(atoi(argv[2]));
		}
	} else if (argc == 4) {
		if (atoi(argv[2]) == 1) DEBUG = 1;
		if(atoi(argv[3]) > 1){
			// nodeList = new node[atoi(argv[3])];
			nodeList.reserve(atoi(argv[3]));
		}
		else {
			//nodeList = new node[DEFAULTSIZE];
			nodeList.reserve(DEFAULTSIZE);
		}
	} else {
		nodeList.reserve(DEFAULTSIZE);
		// nodeList = new node[DEFAULTSIZE];
	}

	unsigned int Nedges = 0;			// Contains the number of edges/components
	unsigned int Nnodes = 0;			// Contains the number of nodes
	unsigned int nodeTemp = 0;

	string line;			// Contains the current line processed
	
	/*
	 // Open the desired file
	 in.open(argv[1]);
	 if(!in){
		 cerr << "Cannot open file for input.\n";
		 return -1;
}
*/

	int tid; 				// Thread id
	int nThreads;				// Number of threads
	bool valid = false;
	genericC *genPtr;

	omp_set_num_threads(m_iUseableProcessors);

	int nlines = 0;

	ifstream afile[m_iUseableProcessors];
	afile[0].open(argv[1]);
	while(afile[0].good()){
		nlines++;
		getline(afile[0], line);
	}
	
	afile[0].close();

	for (int i = 0; i < m_iUseableProcessors; i += 1){
		afile[i].open(argv[1]);
	}

	int top[m_iUseableProcessors];
	int bottom[m_iUseableProcessors];

	int incr = nlines / m_iUseableProcessors;
	int curr = 0;
	
	for (int i = 0; i < m_iUseableProcessors; i += 1){
		top[i] = curr;
		bottom[i] = curr + incr;
		curr += incr;
	}

	for (int i = 0; i < m_iUseableProcessors; i += 1){
		cout << top[i] << endl;
		cout << bottom[i] << endl;
	}

	{
		TestTimer t("Data structure population");
#pragma omp parallel private(genPtr, line, Nnodes, tid) shared(nThreads) reduction(+: Nedges)
		{
			int count = 0;
			tid = omp_get_thread_num();
			Nnodes = 0;
			valid = false;
			string label, param1, param2;	// Holds label and parameters
			unsigned int n1, n2;			// Holds the value of nodes
			float temp;
			int nTemp = 0;
			
#pragma omp master
			{
				nThreads = omp_get_num_threads();
				cout << nThreads << " threads\n";
			}

			while(afile[tid].good()){
				count++;

				// Only do a certain portion of the file
				if(count >= bottom[tid]) break;
				else if(count < top[tid]) {
					getline(afile[tid],line);
					continue;
				}

				stringstream ss (stringstream::in | stringstream::out);			// Used to process the string		
				getline(afile[tid],line);

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

				Nnodes = nTemp;
				if(nodeList.size() < Nnodes) {
					// Increase size of nodeList if required
#pragma omp critical
					nodeList.resize(Nnodes*2);
				}
				// Insert into node list 
				nodeList[n1].insertSrc(genPtr);
				nodeList[n2].insertSink(genPtr);
			}
#pragma omp critical
			if(Nnodes > nodeTemp) nodeTemp = Nnodes;

		}
	}
	Nnodes = nodeTemp;
	cout << "Done parsing\n";
	Nnodes = Nnodes + 1;
	cout << "Number of Nodes = " << Nnodes << ", Number of edges = " << Nedges << endl;

	//	shortcircuit(0,1);
	if(DEBUG){
		unsigned int nLSize = nodeList.size();
		cout << "Size of nodelist is " << nLSize << endl << endl;
		genericC *ptr;
		int *node;
		// gParam *params;
		cout << "Sparse Adjacency Matrix with no connections eliminated - \n";
		cout << "Current node: list of nodes connected as sinks | list of nodes connected as sources\n";
		for (unsigned int i = 0; i < nLSize ; i++){
			if(nodeList[i].getSrcList() == NULL && nodeList[i].getSinkList() == NULL) continue;
			cout << i << ": ";
			ptr = nodeList[i].getSrcList();
			while(ptr != NULL){
				node = ptr->getNodes();
				// params = ptr->getParameters();
				cout << node[1] << ":" << " " ; // params[0] << " "; 
				ptr = ptr->getSrcNext();
			}

			cout << "| ";
			ptr = nodeList[i].getSinkList();

			while(ptr != NULL){
				node = ptr->getNodes();
				// params = ptr->getParameters();
				cout << node[0] << ":" << " "; // params[0] << " "; 
				ptr = ptr->getSinkNext();
			}
			cout << endl;
		}
	}	
	
	for(int i = 0; i < m_iUseableProcessors; i++){ 
	afile[i].close();
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

void shortcircuit(int n1, int n2){
	genericC *ptr = new genericC;

	// Represents short circuit
	ptr->setParameters(0, resistor);
	ptr->setNodes(n1, n2);
	ptr->setLabel("rsc");
	nodeList[n1].insertSrc(ptr);
	nodeList[n2].insertSink(ptr);
}
