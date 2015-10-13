
#include<vector>
#include <string>
#include <fstream>
#include <stdio.h>
#include "Record.h"
using namespace std;
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

const float HUGE_VALUE = 1000000;
// --------------- Functions ----------------------------------
float mean(vector <float> data); 				// Computes the mean of the data

float variance(vector <float> datax,vector <float> datay);	// Computes the covariance of the datax and datay

float max(vector <float> data);					// Computes the max of the data

float min(vector <float> data);					// Computes the min of the data

// reads data from input file abalone.data
void read_data(vector<Record> &data, ifstream &input, const int H0, const int H1);

// Splits data into training and test set
void split_data(const vector<Record> &data, vector <int> &training_set, 
		vector <int> &test_set, int BatchNum, int K);

// Computes the confusion of the results TP FN FP TN 
vector<int> Confusion_Matrix(const vector<Record> &data, const vector <int> &inds);


#endif /* FUNCTIONS_H */
