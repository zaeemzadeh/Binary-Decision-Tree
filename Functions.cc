#include "Functions.h"
#include <iostream>
// --------------- Functions ----------------------------------
float mean(vector <float> data){
	//cout << "Data size = " << data.size() << endl;
	float sum = 0;
	for (int i = 0; i < data.size(); i++){
		sum += data[i];
	}
	return sum/data.size();
}

float variance(vector <float> datax,vector <float> datay){
	float meanx = mean(datax);
	float meany = mean(datay);

	float variance = 0;
	
	int size = (datax.size() < datay.size())? datax.size():datay.size();
	for (int i = 0; i < size;i++){
		variance += (datax[i] - meanx)*(datay[i] - meany);
	}
	variance = variance/size;
	return variance;

}

float max(vector <float> data){
	float max = -HUGE_VALUE;
	for (int i = 0; i < data.size(); i++){
		max = (max > data[i])? max:data[i];
	}
	return max;
}

float min(vector <float> data){
	float min = HUGE_VALUE;
	for (int i = 0; i < data.size(); i++){
		min = (min < data[i])? min:data[i];
	}
	return min;
}

void read_data(vector<Record> &data, ifstream &input, const int H0, const int H1){
	char c_Sex;
	char camma;
	while (input >> c_Sex){
		float attributes [7];
		int rings;
		for (int i = 0; i < 7; i++){
			input >> camma;
			input >> attributes[i];
		}
		input >> camma;
		input >> rings;
		if (rings == H0){
			Record new_record(c_Sex, attributes, -1);
			data.push_back(new_record);
		}
		if (rings == H1){
			Record new_record(c_Sex, attributes, 1);
			data.push_back(new_record);
		}
	}
	return;
}

void split_data(const vector<Record> &data, vector <int> &training_set, 
		vector <int> &test_set, int BatchNum, int K){
	// here we choose a batch of size N/K as the test set
	int TestSetSize = (int)data.size()/K;
	for (int i = 0; i < (int)data.size(); i++) {
		if (i >= (BatchNum - 1) * TestSetSize && i < BatchNum * TestSetSize)
			test_set.push_back(i);
		else
			training_set.push_back(i);
	}	
	return;
}

vector<int> Confusion_Matrix(const vector<Record> &data, const vector <int> &inds){
	int size = inds.size();

	vector <int> confusion_mtx (4,0); 	

	for (int i = 0; i < size; i++){
		int data_ind = inds[i];
		Record current_record = data[data_ind];
		//cout << "label = " << current_record.Label;
		//cout << "rings = " << current_record.Rings << endl;
		if (current_record.Rings == 1 && current_record.Label == 1)	// True Positive
			confusion_mtx[0]++;
		if (current_record.Rings == 1 && current_record.Label == -1)	// False Negative
			confusion_mtx[1]++;
		if (current_record.Rings == -1 && current_record.Label == 1)	// False Positive
			confusion_mtx[2]++;
		if (current_record.Rings == -1 && current_record.Label == -1)	// True Negative
			confusion_mtx[3]++;
	}
	return confusion_mtx;
} 

