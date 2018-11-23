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

		float* VerifySpeaker(const int16_t* const data,const int array_length);
		
		float* VerifySpeakerMel(const float* const result,const int mel_len);

		float* VerifySpeaker(const int32_t* const data,const int array_length);

		void SetGain(float val);

		void SetThreadCount(size_t val);
		
		size_t get_input_data_size();
		
		const std::string GetVersionString();

	private:
	
		float* interpret();
		void PrintDebug();

		uint8_t convert_to_int(float value);

		std::unique_ptr<tflite::Interpreter> interpreter;
		std::unique_ptr<tflite::FlatBufferModel> model;
		tflite::ops::builtin::BuiltinOpResolver resolver;

		int number_of_threads = 1;
		bool quantized = false; 
		FeatureExtractor *f;
		static const size_t melcount = 40;
		static const size_t melframes = 198;
		float fingerprint[64];

		float melwindow[melcount*melframes];

		bool remove_dc;
		int output_size;
};

#endif
