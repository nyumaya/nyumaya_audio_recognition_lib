# nyumaya_audio_recognition_lib

**Warning this is still work in progress, especially the CMake file has a hardcoded target for Linux x86/64. Any other platforms will currently not work out of the box and require some tweaking**


## Depedencies
-Cmake

## How to build the library


- Before cloning this repository clone the official tensorflow repository

```
git clone https://github.com/tensorflow/tensorflow.git 
cd tensorflow
git checkout ff2da7862e43aa624ac1f4689ce87eb929df8745 
```

- Clone this repository within the top folder of the tensorflow repository


```
git clone https://github.com/nyumaya/nyumaya_audio_recognition_lib.git

```

- Modify the registered ops

```
cd nyumaya_audio_recognition_lib/
cp register.cc ../tensorflow/lite/kernels/
```

- Download dependencies

```
../tensorflow/lite/tools/make/download_dependencies.sh 
```

- Init kiss_fft

```
git submodule update --init kissfft/
```

- Make

```
cmake ./
make 
```


## How it works

# The Feature extractor
The Feature extractor is responsible for turning PCM encoded audio data into 8bit mel-spectrogram features. You might question yourself why the feature extractor is seperate and why it uses only 8bits. Some applications like verifying if a hotword is issued by a certain speaker require two models running the same audio data. Having the feature extractor as seperate entity saves the duplicate computation of the mel features. Secondly it can be a convenient way of compressing and transmitting data. 
One second of audio contains  40*98 mel features. You can capture your audio on a lightweight system (like ESP32) and transmit the features to a more powerful system. This only requires 40*98*8bit = 3kbit per second.



## Credits


- The library uses [https://github.com/mborgerding/kissfft](kissfft)
