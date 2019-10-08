#ifndef NYUMAYA_AUDIO_RECOGNITION_JNI_H
#define NYUMAYA_AUDIO_RECOGNITION_JNI_H

#include <string>
#include <jni.h>

class AudioRecognitionImpl;
class FeatureExtractor;


#define DEFAULT_VIS  __attribute__ ((visibility ("default") ))

extern "C"
{
	JNIEXPORT jstring Java_com_nyumaya_audiorecognition_NyumayaLibrary_getVersionString(JNIEnv *env, jobject obj);

	JNIEXPORT jlong Java_com_nyumaya_audiorecognition_NyumayaLibrary_createFeatureExtractor(JNIEnv *env, jobject obj,
		jint nfft,jint melcount, jint sample_rate,jint lowerf, jint upperf, jfloat window_len,jfloat shift);


	JNIEXPORT void Java_com_nyumaya_audiorecognition_NyumayaLibrary_deleteFeatureExtractor(JNIEnv *env, jobject obj,jlong impl);

	JNIEXPORT void Java_com_nyumaya_audiorecognition_NyumayaLibrary_deleteAudioRecognition(JNIEnv *env, jobject obj,jlong impl);


	JNIEXPORT jbyteArray Java_com_nyumaya_audiorecognition_NyumayaLibrary_signalToMel(JNIEnv *env, jobject obj,jlong impl,jbyteArray pcm,jfloat gain);

	JNIEXPORT jint Java_com_nyumaya_audiorecognition_NyumayaLibrary_getMelcount(JNIEnv *env, jobject obj,jlong impl);

	JNIEXPORT jlong Java_com_nyumaya_audiorecognition_NyumayaLibrary_createAudioRecognition(JNIEnv *env, jobject obj);

	JNIEXPORT jint Java_com_nyumaya_audiorecognition_NyumayaLibrary_loadModelFromBuffer(JNIEnv *env, jobject obj,jlong impl,jbyteArray modelData);

	JNIEXPORT void Java_com_nyumaya_audiorecognition_NyumayaLibrary_setSensitivity(JNIEnv *env, jobject obj,jlong impl,jfloat sensitivity);

	JNIEXPORT jint Java_com_nyumaya_audiorecognition_NyumayaLibrary_runDetection(JNIEnv *env, jobject obj,jlong impl,jbyteArray mels);

	JNIEXPORT jbyteArray Java_com_nyumaya_audiorecognition_NyumayaLibrary_runRawDetection(JNIEnv *env, jobject obj,jlong impl,jbyteArray mels);


/*	//Audio Recognition
	
	//Input Mel Features and get the raw probabilities of the labels
	uint8_t*RunRawDetection(AudioRecognitionImpl*impl,const uint8_t* const data,const int mel_length) DEFAULT_VIS;

	size_t GetInputDataSize(AudioRecognitionImpl*impl) DEFAULT_VIS;
*/


}





#endif
