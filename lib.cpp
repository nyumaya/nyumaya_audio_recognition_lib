#include "AudioRecognitionImpl.h"
#include "SpeakerVerificationImpl.h"
#include "feature_extract.h"

#include "lib.h"

#include "libjni.h"

const char*VersionString = "0.3.5";


jbyteArray ToJavaByteArray(JNIEnv* env,const uint8_t* bytes,size_t len) 
{
	jbyteArray byte_array = env->NewByteArray(len);
	env->SetByteArrayRegion(
		byte_array, 0, len, reinterpret_cast<const jbyte*>(bytes));
	return byte_array;
}

	
jstring Java_com_nyumaya_audiorecognition_NyumayaLibrary_getVersionString(JNIEnv *env, jobject obj)
{
	return env->NewStringUTF( VersionString);
}

jlong Java_com_nyumaya_audiorecognition_NyumayaLibrary_createFeatureExtractor(JNIEnv *env, jobject obj,jint nfft,jint melcount, jint sample_rate,
	jint lowerf, jint upperf, jfloat window_len,jfloat shift){
	return (jlong) new FeatureExtractor(nfft,melcount,sample_rate,lowerf,upperf,window_len,shift);
}

jbyteArray Java_com_nyumaya_audiorecognition_NyumayaLibrary_signalToMel(JNIEnv *env, jobject obj,jlong impl,jbyteArray pcm,jfloat gain)
{
	FeatureExtractor* f = (FeatureExtractor*) impl;
	int inlen = env->GetArrayLength(pcm)/2;
	jbyte *pcm_arr = env->GetByteArrayElements(pcm, 0);

	//FIXME: Properly get required size
	//This depends on the Feature Extractor parameters and is therefore only known
	//by the feature extractor
	uint8_t result[2048];

	int outlen = f->signal_to_mel((int16_t *)pcm_arr,inlen,result,gain);

	env->ReleaseByteArrayElements(pcm, pcm_arr, 0);

	return ToJavaByteArray(env,result,outlen);
}

jint Java_com_nyumaya_audiorecognition_NyumayaLibrary_getMelcount(JNIEnv *env, jobject obj,jlong impl)
{
	FeatureExtractor* f = (FeatureExtractor*) impl;
	return f->get_melcount();
}


void Java_com_nyumaya_audiorecognition_NyumayaLibrary_deleteFeatureExtractor(JNIEnv *env, jobject obj,jlong impl)
{
	FeatureExtractor* f = (FeatureExtractor*) impl;
	delete f;
}

void Java_com_nyumaya_audiorecognition_NyumayaLibrary_deleteAudioRecognition(JNIEnv *env, jobject obj,jlong impl)
{
	AudioRecognitionImpl* f = (AudioRecognitionImpl*) impl;
	delete f;
}



jlong Java_com_nyumaya_audiorecognition_NyumayaLibrary_createAudioRecognition(JNIEnv *env, jobject obj)
{
	return (jlong) new AudioRecognitionImpl();
}



void Java_com_nyumaya_audiorecognition_NyumayaLibrary_setSensitivity(JNIEnv *env, jobject obj,jlong impl,jfloat sensitivity)
{
	AudioRecognitionImpl* f = (AudioRecognitionImpl*) impl;
	f->SetSensitivity(sensitivity);
}

jint Java_com_nyumaya_audiorecognition_NyumayaLibrary_runDetection(JNIEnv *env, jobject obj,jlong impl,jbyteArray mels)
{
	AudioRecognitionImpl* f = (AudioRecognitionImpl*) impl;

	int inlen = env->GetArrayLength(mels);
	jbyte *mels_arr = env->GetByteArrayElements(mels, 0);

	int result =  f->RunDetection((uint8_t*)mels_arr,inlen);
	env->ReleaseByteArrayElements(mels, mels_arr, 0);
	return result;
}


JNIEXPORT jbyteArray Java_com_nyumaya_audiorecognition_NyumayaLibrary_runRawDetection(JNIEnv *env, jobject obj,jlong impl,jbyteArray mels)
{
	AudioRecognitionImpl* f = (AudioRecognitionImpl*) impl;

	int inlen = env->GetArrayLength(mels);
	jbyte *mels_arr = env->GetByteArrayElements(mels, 0);

	uint8_t*result= f->RunRawDetection((uint8_t*)mels_arr,inlen);
	int result_len = 1;
	//FIXME: Result len is dummy and should be label count.
	//Need a way to get the label count


	return ToJavaByteArray(env,result,result_len);
}


jint Java_com_nyumaya_audiorecognition_NyumayaLibrary_loadModelFromBuffer(JNIEnv *env, jobject obj,jlong impl,jbyteArray modelData){
	AudioRecognitionImpl* f = (AudioRecognitionImpl*) impl;

	int len = env->GetArrayLength(modelData);
	jbyte *binaryModel = env->GetByteArrayElements(modelData, 0);

	return f->LoadModelFromBuffer((const char*)binaryModel,len);
}



AudioRecognitionImpl* createAudioRecognition()
{
	return new AudioRecognitionImpl();
}

void deleteAudioRecognition(AudioRecognitionImpl*impl)
{
	delete impl;
	impl = 0;
}

const char* getVersionString()
{
	return VersionString;
}


int runDetection(AudioRecognitionImpl*impl,const uint8_t* const data,const int mel_length)
{
	return impl->RunDetection(data,mel_length);
}

uint8_t*runRawDetection(AudioRecognitionImpl*impl,const uint8_t* const data,const int mel_length)
{
	return impl->RunRawDetection(data,mel_length);
}

void setSensitivity(AudioRecognitionImpl*impl,float sens)
{
	 impl->SetSensitivity(sens);
}



int openModel(AudioRecognitionImpl*impl,const std::string& modelPath)
{
	impl->OpenModel(modelPath);
}


size_t getInputDataSize(AudioRecognitionImpl*impl)
{
	return impl->get_input_data_size();
}


int loadModelFromBuffer(AudioRecognitionImpl*impl,const char*binaryModel,int len)
{
	return impl->LoadModelFromBuffer(binaryModel,len);
}

FeatureExtractor* createFeatureExtractor(size_t nfft,size_t melcount,size_t sample_rate,
                    size_t lowerf, size_t upperf,float window_len,float shift)
{
	return new FeatureExtractor(nfft,melcount,sample_rate,lowerf,upperf,window_len,shift);
}
	
void deleteFeatureExtractor(FeatureExtractor*impl)
{
	delete impl;
	impl = 0;
}


int signalToMel(FeatureExtractor*impl,const int16_t * const pcm, size_t len,uint8_t*result,float gain)
{
	return impl->signal_to_mel(pcm,len,result,gain);
}


size_t getMelcount(FeatureExtractor*impl)
{
	return impl->get_melcount();
}









