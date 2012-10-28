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
	cType = unset;
};

genericC::genericC(gParam a, gParam b, CompType in, int n1, int n2, int n3){
	srcNext = NULL;
	sinkNext = NULL;	
	param0 = a;
	param1 = b;
	node1 = n1; 
	node2 = n2;
	node3 = n3;
	cType = in;
};

genericC::~genericC(){

};

void genericC::setNodes(int n1, int n2, int n3){
	node1 = n1;
	node2 = n2;
	node3 = n3;
}

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

void genericC::setParameters(gParam a, gParam b, enum CompType in){
	cType = in;
	param0 = a;
	param1 = b;
}

gParam *genericC::getParameters(){
	gParam *out; 
	if(param1 == -1) {
		out = &param0;
	}
	else {
		out = new gParam[2];
		out[0] = param0;
		out[1] = param1;
	}

	return out;
}

/******************Node Functions******************************/
node::node(){
	srcList = NULL;
	sinkList = NULL;
	omp_init_lock(&srcLock);
	omp_init_lock(&sinkLock);
}

void node::insertSrc(genericC *ptr){
	omp_set_lock(&srcLock);
	if (srcList != NULL) ptr->srcInsert(srcList);
	srcList = ptr;
	omp_unset_lock(&srcLock);
}

void node::insertSink(genericC *ptr){
	omp_set_lock(&sinkLock);
	if(sinkList != NULL) ptr->sinkInsert(sinkList);
	sinkList = ptr;
	omp_unset_lock(&sinkLock);
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
