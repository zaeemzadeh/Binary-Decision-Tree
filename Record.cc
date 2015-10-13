#include "Record.h"

Record::Record(char d_sex, float d_att [], int d_rings): Sex(d_sex), Rings(d_rings) , Label(0){
	for(int i = 0; i < 7; i++){
		real_Attributes[i] = d_att[i];
	}
}
