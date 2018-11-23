#ifndef NYUMAYA_AUDIO_RECOGNITION_H
#define NYUMAYA_AUDIO_RECOGNITION_H

#include <string>

class AudioRecognitionImpl;
class SpeakerVerificationImpl;


extern "C"
{


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
	
	
	
	
	
	SpeakerVerificationImpl* create_speaker_verification(const char*modelPath);

	float* VerifySpeaker(SpeakerVerificationImpl*impl,const int16_t* const data,const int array_length);

}





#endif
