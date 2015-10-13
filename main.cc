#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include "Record.h"
#include "Node.h"
#include "Functions.h"
using namespace std;


int main(){
	vector <Record> data;			// Data vector

	// Opening dataset Abalone available online https://archive.ics.uci.edu/ml/datasets/Abalone
	ifstream InputFile;
	InputFile.open("abalone.data", fstream::in);	
	
	cout << "\n\n\n*************Abalone Data Set****************** \n";
	// turning the multi_class classification problem into binary classification
	// H0 and H1 should belong to the range [1,29]
	const int H0 = 8; 		// if Rings == 8 then label -1
	const int H1 = 11; 		// if Rings == 12 then label 1
	cout << "******* H0: Rings == " << H0 << "   H1: Rings == " << H1 << " ********* \n\n\n";
	read_data(data, InputFile, H0, H1);

	int NumOfFolds = 10;		// Number of folds for validation
	vector <Attribute> all_atts;	// Attributes to include in decision tree
	all_atts.push_back(Sex);	// comment out each line to exclude the attribute from the decision tree
	all_atts.push_back(Length);
	all_atts.push_back(Diameter);
	all_atts.push_back(Height);
	all_atts.push_back(WholeWeight);
	all_atts.push_back(ShuckedWeight);
	all_atts.push_back(VisceraWeight);
	all_atts.push_back(ShellWeight);
	/**/

	vector <float> accuracy;	// accuracy of different folds
	vector <float> precision;	// precision of different folds
	vector <float> recall;		// recall of different folds
	vector <float> F_measure;	// F_measure of different folds

	// Training and validating for different folds
	for (int BatchNum = 1; BatchNum <= NumOfFolds; BatchNum ++){
		vector<int> training_set;		// indices of the training set
		vector<int>	test_set;		// indices of the test set
		
		// Split data into training and test sets
		split_data(data, training_set, test_set, BatchNum, NumOfFolds);

		// creating the root node and intialize it with training data
		Node *head = new Node(&data,training_set,0);
		// Build the Decision Tree
		// The stopping criteria can be set in the header file "Node.h"
		head->Build_Tree(all_atts);

		// Feeding the test data and generating the results
		head->set_data(test_set);
		head->decide();

		// Computing Accuracy
		vector <int> confusion_mtx = Confusion_Matrix(data,test_set);
		int TP = confusion_mtx[0];
		int FN = confusion_mtx[1];
		int FP = confusion_mtx[2];
		int TN = confusion_mtx[3];

		accuracy.push_back((float)(TP+TN)/(TP+TN+FP+FN));
		precision.push_back((float)TP/(TP+FP));
		recall.push_back((float)TP/(TP+FN));
		F_measure.push_back((float)(2*TP)/(2*TP+FP+FN));

		cout << "Fold #" << BatchNum <<  " \t ACCURACY  = " << accuracy[BatchNum - 1] <<
						 "\t precision  = " << precision[BatchNum - 1] <<
						 "\t RECALL  = " << recall[BatchNum-1] <<
						 "\t F_Measure  = " << F_measure[BatchNum-1] <<  endl;
		//head->print_tree();
		delete head;
	}

	cout << "\n\n\n=============================================== \n";
	cout << "Average Performance Metrics: \n";
	cout << "AVG Accuracy:  \t" << mean(accuracy) << " \t STD: " << sqrt(variance(accuracy,accuracy)) << endl;
	cout << "AVG precision: \t" << mean(precision) << " \t STD: " << sqrt(variance(precision,precision)) << endl;
	cout << "AVG Recall:    \t" << mean(recall) << " \t STD: " << sqrt(variance(recall,recall)) << endl;
	cout << "AVG F_measure: \t" << mean(F_measure) << " \t STD: " << sqrt(variance(F_measure,F_measure)) << endl << endl << endl;
	return 1;
}


