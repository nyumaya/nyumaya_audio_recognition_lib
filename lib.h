#ifndef NYUMAYA_AUDIO_RECOGNITION_H
#define NYUMAYA_AUDIO_RECOGNITION_H

#include <string>

extern "C"
{


	//Audio Recognition
	AudioRecognitionImpl* create_audio_recognition(const char* modelPath);
	
	//@param array_length    Number of int16_t samples
	//@param data            Signed int16 pcm data
	int RunDetection(AudioRecognitionImpl*impl,const int16_t* const data,const int array_length); 
	
	int RunMelDetection(AudioRecognitionImpl*impl,const float* const result,const int mel_len);
	
	int GetFeatures(AudioRecognitionImpl*impl,const int16_t* const data, size_t len,float*result);
	
	void RemoveDC(AudioRecognitionImpl*impl,bool val);
	
	void SetSensitivity(AudioRecognitionImpl*impl,float sens);
	
 	void SetGain(AudioRecognitionImpl*impl,float val);
 	
	size_t GetInputDataSize(AudioRecognitionImpl*impl);
	
	const char* GetVersionString(AudioRecognitionImpl*impl);

	
	//Speaker Verification
	SpeakerVerificationImpl* create_speaker_verification(const char*modelPath);

	float* VerifySpeaker(SpeakerVerificationImpl*impl,const int16_t* const data,const int array_length);
	
	//Feature Extractor
	FeatureExtractor* create_feature_extractor(size_t nfft=512,size_t melcount = 40,size_t sample_rate=16000,size_t lowerf=20, size_t upperf=8000,float window_len=0.03,float shift=0.01);

	int signal_to_mel(FeatureExtractor*impl,const int16_t * const pcm, size_t len,float*result,float gain);
	
	size_t get_melcount(FeatureExtractor*impl);
		
	void remove_dc_offset(FeatureExtractor*impl,bool value);
	

}





#endif
