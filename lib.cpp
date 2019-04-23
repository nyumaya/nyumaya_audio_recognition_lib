#include "AudioRecognitionImpl.h"
#include "SpeakerVerificationImpl.h"
#include "feature_extract.h"

#include "lib.h"



SpeakerVerificationImpl* create_speaker_verification(const char*modelPath)
{
	std::string str(modelPath);
	return new SpeakerVerificationImpl(str);
}

uint8_t* VerifySpeaker(SpeakerVerificationImpl*impl,const uint8_t* const data,const int mel_length)
{
	return impl->VerifySpeaker(data, mel_length);
}


AudioRecognitionImpl* create_audio_recognition(const char* modelPath)
{
	std::string str(modelPath);
	return new AudioRecognitionImpl(str);
}


int RunDetection(AudioRecognitionImpl*impl,const uint8_t* const data,const int mel_length)
{
	return impl->RunDetection(data,mel_length);
}

uint8_t*RunRawDetection(AudioRecognitionImpl*impl,const uint8_t* const data,const int mel_length)
{
	return impl->RunRawDetection(data,mel_length);
}

void SetSensitivity(AudioRecognitionImpl*impl,float sens)
{
	 impl->SetSensitivity(sens);
}


size_t GetInputDataSize(AudioRecognitionImpl*impl)
{
	return impl->get_input_data_size();
}
	

FeatureExtractor* create_feature_extractor(size_t nfft,size_t melcount,size_t sample_rate,
                    size_t lowerf, size_t upperf,float window_len,float shift)
{
	return new FeatureExtractor(nfft,melcount,sample_rate,lowerf,upperf,window_len,shift);
}

int signal_to_mel(FeatureExtractor*impl,const int16_t * const pcm, size_t len,uint8_t*result,float gain){
	return impl->signal_to_mel(pcm,len,result,gain);
}


size_t get_melcount(FeatureExtractor*impl){
	return impl->get_melcount();

}


void print_mel_filter(FeatureExtractor*impl){
	return impl->print_mel_filter();

}







