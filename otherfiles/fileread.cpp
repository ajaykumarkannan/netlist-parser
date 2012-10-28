#include <iostream>
#include <fstream>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[]){
	if(argc < 3) return -1;
	int nThreads; 

#pragma omp parallel
	{

#pragma omp master
		{
			cout << omp_get_num_threads() << endl;
		}
		ifstream in; 
		in.open(argv[1]);
		if(!in) {
			cerr << "Could not open file";
		} else {
			string line;
			while(in.good()){
				getline(in, line);
			}
		}
	}
}
