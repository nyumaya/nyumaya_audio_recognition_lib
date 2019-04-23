#include <iostream>
#include <cmath> 
#include <iomanip>


#define PI 3.14159265

#include "lib.h"

int main(){

	const uint8_t reference[] = {4,7,67,86,70,7,43,45,5,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,7,67,
	86,70,8,43,45,5,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,7,67,86,70,7,43,45,5,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,7,67,86,70,6,43,45,5,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,
	7,67,86,70,7,43,45,5,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,7,67,86,70,8,43,45,5,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,7,67,86,70,8,43,45,5,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,6,9,67,86,70,6,43,45,6,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,37,47,70,82,72,53,53,45,32,31,29,27,
	25,24,22,21,19,18,17,16,15,14,13,12,12,11,10,10,9,9,8,8,7,7,7,7,7,6,6,6,31,43,55,60,58,50,44,37,30,28,26,24,22,21,19,18,17,
	16,14,13,12,12,11,10,10,9,8,8,8,7,7,7,6,6,6,6,6,5,5,5};

	const int test_signal_len = 1600;

	int16_t test_signal[test_signal_len];

	const int expected_mel_len = 40*10;

	uint8_t result[expected_mel_len];

	const float gain = 1.0;

	for (int i = 0 ; i < test_signal_len; i++){
		test_signal[i] =  (1000 * sin (5*i*PI/180)) + (100* cos (10*i*PI/180));
	}

	FeatureExtractor* f = create_feature_extractor();

	print_mel_filter(f);

	int res;
	res = signal_to_mel(f,test_signal ,test_signal_len,result,gain);

	bool test_failed = false;

	std::cout << std::setprecision (16) << "Log10: " << log10(1e-5) << std::endl;
	std::cout << std::setprecision (16) << "Log: "   << log(1e-5)   << std::endl;
	std::cout << "Sizeof Float: " << sizeof(float) << std::endl;
	std::cout << "Sizeof Double: " << sizeof(double) << std::endl;
	std::cout << "TestSignal100: " << test_signal[100] << std::endl; 


	//Check if result is golden standard
	for(int i = 0 ; i < res; i++){
		if(result[i] != reference[i]){
			std::cout << "Feature extraction test failed at pos " << i << std::endl;
			std::cout << "Value should be " << (unsigned) reference[i] << " but is " <<  (unsigned) result[i] << std::endl;
			test_failed = true;	
		}
	}

	if(test_failed == true){
		exit(EXIT_FAILURE); 
	}


	std::cout << "Test passed" << std::endl;

	return 0;
}




