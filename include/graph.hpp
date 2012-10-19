#define STRDEF
#include <string>

// This class is for a generic component. It is abstract.
typedef float gParam;			
/* The parameters are to be of the this form. 
 * Unused parameters and node points will have a -1 value.
 */ 

class genericC{
	protected:
		genericC *srcNext;
		genericC *sinkNext;

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
		genericC(gParam a, gParam b = -1, int n1 = -1, int n2 = -1, int n3 = -1);

		// Destructor - Memory clean up
		~genericC();

		void printAll();
		virtual void setParameters(gParam a, gParam b) = 0;
		virtual gParam * getParameters() = 0;
		virtual void setNodes(int n1, int n2, int n3) = 0;
		void setLabel(std::string inLab){
			label = inLab;
		}
		int* getNodes();
		std::string getLabel(){
			return label;
		}

		void srcInsert(genericC *ptr);
		void sinkInsert(genericC *ptr);
		genericC *getSrcNext();
		genericC *getSinkNext();
};

class resistor : public genericC{
	resistor* next;
	public:
	resistor() : genericC(){
		next = NULL;
	}

	resistor(gParam resistance, int n1, int n2) : genericC(resistance, -1, n1, n2, -1){
		next = NULL;
	}

	~resistor(){
		if(next != NULL) {
			next->~resistor();
		}
	}

	void printAll();
	void setParameters(gParam a, gParam b = -1);
	gParam* getParameters();
	void setNodes(int n1, int n2, int n3 = -1);
	void insert(resistor *ptr);
	resistor *getNext();
};

class voltageSource : public genericC{
	voltageSource *next;
	public:
	/************************************************************
	 * volts is the voltage associated with the power source.
	 * acdc is whether the source is an ac source or a dc source. 
	 * Let us fix the convention for ac source, the value is 0.
	 * For dc source, the value is 1.
	 * The node order is important in the case of dc sources. The 
	 * first node is the positive terminal and the second node 
	 * is the negative terminal.
	 ***********************************************************/ 		
	voltageSource() : genericC(){
		next = NULL;
	}

	voltageSource(gParam volts, gParam acdc, int positiveNode, int negativeNode) : genericC(volts, acdc, positiveNode, negativeNode, -1){
		next = NULL;
	}

	~voltageSource(){
		if(next != NULL) {
			next->~voltageSource();
		}
	}

	void printAll();
	void setParameters(gParam volts, gParam acdc);
	void setNodes(int positive, int negative, int n3 = -1);
	gParam* getParameters();
	void insert(voltageSource *ptr);
	voltageSource *getNext();
};

class currentSource : public genericC{
	currentSource *next;
	public:
	/************************************************************
	 * volts is the voltage associated with the power source.
	 * acdc is whether the source is an ac source or a dc source. 
	 * Let us fix the convention for ac source, the value is 0.
	 * For dc source, the value is 1.
	 * The node order is important in the case of dc sources. The 
	 * first node is the positive terminal and the second node 
	 * is the negative terminal.
	 ***********************************************************/ 		
	currentSource() : genericC(){
		next = NULL;
	}
	currentSource(gParam amps, gParam acdc, int positiveNode, int negativeNode) : genericC(amps, acdc, positiveNode, negativeNode, -1){
		next = NULL;
	}

	~currentSource() {
		if(next != NULL) {
			next->~currentSource();
		}
	}

	void printAll();
	void setParameters(gParam amps, gParam acdc);
	void setNodes(int positive, int negative, int n3 = -1);
	gParam* getParameters();
	void insert(currentSource *ptr);
	currentSource *getNext();
};

/* This class is to store the nodes that are present in the circuit.
 * It will have a list of edges as well for each node. This will point
 * to the data structure of the circuit element. 
 */ 
class node{
	genericC *srcList;
	genericC *sinkList;
	public:
	node();
	void insertSrc(genericC *ptr);
	void insertSink(genericC *ptr);
	void insert(genericC *src, genericC *sink);
	genericC *getSrcList();
	genericC *getSinkList();
};

/* This class is for a linked list of all the inputs to allow nodes
 * of the class definition for any particular type. Each type of 
 * input data has a different linked list. This should simplify 
 * the making of the graph data structure.
 */
class headNode{
	voltageSource *vsHead;
	voltageSource *vsEnd;
	resistor *rHead;
	resistor *rEnd;
	currentSource *iHead;
	currentSource *iEnd;

	public:
	// Overloaded Constructors
	headNode();
	headNode(resistor *ptr);
	headNode(voltageSource *ptr);
	headNode(currentSource *ptr);

	// Node insert function
	void insert(voltageSource *ptr);
	void insert(resistor *ptr);
	void insert(currentSource *ptr);
	
	// Merge two head nodes
	void merge(headNode hN);

	// Returns top node
	voltageSource *topVS();
	resistor *topR();
	currentSource *topI();

	// Returns bottom node
	voltageSource *bottomVS();
	resistor *bottomR();
	currentSource *bottomI();
};
