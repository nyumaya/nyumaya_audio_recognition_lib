# nyumaya_audio_recognition_lib

[![Build Status](https://travis-ci.org/nyumaya/nyumaya_audio_recognition_lib.svg?branch=V0.3)](https://travis-ci.org/nyumaya/nyumaya_audio_recognition_lib)

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

## Credits


- The library uses [https://github.com/mborgerding/kissfft](kissfft)
