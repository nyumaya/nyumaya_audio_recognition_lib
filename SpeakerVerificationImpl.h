#ifndef NYUMAYA_SPEAKER_VERIFICATION_IMPL_H
#define NYUMAYA_SPEAKER_VERIFICATION_IMPL_H

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

class SpeakerVerificationImpl {

	public:

		SpeakerVerificationImpl(const std::string& modelPath);

		~SpeakerVerificationImpl();

		uint8_t* VerifySpeaker(const uint8_t* const data,const int mel_length);

		void SetThreadCount(size_t val);
		
		size_t get_input_data_size();

	private:
	
		uint8_t* interpret();
		void PrintDebug();

		std::unique_ptr<tflite::Interpreter> interpreter;
		std::unique_ptr<tflite::FlatBufferModel> model;
		tflite::ops::builtin::BuiltinOpResolver resolver;

		int number_of_threads = 1;
		bool quantized = false; 

		static const size_t melcount = 40;
		static const size_t melframes = 198;
		uint8_t fingerprint[512];

		uint8_t melwindow[melcount*melframes];

		int output_size;
};

#endif
