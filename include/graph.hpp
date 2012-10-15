#define STRDEF
#include <string>

// This class is for a generic component. It is abstract.
typedef float gParam;			
/* The parameters are to be of the this form. 
 * Unused parameters and node points will have a -1 value.
 */ 

class genericC{
	protected:
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
		virtual void printAll() = 0;
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
};

class resistor : public genericC{
	resistor* next;
	public:
		resistor() : genericC (){
			next = NULL;
		}

		resistor(gParam resistance, int n1, int n2) : genericC(resistance, -1, n1, n2, -1){
			next = NULL;
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
		void printAll();
		void setParameters(gParam volts, gParam acdc);
		void setNodes(int positive, int negative, int n3 = -1);
		gParam* getParameters();
		void insert(voltageSource *ptr);
		voltageSource *getNext();
};

/* This class is for a linked list of all the inputs to allow nodes
 * of the class definition for any particular type. Each type of 
 * input data has a different linked list. This should simplify 
 * the making of the graph data structure.
 */
class headNode{
	voltageSource *vsHead;
	resistor *rHead;
	public:
	// Overloaded Constructors
	headNode();
	headNode(resistor *ptr);
	headNode(voltageSource *ptr);
	
	// Node insert function
	void insert(voltageSource *ptr);
	void insert(resistor *ptr);

	// Returns top node
	voltageSource *topVS();
	resistor *topR();

};

// This class contains the actual graph
class graph{
	int Nedges;		// Number of edges
	int Nnodes;		// Number of nodes
public:
	graph();
};
