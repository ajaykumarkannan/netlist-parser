#define STRDEF
#include <string>
#include <omp.h>

// This class is for a generic component. It is abstract.
typedef float gParam;			
enum CompType{
	unset,
	resistor,
	voltageSrc,
	currentSrc,
	capacitor
};
/* The parameters are to be of the this form. 
 * Unused parameters and node points will have a -1 value.
 */ 

class genericC{
	protected:
		genericC *srcNext;
		genericC *sinkNext;

		// This is the type of the component
		enum CompType cType;

		// Atmost we need 2 parameters. We can add more if required.
		gParam param0;
		gParam param1;
		/* These are the nodes that the component is connected to. There are
		 * at more three nodes. 
		 */ 
		int node1;
		int node2;
		int node3;

		// Label for the module
		std::string label;
	public:
		// Constructor
		genericC();
		genericC(gParam a, gParam b = -1, enum CompType in = unset, int n1 = -1, int n2 = -1, int n3 = -1);

		// Destructor - Memory clean up
		~genericC();

		void printAll();
		void setParameters(gParam a, gParam b = -1, enum CompType in = unset);
		gParam * getParameters();
		void setLabel(std::string inLab){
			label = inLab;
		}

		std::string getLabel(){
			return label;
		}

		void setNodes(int n1, int n2, int n3 = -1);
		int* getNodes();

		void srcInsert(genericC *ptr);
		void sinkInsert(genericC *ptr);
		genericC *getSrcNext();
		genericC *getSinkNext();
};


/* This class is to store the nodes that are present in the circuit.
 * It will have a list of edges as well for each node. This will point
 * to the data structure of the circuit element. 
 */ 
class node{
	genericC *srcList;
	genericC *sinkList;

	// Locks for the two linked list
	omp_lock_t srcLock;
	omp_lock_t sinkLock;
	public:
	node();
	void insertSrc(genericC *ptr);
	void insertSink(genericC *ptr);
	void insert(genericC *src, genericC *sink);
	genericC *getSrcList();
	genericC *getSinkList();
};
