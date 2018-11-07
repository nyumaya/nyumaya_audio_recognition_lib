#include "AudioRecognitionImpl.h"
#include "lib.h"


AudioRecognitionImpl* create_audio_recognition(const char* modelPath)
{
	std::string str(modelPath);
	return new AudioRecognitionImpl(str);
}



int RunMelDetection(AudioRecognitionImpl*impl,const float* const result,const int mel_len)
{
	return impl->RunMelDetection(result,mel_len);
}


int RunDetection(AudioRecognitionImpl*impl,const int16_t* const data,const int array_length)
{
	return impl->RunDetection(data,array_length);
}


void RemoveDC(AudioRecognitionImpl*impl,bool val)
{
	impl->RemoveDC(val);
}

void SetSensitivity(AudioRecognitionImpl*impl,float sens)
{
	 impl->SetSensitivity(sens);
}



int GetFeatures(AudioRecognitionImpl*impl,const int16_t* const data, size_t len,float*result)
{
	return impl->GetFeatures(data, len,result);
}

void SetGain(AudioRecognitionImpl*impl,float val)
{
	impl->SetGain(val);
}
	
size_t GetInputDataSize(AudioRecognitionImpl*impl)
{
	return impl->get_input_data_size();
}
	

const char* GetVersionString(AudioRecognitionImpl*impl)
{
	return impl->GetVersionString().c_str();
}


AudioRecognition::AudioRecognition(const std::string& modelPath)
{
	mImpl = new AudioRecognitionImpl(modelPath);
}


AudioRecognition::~AudioRecognition()
{
	free(mImpl);
}


void AudioRecognition::RemoveDC(bool val)
{
	mImpl->RemoveDC(val);
}


int AudioRecognition::RunDetection(int16_t*data,const int array_length)
{
	return mImpl->RunDetection(data,array_length);
}



size_t AudioRecognition::GetInputDataSize()
{
	return mImpl->get_input_data_size();
}

void AudioRecognition::SetSensitivity(float sens)
{
	 mImpl->SetSensitivity(sens);
}


void AudioRecognition::SetGain(float val)
{
	mImpl->SetGain(val);
}


std::string AudioRecognition::GetVersionString()
{
	return mImpl->GetVersionString();
}












