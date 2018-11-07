#ifndef NYUMAYA_AUDIO_RECOGNITION_H
#define NYUMAYA_AUDIO_RECOGNITION_H

#include <string>

class AudioRecognitionImpl;
class AudioRecognition {

	public:

		AudioRecognition(const std::string& modelPath);

		~AudioRecognition();

		int RunDetection(int16_t*data,const int array_length);

		//Set the detection sensitivity between 0-1
		//0 low sensitivity
		//1 high sensitivity
		void SetSensitivity(float sens);

		//Set the volume gain to be applied to the input signal
		void SetGain(float val);
		
		//Remove the DC offset from a microphone signal
		void RemoveDC(bool val);
		
		size_t GetInputDataSize();
		
		std::string GetVersionString();
		
	private:
		AudioRecognitionImpl* mImpl;
};




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
}





#endif
