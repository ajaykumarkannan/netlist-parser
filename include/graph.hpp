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
		genericC(){
			param0 = -1;
			param1 = -1;
			node1 = -1;
			node2 = -1;
			node3 = -1;
		};

		genericC(gParam a, gParam b = -1, int n1 = -1, int n2 = -1, int n3 = -1){
			param0 = a;
			param1 = b;
			node1 = n1; 
			node2 = n2;
			node3 = n3;
		};


		virtual void printAll() = 0;
		virtual void setParameters(gParam a, gParam b) = 0;
		virtual gParam * getParameters() = 0;
		virtual void setNodes(int n1, int n2, int n3) = 0;
		virtual int* getNodes() = 0;
		void setLabel(std::string inLab){
			label = inLab;
		}
		std::string getLabel(){
			return label;
		}
};

class resistor : public genericC{
	public:
		resistor() : genericC (){
			// Empty
		}

		resistor(gParam resistance, int n1, int n2) : genericC(resistance, -1, n1, n2, -1){
			// Calls the generic structure
		}

		void printAll();
		void setParameters(gParam a, gParam b = -1);
		gParam* getParameters();
		void setNodes(int n1, int n2, int n3 = -1);
		int* getNodes();

};

class voltageSource : public genericC{
	public:
		voltageSource() : genericC(){
			// Empty
		}

		/************************************************************
		 * volts is the voltage associated with the power source.
		 * acdc is whether the source is an ac source or a dc source. 
		 * Let us fix the convention for ac source, the value is 0.
		 * For dc source, the value is 1.
		 * The node order is important in the case of dc sources. The 
		 * first node is the positive terminal and the second node 
		 * is the negative terminal.
		 ***********************************************************/ 
		voltageSource(gParam volts, gParam acdc, int positiveNode, int negativeNode) : genericC(volts, acdc, positiveNode, negativeNode, -1){
			// Calls the generic structure
		}

		void printAll();
		void setParameters(gParam volts, gParam acdc);
		void setNodes(int positive, int negative, int n3 = -1);
		int* getNodes();
		gParam* getParameters();
};

class graph{


};
