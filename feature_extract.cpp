
#include "feature_extract.h"

#include <math.h> 
#include <iostream>
#include <vector>
#include <cstdlib>

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


float mel(float f){
	return 2595. * log10(1. + f / 700.);
}

float melinv(float m){
	return 700. * (pow(10., m / 2595.) - 1.);
}

FeatureExtractor::FeatureExtractor()
{
	this->cfg = kiss_fftr_alloc( this->nfft ,false ,0,0 );
	this->create_hanning_window();
	this->create_mel_filter();
}

FeatureExtractor::~FeatureExtractor()
{
	kiss_fftr_free(this->cfg);
}

size_t FeatureExtractor::get_melcount()
{
	return melcount;
}



void FeatureExtractor::create_mel_filter()
{
	float melmax = mel(this->upperf);
	float melmin = mel(this->lowerf);
	float dmelbw = (melmax - melmin) / (this->melcount + 1);
	float dfreq = this->sample_rate / this->nfft;


	if (this->upperf > this->sample_rate/2){
		std::cerr << "Upper frequency " << upperf << " exceeds Nyquist " << this->sample_rate/2 << std::endl; 
	}

	std::vector<float> filter_edge;


	for(int i=0; i < this->melcount+2; ++i){
		float edge = melinv(melmin + dmelbw * i);
		filter_edge.push_back(edge);
	}
	
	
	for(int i = 0; i < (512/2+1) ; i++){
		for(int j = 0; j < melcount ; j++){
			mel_filters[i][j] = 0;
		}
	}

	for(int i=0; i < this->melcount; ++i){

		int leftfr = round(filter_edge[i] / dfreq);
		int centerfr = round(filter_edge[i + 1] / dfreq);
		int rightfr = round(filter_edge[i + 2] / dfreq);
		float fwidth = (rightfr - leftfr) * dfreq;
		float height = 2. / fwidth;

		float leftslope = 0;
		float rightslope =0;

		if (centerfr != leftfr)
			leftslope = height / (centerfr - leftfr);
		else
			leftslope = 0;

		int freq = leftfr + 1;
		while (freq < centerfr){
			this->mel_filters[freq][i] = (freq - leftfr) * leftslope;
			freq++; 
		}
		if (freq == centerfr){ // This is always true
			this->mel_filters[freq][i] = height;
			freq++;
		}

		if (centerfr != rightfr){
			rightslope = height / (centerfr - rightfr);
		}
	
	
		while(freq < rightfr){
			this->mel_filters[freq][i] = (freq - rightfr) * rightslope;
			freq++;
		}
	}


}


void FeatureExtractor::create_hanning_window()
{
	for(int n = 0 ; n < this->nfft ; ++n){
		float val = 0.5-0.5*cos( (2*M_PI*n) / (this->nfft-1) );
		this->hann[n] = val;
	}
}


void FeatureExtractor::spectrum(const float*const pcm,float*real,float*imag)
{

	size_t fft_out_size = (this->nfft/2)+1;

	kiss_fft_cpx fft_result[fft_out_size];

	kiss_fftr(this->cfg,pcm,fft_result); 
	for (int i = 0; i < fft_out_size ; i++){
		real[i] = fft_result[i].r;
		imag[i] = fft_result[i].i;
	}
}

int FeatureExtractor::signal_to_mel(const int16_t * const pcm ,const size_t len, float*result,float gain)
{

	const float convert = (1.0/32768.0)*gain;

	size_t number_of_frames = int(len / this->shift);

	size_t fft_out_size = (this->nfft/2)+1;

	for(int i=0; i < number_of_frames; ++i){
		int start = i*this->shift;
		kiss_fft_cpx fft_result[fft_out_size];
		float power_spectrum[this->nfft];


		//Apply Hanning Window
                float frame[this->datalen];
                memset( frame, 0, this->datalen*sizeof(float) );

		for (int pos = 0 ; pos < this->datalen ; ++pos){
			if(pos+start < len){
				frame[pos] = pcm[pos+start] * convert * this->hann[pos];
				
			} else {
				frame[pos] = 0*this->hann[pos];
			}
		}

		kiss_fftr(this->cfg,frame,fft_result); 

		//Power Spectrum
		for(int j=0; j < fft_out_size; ++j){
			float imag = fft_result[j].i;
			float real = fft_result[j].r;
			power_spectrum[j] = abs(-(imag*-imag) + real*real);
		}
		

		//Apply Mel Scale
		for(int j=0; j < this->melcount; ++j){
			float sum = 0;
			for (int k = 0 ; k < fft_out_size; k++){
				sum += power_spectrum[k] * mel_filters[k][j];
			}

			result[j+this->melcount*i] = log(sum+1e-6);
 		}
	}


	return this->melcount*number_of_frames;
}




