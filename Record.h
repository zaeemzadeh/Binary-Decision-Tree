#ifndef RECORD_H
#define RECORD_H
enum Attribute{
	Sex,
	Length,
	Diameter,
	Height,
	WholeWeight,
	ShuckedWeight,
	VisceraWeight,
	ShellWeight,		
};


class Record{
public:
	char Sex;			// nominal			M, F, and I (infant)
	float  real_Attributes[7]; 	
	// 1. Length 		continuous	mm	Longest shell measurement
	// 2. Diameter;		 continuous	mm	perpendicular to length
	// 3. Height;		 continuous	mm	with meat in shell
	// 4. WholeWeight;	 continuous	grams	whole abalone
	// 5. ShuckedWeight;	 continuous	grams	weight of meat
	// 6. VisceraWeight	 continuous	grams	gut weight (after bleeding)
	// 7. ShellWeight	 continuous	grams	after being dried
	int Rings;		// integer			+1.5 gives the age in years
	int Label;
public:
	Record(char d_sex, float d_att [], int d_rings);
};

#endif /* RECORD_H */
