#include "../include/graph.hpp"
#include <iostream>

using namespace std;

/************Resistor class function definitions******************
 *****************************************************************/

// Prints all the parameters and nodes
void resistor::printAll(){
	cout << "R = " << param0 << " ohms\n";
	cout << "Node1 = " << node1 << endl;
	cout << "Node2 = " << node2 << endl;
}

void resistor::setParameters(gParam res, gParam b){
	param0 = res;
}

void resistor::setNodes(int n1, int n2, int n3){
	node1 = n1; 
	node2 = n2;
}

gParam* resistor::getParameters(){
	gParam* out;
	out = new gParam;
	out[0] = param0;
	return out;
}

int* resistor::getNodes(){
	int* out;
	out = new int[2];
	out[0] = node1;
	out[1] = node2;
	return out;
}

/**********voltageSource class function definitions****************
 *****************************************************************/
void voltageSource::printAll(){
	if(param1 == 1) cout << "DC source of ";
	else cout << "AC source of ";
	cout << param0 << " volts\n";
	cout << "Positive Terminal = " << node1 << endl;
	cout << "Negative Terminal = " << node2 << endl;
}

/* If acdc > 0, then set the value as DC i.e. param1 = 1 and if
 * acdc <= 0, then set the value as AC i.e. param1 = 0.
 */ 
void voltageSource::setParameters(gParam volts, gParam acdc){
	param0 = volts;
	if(acdc > 0) param1 = 1;
	else param1 = 0;
}

gParam* voltageSource::getParameters(){
	gParam* out;
	out = new gParam[2];
	out[0] = param0;
	out[1] = param1;
	return out;
}

// Only set the postive and negative nodes. The third is ignored
void voltageSource::setNodes(int positive, int negative, int n3){
	node1 = positive;
	node2 = negative;
}

int* voltageSource::getNodes(){
	int* out;
	out = new int[2];
	out[0] = node1;
	out[1] = node2;
	return out;
}
