#include "AudioRecognitionImpl.h"
#include "feature_extract.h"

#include <iostream>


using namespace tflite;


AudioRecognitionImpl::AudioRecognitionImpl()
{

}


AudioRecognitionImpl::~AudioRecognitionImpl()
{
	for(auto const& elem: last_frames) {
		delete(elem);
	}
	last_frames.clear();
}




void AudioRecognitionImpl::BuildInterpreter()
{

	//FIXME: Clear old lastframes

	InterpreterBuilder builder(*model.get(), resolver);
	builder(&interpreter);

	if(!interpreter){
		std::cout << "Error creating Interpreter" << std::endl;
	}

	SetThreadCount(1);

	const std::vector<int> inputs = interpreter->inputs();

	switch (interpreter->tensor(inputs[0])->type) {

		case kTfLiteUInt8:
			quantized = true;
			break;
		default:
			quantized = false;
			break; 
	}

	// Allocate tensor buffers.
	if(interpreter->AllocateTensors() != kTfLiteOk){
		std::cerr << "Failed to Allocate Tensors" << std::endl;
	}

	//int t_size = interpreter->tensors_size();

	const std::vector<int> outputs = interpreter->outputs();
	TfLiteIntArray* output_dims = interpreter->tensor(outputs[0])->dims;
	output_size = output_dims->data[output_dims->size - 1];

	for(int i = 0 ; i < output_size ; i++){
		last_frames.push_back( new std::list<float>);
	}

}


void AudioRecognitionImpl::SetThreadCount(size_t val)
{
	number_of_threads = val;
	if (number_of_threads != -1) {
		interpreter->SetNumThreads(number_of_threads);
	}
}


int AudioRecognitionImpl::RunDetection(const uint8_t* const data,const int mel_length)
{
	_interpret(data,mel_length);
	return smooth();
}

uint8_t* AudioRecognitionImpl::RunRawDetection(const uint8_t* const data,const int mel_length)
{
	_interpret(data,mel_length);
	
	if(quantized){
		return interpreter->typed_output_tensor<uint8_t>(0);
	}
	
	return 0;
}


void AudioRecognitionImpl::_interpret(const uint8_t* const data,const int mel_length)
{

	if(mel_length > melcount*melframes){
		std::cerr << "Invalid size for mel_length: "<< mel_length << std::endl;
		return;
	} 

	size_t fs = sizeof(uint8_t);
	uint8_t tmp[melcount*melframes];

	
	//Copy old content to tmp, the oldest data of size mel_length is discarded
	memcpy(tmp, melwindow+mel_length, (melcount*melframes-mel_length)*fs);
	
	//Copy new mel content to tmp
	memcpy(tmp + (melcount*melframes-mel_length),data,mel_length*fs);
	
	//Copy tmp to melwindow
	memcpy(melwindow,tmp,melcount*melframes*fs);
	
	//if(false){	
	if(quantized){
		auto input = interpreter->typed_input_tensor<uint8_t>(0);

		for(size_t i=0 ; i < melcount*melframes; i++)
		{
			input[i] = melwindow[i];
		}
	} else {

		auto input = interpreter->typed_input_tensor<float>(0);

		for(size_t i=0 ; i < melcount*melframes; i++)
		{
			input[i] = (float) melwindow[i];
		}
	}

	// Run inference 
	if (interpreter->Invoke() != kTfLiteOk) {
		std::cerr << "Failed to invoke tflite!" << std::endl;
	}

}


uint8_t float_to_uint(float input){

	uint8_t value = (uint8_t) (input * 255);
	return value;
}

int AudioRecognitionImpl::smooth()
{	

	if(quantized){
		auto output = interpreter->typed_output_tensor<uint8_t>(0);
		return smooth_detection(output,output_size);
	} else {
		auto output = interpreter->typed_output_tensor<float>(0);

		std::vector<uint8_t> tmp;

		for (int i = 0 ; i < output_size ; i++ ){
			//std::cout <<"Before: "<< output[i]  << std::endl;
			tmp.push_back(float_to_uint(output[i]));
		}
		
		uint8_t* tmp_arr = &tmp[0];

		//Convert to float
		return smooth_detection(tmp_arr,output_size);
	}
}



// Accumulate scores. Scores are decayed over time. A event is usally 1 second maximum 
// so after one second of no activity score should be close to zero. 
// Maximum one prediction every n frames.
int AudioRecognitionImpl::smooth_detection(uint8_t*scores,int size)
{
	if (cooldown > 0){
		cooldown --;
	}

	//Moving window, max 5 elements
	for (auto& frame : last_frames)
	{
		if (frame->size() >= 3){
			frame->pop_back();
		}
	}



	//exclude index 0 which is _unknown_
	for(int i = 1 ; i < size; ++i){

		//std::cout << (unsigned) scores[i]  << std::endl;

		if(scores[i] >= 250 + 5*sensitivity ){
			last_frames[i]->push_front(scores[i]);
			
		} else {
			last_frames[i]->push_front(0);
		}
	}


	float biggest_score = 0;
	int biggest_score_key = 0;
	
	int key = 0;
	for (auto& frame : last_frames)
	{
		//Check if the biggest score in last frames is over the threshold
		float score = std::accumulate(std::begin(*frame), std::end(*frame), 0.0);
		if(score > biggest_score){
			biggest_score = score;
			biggest_score_key = key;
		}
		key ++;
	}
	

	if(biggest_score > (200+555*sensitivity) && cooldown == 0){
		cooldown = detection_cooldown;
		last_frames[biggest_score_key]->clear();
		return biggest_score_key;
	}

	return 0;
}


size_t AudioRecognitionImpl::get_input_data_size()
{
	int blocksize = 20;
	int frame_shift = 160; 
	int bitsize = 1;
	return blocksize*frame_shift*bitsize;
}

void AudioRecognitionImpl::SetSensitivity(float sens)
{
	if( sens <= 1.0 && sens > 0){
		this->sensitivity = 1.0 - sens;
	}
}



