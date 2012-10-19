#include "../include/graph.hpp"
#include <iostream>

using namespace std;

/******************genericC class function definitions***************/
genericC::genericC(){
	srcNext = NULL;
	sinkNext = NULL;
	param0 = -1;
	param1 = -1;
	node1 = -1;
	node2 = -1;
	node3 = -1;
};

genericC::genericC(gParam a, gParam b, int n1, int n2, int n3){
	srcNext = NULL;
	sinkNext = NULL;	
	param0 = a;
	param1 = b;
	node1 = n1; 
	node2 = n2;
	node3 = n3;
};

int* genericC::getNodes(){
	int* out;
	if(node3 > 0){
		out = new int[3];
		out[2] = node3;
	}
	else{
		out = new int[2];
	}

	out[0] = node1;
	out[1] = node2;
	return out;
}

void genericC::printAll(){
	cout << label;
	cout << ": " << param0 << " ";
	cout << "src = " << node1 << ", sink = " << node2 << endl;
}

void genericC::srcInsert(genericC *ptr){
	if(ptr == NULL) return;
	else if(srcNext == NULL) srcNext = ptr;
	else srcNext->srcInsert(ptr);
}

void genericC::sinkInsert(genericC *ptr){
	if(ptr == NULL) return;
	else if(sinkNext == NULL) sinkNext = ptr;
	else sinkNext->sinkInsert(ptr);
}

genericC *genericC::getSrcNext(){
	return srcNext;
}

genericC *genericC::getSinkNext(){
	return sinkNext;
}

/************Resistor class function definitions********************/

// Prints all the parameters and nodes
void resistor::printAll(){
	cout << label << ": " << param0 << " ohms from " << node1 << " to " << node2 << endl;
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

void resistor::insert(resistor *ptr){
	if(next == NULL) next = ptr;
	else next->insert(ptr);
}

resistor *resistor::getNext(){
	return next;
}

/**********voltageSource class function definitions****************
 *****************************************************************/
void voltageSource::printAll(){
	cout << label;
	if(param1 == 1) cout << " DC voltage source of ";
	else cout << " AC voltage source of ";
	cout << param0 << "V. ";
	cout << "+ve = " << node1 << ", -ve = " << node2 << endl;
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

void voltageSource::insert(voltageSource *ptr){
	if(next == NULL) next = ptr;
	else next->insert(ptr);
}

voltageSource *voltageSource::getNext(){
	return next;
}

/**********currentSource class function definitions****************
 *****************************************************************/
void currentSource::printAll(){
	cout << label;
	if(param1 == 1) cout << " DC current source of ";
	else cout << " AC current source of ";
	cout << param0 << "A. ";
	cout << "+ve = " << node1 << ", -ve = " << node2 << endl;
}

/* If acdc > 0, then set the value as DC i.e. param1 = 1 and if
 * acdc <= 0, then set the value as AC i.e. param1 = 0.
 */ 
void currentSource::setParameters(gParam amps, gParam acdc){
	param0 = amps;
	if(acdc > 0) param1 = 1;
	else param1 = 0;
}

gParam* currentSource::getParameters(){
	gParam* out;
	out = new gParam[2];
	out[0] = param0;
	out[1] = param1;
	return out;
}

// Only set the postive and negative nodes. The third is ignored
void currentSource::setNodes(int positive, int negative, int n3){
	node1 = positive;
	node2 = negative;
}

void currentSource::insert(currentSource *ptr){
	if(next == NULL) next = ptr;
	else next->insert(ptr);
}

currentSource *currentSource::getNext(){
	return next;
}

/******************Node Functions ********************************/
node::node(){
	srcList = NULL;
	sinkList = NULL;
}

void node::insertSrc(genericC *ptr){
	if (srcList != NULL) ptr->srcInsert(srcList);
	srcList = ptr;
}

void node::insertSink(genericC *ptr){
	if(sinkList != NULL) ptr->sinkInsert(sinkList);
	sinkList = ptr;
}

void node::insert(genericC *src, genericC *sink){
	insertSrc(src);
	insertSink(sink);
}

genericC *node::getSrcList(){
	return srcList;
}

genericC *node::getSinkList(){
	return sinkList;
}

/******************headNode Function Definitions*******************
 * Overloaded constructors */
headNode::headNode(){
	vsHead = NULL;
	rHead = NULL;
	iHead = NULL;
	vsEnd = NULL;
	rEnd = NULL;
	iEnd = NULL;
}
headNode::headNode(resistor *ptr){
	rHead = ptr;
	vsHead = NULL;
	vsEnd = NULL;
	rEnd = ptr;
	iEnd = NULL;
iHead = NULL;
}
headNode::headNode(voltageSource *ptr){
	vsHead = ptr;
	rHead = NULL;
	iHead = NULL;
	vsEnd = ptr;
	rEnd = NULL;
	iEnd = NULL;
}
headNode::headNode(currentSource *ptr){
	iHead = ptr;
	vsHead = NULL;
	rHead = NULL;
	vsEnd = NULL;
	rEnd = NULL;
	iEnd = ptr;
}

// Overloaded insert functions
void headNode::insert(voltageSource *ptr){
	if (ptr == NULL) return;
	if(vsHead != NULL) ptr->insert(vsHead);
	vsHead = ptr;
	if(vsEnd == NULL) vsEnd = ptr;
}

void headNode::insert(resistor *ptr){
	if (ptr == NULL) return;
	if(rHead != NULL) ptr->insert(rHead);
	rHead = ptr;
	if(rEnd == NULL) rEnd = ptr;
}

void headNode::insert(currentSource *ptr){
	if (ptr == NULL) return;
	if(iHead != NULL) ptr->insert(iHead);
	iHead = ptr;
	if(iEnd == NULL) iEnd = ptr;
}


// Merge function for two head nodes
void headNode::merge(headNode hN){
	bottomVS()->insert(hN.vsHead);
	bottomR()->insert(hN.rHead);
	bottomI()->insert(hN.iHead);
	vsEnd = hN.bottomVS();
	iEnd = hN.bottomI();
	rEnd = hN.bottomR();
}

// Overload top functions
voltageSource *headNode::topVS(){
	return vsHead;
}

resistor *headNode::topR(){
	return rHead;
}

currentSource *headNode::topI(){
	return iHead;
}

voltageSource *headNode::bottomVS(){
	return vsEnd;
}

resistor *headNode::bottomR(){
	return rEnd;
}

currentSource *headNode::bottomI(){
	return iEnd;
}
