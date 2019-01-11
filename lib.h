#ifndef NYUMAYA_AUDIO_RECOGNITION_H
#define NYUMAYA_AUDIO_RECOGNITION_H

#include <string>

extern "C"
{


	const char* GetVersionString(){return "0.3.4";}

	//Audio Recognition
	AudioRecognitionImpl* create_audio_recognition(const char* modelPath);
	
	
	//Input Mel Features and get the index of the detected label if recognized
	//@param array_length    Number of mel features
	//@param data            Signed uint8_t mel features
	int RunDetection(AudioRecognitionImpl*impl,const uint8_t* const data,const int mel_length); 
	
	
	//Input Mel Features and get the raw probabilities of the labels
	uint8_t*RunRawDetection(AudioRecognitionImpl*impl,const uint8_t* const data,const int mel_length);


	void SetSensitivity(AudioRecognitionImpl*impl,float sens);
 	
	size_t GetInputDataSize(AudioRecognitionImpl*impl);
	
	//Speaker Verification
	SpeakerVerificationImpl* create_speaker_verification(const char*modelPath);

	uint8_t* VerifySpeaker(SpeakerVerificationImpl*impl,const uint8_t* const data,const int array_length);
	
	
	//Feature Extractor
	FeatureExtractor* create_feature_extractor(size_t nfft=512,size_t melcount = 40,size_t sample_rate=16000,
	    size_t lowerf=20, size_t upperf=8000,float window_len=0.03,float shift=0.01);

	int signal_to_mel(FeatureExtractor*impl,const int16_t * const pcm, size_t len,uint8_t*result,float gain);
	
	size_t get_melcount(FeatureExtractor*impl);

}





#endif
