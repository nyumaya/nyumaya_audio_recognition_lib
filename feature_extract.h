#ifndef FEATURE_EXTRACT_H
#define FEATURE_EXTRACT_H

#include "./kissfft/tools/kiss_fftr.h"
#include <stdint.h>
#include <vector>


class FeatureExtractor {

	public:
		FeatureExtractor(size_t nfft=512,size_t melcount = 40,size_t sample_rate=16000,size_t lowerf=20, size_t upperf=8000,float window_len=0.03,float shift=0.01);
		~FeatureExtractor();

		int signal_to_mel(const int16_t * const pcm, size_t len,uint8_t*result,float gain);
		
		void spectrum(const float*const pcm,float*real,float*imag);
		
		size_t get_melcount();
		
		uint8_t quantize_float(float value);
		
	private:

		void create_mel_filter();
		void create_hanning_window();
		
		kiss_fftr_cfg cfg;
		size_t melcount;
		size_t sample_rate;
		size_t nfft;
		float window_len;
		float shift;
		size_t lowerf;
		size_t upperf;
		std::vector<float> hann;
		float mean = 0;
		bool remove_dc = false;
		float mel_filters[512/2+1][40];
};

#endif
