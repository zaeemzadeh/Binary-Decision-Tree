#include "Node.h"
#include "Functions.h"
#include <math.h>
#include <iostream>


// Constructor
Node::Node(vector<Record> *all_data, const vector<int> data_in_node, const int depth)
	: m_all_data(all_data) , m_data_in_node(data_in_node), m_depth(depth){

	m_Label = 0;
	m_threshold = -1;
}

// Destructor
Node::~Node(){
	for (int i = 0 ; i < m_Children.size(); i++){
		delete m_Children[i];
	}
	return;
}


void Node::Build_Tree(const vector <Attribute> candidate_atts){

	
	// First we check the stopping criterias

	if (candidate_atts.size() == 0){
		m_select_label();
		//cout << " return: No ATT = " << " Depth = " << m_depth << endl;
		return;
	}
	
	 if (Entropy() < MIN_ENTROPY){
		//cout << " return: Entropy = " << Entropy() << " Depth = " << m_depth <<  endl;
		m_select_label();		
		return;
	}

	if (m_data_size() < MIN_DATA_SIZE){		
		//cout << " return: Size = " << m_data_size()  << " Depth = " << m_depth << endl;	
		m_select_label();	
		return;
	}

	if (m_depth > MAX_DEPTH ){
		//cout << " return: Depth = " << m_depth << " Depth = " << m_depth << endl;		
		m_select_label();
		return;
	}

	m_decision_att = select_decision_attribute(candidate_atts);
	//cout << "best att: " << m_decision_att << endl;
	if (m_decision_att <0 || m_decision_att > 7){
		//cout << "decision att return: ATT = " << m_decision_att << " Depth = " << m_depth << endl;
		m_select_label();
		return;
	}

	// Then we compute the threshold (If necessary)
	if (m_decision_att != Sex)
		m_threshold = compute_threshold(m_decision_att);

	if (m_threshold == -1 && m_decision_att != Sex){
		m_select_label();
		//cout << " return: Thrshold = -1 " << endl;
		return;
	}

	// Then the children nodes are created
	m_create_children();

	// Remove the current attribute to avoid using repeating attributes
	vector <Attribute> next_candidate_atts;
	for (int i = 0; i < candidate_atts.size(); i++){
		if (candidate_atts[i] != m_decision_att)
			next_candidate_atts.push_back(candidate_atts[i]);
	}

	// Building the tree for children nodes
	for (int i = 0; i < m_Children.size(); i++){
		m_Children[i]->Build_Tree(next_candidate_atts);
	}
	return;
}

void Node::m_create_children(){
	if (m_decision_att == Sex){
		vector <int> data_m;
		vector <int> data_f;
		vector <int> data_i;
		for (int i = 0; i < m_data_size(); i++){
			int data_ind = m_data_in_node[i];
			Record current_record = (*m_all_data)[data_ind];
			// Split the data among the children nodes
			if (current_record.Sex == 'M')
				data_m.push_back(data_ind);
			else if (current_record.Sex == 'F')
				data_f.push_back(data_ind);
			else
				data_i.push_back(data_ind);
		}

		// create children nodes
		Node *child_m = new Node(m_all_data, data_m, m_depth+1);
		Node *child_f = new Node(m_all_data, data_f, m_depth+1);
		Node *child_i = new Node(m_all_data, data_i, m_depth+1);
		m_Children.push_back(child_m);
		m_Children.push_back(child_f);
		m_Children.push_back(child_i);
	}
	else {
		vector <int> data_child1;
		vector <int> data_child2;
		for (int i = 0; i < m_data_in_node.size(); i++){
			int data_ind = m_data_in_node[i];
			Record current_record = (*m_all_data)[data_ind];
			float field = (current_record.real_Attributes)[m_decision_att-1];
			// Split the data among the children nodes
			if (field < m_threshold)
				data_child1.push_back(data_ind);
			else 
				data_child2.push_back(data_ind);
		}
		// create children nodes
		Node *child_1 = new Node(m_all_data, data_child1, m_depth+1);
		Node *child_2 = new Node(m_all_data, data_child2, m_depth+1);

		m_Children.push_back(child_1);
		m_Children.push_back(child_2);
	}
	return;
}

Attribute Node::select_decision_attribute(const vector <Attribute> candidate_atts){
	float best_entropy = HUGE_VALF;
	Attribute best_att = candidate_atts[0];

	// testing all attributes to find the minimum Entropy
	for (int i =0; i < candidate_atts.size(); i++){
		m_decision_att = candidate_atts[i];
		if (m_decision_att != Sex)
			m_threshold = compute_threshold(m_decision_att);

		m_Children.clear();
		m_create_children();

		float curr_entropy = 0;
		// Weighted average over the children
		for (int child = 0; child < m_Children.size(); child++){
			curr_entropy += (float)m_Children[child]->Entropy()*m_Children[child]->m_data_size()
					/m_data_size();			
		}

		// choosing the better attribute
		if (best_entropy > curr_entropy ){
			best_entropy = curr_entropy;
			best_att = candidate_atts[i];
		}
	}
	m_Children.clear();
	return best_att;
}

