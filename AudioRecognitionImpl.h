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
#include "tensorflow/lite/optional_debug_tools.h"

class FeatureExtractor;

class AudioRecognitionImpl {

	public:

		AudioRecognitionImpl(const std::string& modelPath);

		~AudioRecognitionImpl();

		int RunDetection(const int16_t* const data,const int array_length);
		
		int RunMelDetection(const float* const result,const int mel_len);
		
		int GetFeatures(const int16_t* const data, size_t len,float*result);

		int RunDetection(const int32_t* const data,const int array_length);

		void SetSensitivity(float sens);

		void SetGain(float val);

		void SetThreadCount(size_t val);

		void ProfileRun();

		size_t get_input_data_size();
		
		void PrintDebug();
		
		const std::string GetVersionString();

	private:
	
		int interpret();
		
		void test();

		std::unique_ptr<tflite::Interpreter> interpreter;
		std::unique_ptr<tflite::FlatBufferModel> model;
		tflite::ops::builtin::BuiltinOpResolver resolver;

		int smooth_detection(float*scores,int size);
		int smooth_detection(uint8_t*scores,int size);

		int number_of_threads = 1;
		bool quantized = false; 
		FeatureExtractor *f;
		static const size_t melcount = 40;
		static const size_t melframes = 98;
		float sensitivity = 0.5;
		float gain = 1.0;
		float melwindow[melcount*melframes];

		std::vector< std::list<float>* > last_frames;

		int cooldown = 0;
		int detection_cooldown = 7;
		int output_size = 0;
};

#endif
