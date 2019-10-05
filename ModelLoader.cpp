#include "AudioRecognitionImpl.h"
#include <iostream>
#include <fstream>

using namespace tflite;


bool file_exists (const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}



int AudioRecognitionImpl::LoadModelFromBuffer(const char*binaryModel,int len)
{	
	//Load model
	model = tflite::FlatBufferModel::VerifyAndBuildFromBuffer(binaryModel,len);

	if(!model){
		std::cerr << "Failed to load model" << std::endl;
		return -1;
	}

	BuildInterpreter();

	return 0;
}





int AudioRecognitionImpl::OpenModel(const std::string& modelPath)
{

	std::string extension = modelPath.substr(modelPath.find_last_of(".") + 1);

	if(!file_exists(modelPath)){
		std::cerr << "File " << modelPath << " does not exist" << std::endl;
		return -1;
	}

	if(extension == "tflite") {
		model = tflite::FlatBufferModel::VerifyAndBuildFromFile(modelPath.c_str());
	} else {
		std::cerr << "Unknown Model File extension" << std::endl;
		return -1;
	}

	if(!model){
		std::cerr << "Failed to load model" << std::endl;
		return -1;
	}

	BuildInterpreter();

	return 0;

}


