#include "SpeakerVerificationImpl.h"
#include <iostream>
#include "feature_extract.h"

using namespace tflite;



#define TFLITE_MINIMAL_CHECK(x)                              \
  if (!(x)) {                                                \
    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
    exit(1);                                                 \
}



SpeakerVerificationImpl::SpeakerVerificationImpl(const std::string& modelPath){


	// Load model
	model = tflite::FlatBufferModel::BuildFromFile(modelPath.c_str());
	TFLITE_MINIMAL_CHECK(model != nullptr);


	InterpreterBuilder builder(*model.get(), resolver);
	builder(&interpreter);

	if(!interpreter){
		std::cout << "Error creating Interpreter" << std::endl;
	}
 

	const std::vector<int> inputs = interpreter->inputs();

	switch (interpreter->tensor(inputs[0])->type) {
		case kTfLiteFloat32:
			quantized = false;
			break;

		case kTfLiteUInt8:
			quantized = true;
			break;
	}

 	std::vector<int> dims;
	dims.push_back(1);
	dims.push_back(melframes*melcount);

	// Allocate tensor buffers.
	TFLITE_MINIMAL_CHECK(interpreter->AllocateTensors() == kTfLiteOk);

	int t_size = interpreter->tensors_size();

	const std::vector<int> outputs = interpreter->outputs();
	TfLiteIntArray* output_dims = interpreter->tensor(outputs[0])->dims;
	output_size = output_dims->data[output_dims->size - 1];

	PrintDebug();

}


SpeakerVerificationImpl::~SpeakerVerificationImpl()
{

}


void SpeakerVerificationImpl::PrintDebug()
{

	std::cout << "tensors size: " << interpreter->tensors_size() << "\n";
	std::cout << "nodes size: " << interpreter->nodes_size() << "\n";
	std::cout << "inputs: " << interpreter->inputs().size() << "\n";
	std::cout << "input(0) name: " << interpreter->GetInputName(0) << "\n";

	int t_size = interpreter->tensors_size();
	for (int i = 0; i < t_size; i++) {
		if (interpreter->tensor(i)->name)
			std::cout << i << ": " << interpreter->tensor(i)->name << ", "
			<< interpreter->tensor(i)->bytes << ", "
			<< interpreter->tensor(i)->type << ", "
			<< interpreter->tensor(i)->params.scale << ", "
			<< interpreter->tensor(i)->params.zero_point << "\n";
	}

	const std::vector<int> inputs = interpreter->inputs();
	const std::vector<int> outputs = interpreter->outputs();


	std::cout << "number of inputs: " << inputs.size() << "\n";
	std::cout << "number of outputs: " << outputs.size() << "\n";


}


uint8_t SpeakerVerificationImpl::convert_to_int(float value)
{
	uint8_t ret = (value * 6.4) + 128;
	return ret;
}


void SpeakerVerificationImpl::SetThreadCount(size_t val)
{
	number_of_threads = val;
	if (number_of_threads != -1) {
		interpreter->SetNumThreads(number_of_threads);
	}
}



float* SpeakerVerificationImpl::VerifySpeaker(const uint8_t* const data,const int mel_length)
{

	size_t fs = sizeof(uint8_t);
	memcpy(melwindow,data,mel_length*fs);
	
	return interpret();
}



float* SpeakerVerificationImpl::interpret()
{

	if(quantized){
		auto input = interpreter->typed_input_tensor<uint8_t>(0);

		for(size_t i=0 ; i < melcount*melframes; i++)
		{
			input[i] = convert_to_int(melwindow[i]);
		}

	} else {
		auto input = interpreter->typed_input_tensor<float>(0);

		for(size_t i=0 ; i < melcount*melframes; i++)
		{
			input[i] = melwindow[i];
		}
	}

	// Run inference 
	if (interpreter->Invoke() != kTfLiteOk) {
		std::cerr << "Failed to invoke tflite!" << std::endl;
	}
	

	//FIXME: output_size seems to get overwritten
	int outputs = 128;
	if(quantized){
		std::cout << "Output Size: " << output_size << std::endl;
		auto output = interpreter->typed_output_tensor<uint8_t>(0);

		for (int i = 0 ; i < outputs ; i++){
			fingerprint[i] = output[i] - 128;
		}
		
		return fingerprint;
	} else {
		std::cout << "WARNING NOT QUANTIZED" << std::endl;
		auto output = interpreter->typed_output_tensor<float>(0);
		return output;
	}

	return 0;
}


size_t SpeakerVerificationImpl::get_input_data_size()
{
	int sample_rate = 16000;
	int len = 2; 
	int bitsize = 2;
	return sample_rate*len*bitsize;
}




