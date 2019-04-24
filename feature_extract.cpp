
#include "feature_extract.h"

#include <cmath> 
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>

/* 

Mel Filter creation is based on Sphinx-III 


Copyright (c) 2006 Carnegie Mellon University
#
# You may copy and modify this freely under the same terms as
# Sphinx-III

"""Compute MFCC coefficients.
This module provides functions for computing MFCC (mel-frequency
cepstral coefficients) as used in the Sphinx speech recognition
system.
"""

__author__ = "David Huggins-Daines <dhuggins@cs.cmu.edu>"
*/


float mel(const float f){
	return 2595. * log10(1. + f / 700.);
}

float melinv(const float m){
	return 700. * (pow(10., m / 2595.) - 1.);
}

FeatureExtractor::FeatureExtractor(size_t nfft,size_t melcount,size_t sample_rate,size_t lowerf , size_t upperf,float window_len,float shift ) :
	nfft(nfft),melcount(melcount),sample_rate(sample_rate),lowerf(lowerf),upperf(upperf),shift(shift*sample_rate),window_len(window_len)
{
	this->cfg  = pffft_new_setup(this->nfft, pffft_transform_t::PFFFT_REAL);

	size_t fft_out_size = (nfft/2)+1;
	fft_result = (float*) pffft_aligned_malloc(fft_out_size*2 * sizeof(float));
	frame = (float*) pffft_aligned_malloc(nfft * sizeof(float));

	this->create_hanning_window();
	this->create_mel_filter();
}


FeatureExtractor::~FeatureExtractor()
{
	pffft_aligned_free(fft_result);
	pffft_aligned_free(frame);
}

size_t FeatureExtractor::get_melcount()
{
	return melcount;
}

void FeatureExtractor::print_mel_filter()
{
	for(size_t i=0; i < (this->nfft/2+1) ; i++){
		for(size_t j = 0; j < melcount ; j++){
			if(mel_filters[i][j] != 0){
				std::cout << i << ":" << j << " " << mel_filters[i][j] << "\t";
			}
		}
		std::cout << std::endl;
	}
}

void FeatureExtractor::create_mel_filter()
{
	const float melmax = mel(this->upperf);
	const float melmin = mel(this->lowerf);
	const float dmelbw = (melmax - melmin) / (this->melcount + 1);
	const float dfreq = this->sample_rate / this->nfft;


	if (this->upperf > this->sample_rate/2){
		std::cerr << "Upper frequency " << upperf << " exceeds Nyquist " << this->sample_rate/2 << std::endl; 
	}

	std::vector<float> filter_edge;


	for(size_t i=0; i < this->melcount+2; ++i){
		float edge = melinv(melmin + dmelbw * i);
		filter_edge.push_back(edge);
	}
	
	
	for(size_t i=0; i < (this->nfft/2+1) ; i++){
		for(size_t j = 0; j < melcount ; j++){
			mel_filters[i][j] = 0;
		}
	}

	for(size_t i=0; i < this->melcount; ++i){

		int leftfr = round(filter_edge[i] / dfreq);
		int centerfr = round(filter_edge[i + 1] / dfreq);
		int rightfr = round(filter_edge[i + 2] / dfreq);
		float fwidth = (rightfr - leftfr) * dfreq;
		float height = 2. / fwidth;

		float leftslope  = 0;
		float rightslope = 0;

		if (centerfr != leftfr)
			leftslope = height / (centerfr - leftfr);
		else
			leftslope = 0;

		int freq = leftfr + 1;
		
		while (freq < centerfr){
			mel_filters[freq][i] = (freq - leftfr) * leftslope;
			freq++; 
		}
		
		if (freq == centerfr){ // This is always true
			mel_filters[freq][i] = height;
			freq++;
		}

		if (centerfr != rightfr){
			rightslope = height / (centerfr - rightfr);
		}
	
	
		while(freq < rightfr){
			mel_filters[freq][i] = (freq - rightfr) * rightslope;
			freq++;
		}
	}


}


void FeatureExtractor::create_hanning_window()
{
	for(size_t n = 0 ; n < this->nfft ; ++n){
		float val = 0.5-0.5*cos( (2*M_PI*n) / (this->nfft-1) );
		hann.push_back(val);
	}
}



void FeatureExtractor::spectrum(const float*const pcm,float*real,float*imag)
{
	/*const size_t fft_out_size = (this->nfft/2)+1;

	kiss_fft_cpx fft_result[fft_out_size];

	kiss_fftr(this->cfg,pcm,fft_result); 
	for (size_t i = 0; i < fft_out_size ; i++){
		real[i] = fft_result[i].r;
		imag[i] = fft_result[i].i;
	}*/
}

uint8_t FeatureExtractor::quantize_float(const float value)
{
	//The values 12 and 8 are determined empirically
	float converted_value = (value + 12) * 8;
	if(converted_value > 255){
		return 255;
	}
	
	if(converted_value < 0){
		return 0;
	}
	
	return (uint8_t) converted_value;
}



int FeatureExtractor::signal_to_mel(const int16_t * const pcm ,const size_t len, uint8_t*result,const float gain)
{

	const float convert = (1.0/32768.0)*gain;

	double max = 0;

	for(size_t i=0; i < len; ++i){
		max += pcm[i];
	}
		
	const float mean = max/len;

	const size_t number_of_frames = int(len / this->shift);
	const size_t fft_out_size = (this->nfft/2)+1;

	float power_spectrum[fft_out_size];

	
	for(size_t i=0; i < number_of_frames; ++i){
		const int start = i*this->shift;
		
		//Apply Hanning Window

		memset(frame, 0, this->nfft*sizeof(float) );

		for (size_t pos = 0 ; pos < this->nfft ; ++pos){
			if(pos+start < len){
				frame[pos] = (pcm[pos+start] - mean) * convert * this->hann[pos];
			}
		}

		pffft_transform_ordered(this->cfg,frame,fft_result,NULL, pffft_direction_t::PFFFT_FORWARD);

		//Power Spectrum
		for(size_t j=0; j < fft_out_size*2;j +=2 ){

			float imag = fft_result[j];
			float real = fft_result[j+1];

			power_spectrum[j/2] = std::abs(-(imag*-imag) + real*real);
		}
		
		//Apply Mel Scale
		for(size_t j=0; j < this->melcount; ++j){
			float sum = 0;
			for (size_t k = 0 ; k < fft_out_size; k++){
				sum += power_spectrum[k] * mel_filters[k][j];
			}

			result[j+this->melcount*i] = quantize_float(log(sum+1e-5));
 		}
	}


	return this->melcount*number_of_frames;
}










