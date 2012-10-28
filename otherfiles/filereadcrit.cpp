#include <iostream>
#include <fstream>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[]){
	if(argc < 3) return -1;
	int nThreads; 
	ifstream in; 
	in.open(argv[1]);
	if(!in) {
		cerr << "Could not open file";
		return -1;
	}

#pragma omp parallel
	{

#pragma omp master
		{
			cout << omp_get_num_threads() << endl;
		}

		string line;

		while(in.good()){
#pragma omp critical			
			getline(in, line);
		}

	}
}
