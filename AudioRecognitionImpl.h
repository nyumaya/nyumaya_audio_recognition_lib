#ifndef NYUMAYA_AUDIO_RECOGNITION_IMPL_H
#define NYUMAYA_AUDIO_RECOGNITION_IMPL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <cstdio>
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"

class FeatureExtractor;

class AudioRecognitionImpl {

	public:

		AudioRecognitionImpl();

		~AudioRecognitionImpl();

		int RunDetection(const uint8_t* const data,const int mel_length);
		
		uint8_t*RunRawDetection(const uint8_t* const data,const int mel_length);

		void SetSensitivity(float sens);

		void SetThreadCount(size_t val);

		int OpenModel(const std::string& modelPath);

		int LoadModelFromBuffer(const char*binaryModel,int len);

		size_t get_input_data_size();


	private:
	
		int smooth();
		void _interpret(const uint8_t* const data,const int mel_length);
		
		void test();

		void BuildInterpreter();

		std::unique_ptr<tflite::Interpreter> interpreter;
		std::unique_ptr<tflite::FlatBufferModel> model;
		tflite::ops::builtin::BuiltinOpResolver resolver;
		std::vector<char> buffer;

		int smooth_detection(uint8_t*scores,int size);

		int number_of_threads = 1;
		bool quantized = false; 

		static const size_t melcount = 40;
		static const size_t melframes = 98;
		float sensitivity = 0.5;
		uint8_t melwindow[melcount*melframes];

		std::vector< std::list<float>* > last_frames;
		
		int cooldown = 0;
		int detection_cooldown = 7;
		int output_size = 0;
};

#endif