float Node::Entropy(){
	const int Num_of_classes = 2;
	int data_size = m_data_size();

	if (data_size == 0)
		return 0;

	int frequency[Num_of_classes];
	for (int i = 0; i < Num_of_classes; i++){
		frequency[i] = 0;
	}
	
	for (int i = 0; i < m_data_size(); i++){
		int data_ind = m_data_in_node[i];
		Record curr_data = (*m_all_data)[data_ind];
		int label = 0.5 * curr_data.Rings + 0.5;  	// mapping labels into indeces 0,1
		frequency[label]++;				// counting different classes
	}
	
	// computing entropy
	float entropy = 0;
	for (int i = 0; i < Num_of_classes; i++){
		float p = (float)frequency[i]/data_size;
		if ( p == 0)
			continue;

		entropy -= p*log2(p);
	}
	return entropy;
}

float Node::compute_threshold(Attribute candidate_att){
	if (candidate_att == Sex)
		return -1;

	vector <float> labels;
	vector <float> attribute;
	
	// creating vector of attribute values and labels
	for (int i = 0; i < m_data_size(); i++){
		int data_ind = m_data_in_node[i];
		Record curr_data = (*m_all_data)[data_ind];

		labels.push_back((float)curr_data.Rings);
		attribute.push_back((curr_data.real_Attributes)[candidate_att-1]);
	}
		
	if (labels.size() == 0)	
		return -1;

	float w = variance(attribute,labels)/variance(attribute,attribute);
	float b = -w*mean(attribute) + mean(labels) ;
	
	if (-b/w > max(attribute) || -b/w < min(attribute) ){
		//cout << "Threshold out of bound : ATT = "  << candidate_att << " Depth: " << m_depth 
		//<< " Size: " << m_data_size() << endl;
		return (max(attribute) + min(attribute))/2;
	} 
		return -b/w;
}

void Node::m_select_label(){
	int num_of_L1 = 0;
	// select the label with highest frwquency in the node
	for (int i = 0; i < m_data_size(); i++){
		int data_ind = m_data_in_node[i];
		Record curr_data = (*m_all_data)[data_ind];
		
		if (curr_data.Rings == 1)
			num_of_L1 ++;
	}
	m_Label = (num_of_L1 > m_data_size()/2)? 1 : -1;
}

void Node::decide(){
	if (m_Children.size() == 0){
		m_set_label();
		return;
	}
	if (m_decision_att == Sex){
		vector <int> data_m;
		vector <int> data_f;
		vector <int> data_i;
		for (int i = 0; i < m_data_size(); i++){
			int data_ind = m_data_in_node[i];
			Record current_record = (*m_all_data)[data_ind];
			// split data among children nodes
			if (current_record.Sex == 'M')
				data_m.push_back(data_ind);
			else if (current_record.Sex == 'F')
				data_f.push_back(data_ind);
			else
				data_i.push_back(data_ind);
		}
		// split the data among grandchildren ....
		m_Children[0]->set_data(data_m);
		m_Children[0]->decide();
		m_Children[1]->set_data(data_f);
		m_Children[1]->decide();
		m_Children[2]->set_data(data_i);
		m_Children[2]->decide();
	}
	else {
		vector <int> data_child1;
		vector <int> data_child2;
		for (int i = 0; i < m_data_in_node.size(); i++){
			int data_ind = m_data_in_node[i];
			Record current_record = (*m_all_data)[data_ind];
			float field = (current_record.real_Attributes)[m_decision_att-1];
			// split data among nodes
			if (field < m_threshold)
				data_child1.push_back(data_ind);
			else 
				data_child2.push_back(data_ind);
		}
		// split the data among grandchildren ....
		m_Children[0]->set_data(data_child1);
		m_Children[0]->decide();
		m_Children[1]->set_data(data_child2);
		m_Children[1]->decide();
	}
	//m_set_label();
	return;

}

void Node::m_set_label(){
	for (int i = 0; i < m_data_size(); i++){
		int data_ind = m_data_in_node[i];
		((*m_all_data)[data_ind]).Label = m_Label;
	}
	return;
}



void Node::print_tree(){

	if (m_Label == 0){
		cout 	<< " Depth: " << m_depth
			<< " Attribute: " << m_decision_att
			<< " Threshold: " << m_threshold
			<< " #Children: " << m_Children.size()
			<< " Entropy: "  << Entropy()
			<< " #Data: "  << m_data_in_node.size()
			<< endl;
	} else {
		cout 	<< "LEAF NODE: "
			<< " Depth: " << m_depth
			<< " #Children: " << m_Children.size()
			<< " Entropy: "  << Entropy()
			<< " Label: " << m_Label
			<< " #Data: "  << m_data_in_node.size()
			<< endl;
}
	
	for (int i = 0; i < m_Children.size() ; i ++)
		m_Children[i]->print_tree();

	return;
}	
