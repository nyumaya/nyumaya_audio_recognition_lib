#ifndef FEATURE_EXTRACT_H
#define FEATURE_EXTRACT_H
//#include "./kissfft/tools/kiss_fftr.h"
#include <cstdint>
#include <vector>
#include "pffft.h"

class FeatureExtractor {

	public:
		FeatureExtractor(size_t nfft=512,size_t melcount = 40,size_t sample_rate=16000,size_t lowerf=20, size_t upperf=8000,float window_len=0.03,float shift=0.01);
		~FeatureExtractor();

		int signal_to_mel(const int16_t * const pcm, const size_t len, uint8_t * result, const float gain);
		
		void spectrum(const float*const pcm,float*real,float*imag);
		
		size_t get_melcount();

		void print_mel_filter();
		
	private:
		uint8_t quantize_float(const float value);

		void create_mel_filter();
		void create_hanning_window();
		
		//kiss_fftr_cfg cfg;
		PFFFT_Setup *cfg;

		float *fft_result;
		float *frame;

		const size_t nfft;
		const size_t melcount;
		const size_t sample_rate;
		const size_t lowerf;
		const size_t upperf;
		const float shift;
		const float window_len;

		std::vector<float> hann;
		float mel_filters[512/2+1][40];
};

#endif
