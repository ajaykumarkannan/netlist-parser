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

	string line;

	vector<string> strlist;
	ifstream in; 	

	// Open the desired file
	in.open(argv[1]);
	if(!in){
		cerr << "Cannot open file for input.\n";
		return -1;
	}

	while(in.good()){
		string label, param1, param2;
		int n1, n2;

		stringstream ss (stringstream::in | stringstream::out);
		getline(in, line);			// Read line by line
		strlist.push_back(line);
		ss << line;
		char first = line[0];			// First character tells us what component it is
		switch(first){
			case 'r':
			case 'R':
				// Add resistor
				ss >> label >> n1 >> n2 >> param1 ;
				resistor *newRes;
				newRes = new resistor;
				newRes->setParameters(convert(param1));
				newRes->setNodes(n1, n2);
				newRes->setLabel(label);
				break;
			case 'v':
			case 'V':
				// Add voltage supply
				ss >> label >> n1 >> n2 >> param1 >> param2;
				float temp;
				if(param1 == "dc" || param1 == "DC") temp = 1;
				else temp = 0;
				voltageSource *vsNew;
				vsNew = new voltageSource;
				vsNew->setParameters(atof(param2.c_str()), temp);
				vsNew->setNodes(n1, n2);
				vsNew->setLabel(label);
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

