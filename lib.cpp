#include "AudioRecognitionImpl.h"
#include "SpeakerVerificationImpl.h"
#include "feature_extract.h"

#include "lib.h"



SpeakerVerificationImpl* create_speaker_verification(const char*modelPath)
{
	std::string str(modelPath);
	return new SpeakerVerificationImpl(str);
}

float* VerifySpeaker(SpeakerVerificationImpl*impl,const int16_t* const data,const int array_length)
{
	return impl->VerifySpeaker(data, array_length);
}


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




FeatureExtractor* create_feature_extractor(size_t nfft,size_t melcount,size_t sample_rate,
                    size_t lowerf, size_t upperf,float window_len,float shift)
{
	return new FeatureExtractor(nfft,melcount,sample_rate,lowerf,upperf,window_len,shift);
}

int signal_to_mel(FeatureExtractor*impl,const int16_t * const pcm, size_t len,float*result,float gain){
	return impl->signal_to_mel(pcm,len,result,gain);
}


size_t get_melcount(FeatureExtractor*impl){
	return impl->get_melcount();

}

void remove_dc_offset(FeatureExtractor*impl,bool value)
{
	impl->remove_dc_offset(value);
}






