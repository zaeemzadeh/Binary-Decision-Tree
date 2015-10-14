#include <vector>
#include "Record.h"

using namespace std;
#ifndef NODE_H 
#define NODE_H

const int MAX_DEPTH = 8;	// Maximum Depth for stopping criteria
const float MIN_ENTROPY = 0.2;	// Minimum Entropy for stopping criteria
const int MIN_DATA_SIZE = 10;  	// Minimum Data size in node for stopping criteria

class Node{
private:
	vector <Node *> m_Children;		// Pointer to children nodes
	const int m_depth;			// Depth of the node in tree
	vector<int> m_data_in_node;		// indices of the data points in this node
	vector<Record> *m_all_data;		// pointer to the whole data set
	Attribute m_decision_att;		// decision attribute in this node
	int m_Label;				// Label of this node (Leaf node 1 or -1) (otherwise 0)
	float m_threshold;			// Threshold for continous attributes

	// Finds the best attribute for splitting data based on best Entropy Gain
	Attribute select_decision_attribute(const vector <Attribute> candidate_atts);
	// Computes the threshold for splitting data for contionous attributes
	float compute_threshold(Attribute candidate_att);
	// Computes the Entropy of the data points in current node
	float Entropy();
	// splits the data and creates the children nodes (training phase)
	void m_create_children();

	void m_remove_children();
	// Selects the label for leaf nodes  (training phase)
	void m_select_label();
	// sets the labes for data in leaf nodes (testing phase)
	void m_set_label();
public:
	// Constructor
	Node(vector<Record> *all_data, const vector<int> data_in_node, const int depth);
	// Destructor
	~Node();
	// Builds the Tree
	void Build_Tree(const vector <Attribute> candidate_atts);
	// splits the data among the children (testing phase)
	void decide();
	// returns the size of data in current node
	int m_data_size() {return m_data_in_node.size();};
	// sets the data for a node
	void set_data(vector<int> data_in_node){ m_data_in_node = data_in_node;};
	// prints  the node features 
	void print_tree();
};

#endif /* NODE_H */
