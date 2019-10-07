# nyumaya_audio_recognition_lib

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/c66fbe7fad2942018121844687ede135)](https://app.codacy.com/app/yodakohl/nyumaya_audio_recognition_lib?utm_source=github.com&utm_medium=referral&utm_content=nyumaya/nyumaya_audio_recognition_lib&utm_campaign=Badge_Grade_Dashboard)
[![Build Status](https://travis-ci.org/nyumaya/nyumaya_audio_recognition_lib.svg?branch=master)](https://travis-ci.org/nyumaya/nyumaya_audio_recognition_lib)
[![GitHub Release](https://github-basic-badges.herokuapp.com/release/nyumaya/nyumaya_audio_recognition_lib.svg)]()
[![Gitter chat](https://badges.gitter.im/gitterHQ/gitter.png)](https://gitter.im/nyumaya_audio_recognition)



**You don't have to build this library by yourself if you want to run the nyumaya_audio_recognition on the Raspberry Pi
,macOS or Linux x86-64. There are prebuilt libraries [here](https://github.com/nyumaya/nyumaya_audio_recognition/tree/master/lib).**

## Depedencies
-Cmake
-Clang-6 compiler (Can be changed in make_all.sh)


## Prerequisite

To Build for Android and export Java Bindings for non Android builds, two environment 
Variables have to be set

export JAVA_HOME="/usr/lib/jvm/java-11-openjdk-amd64/"
export ANDROID_NDK="/opt/android/android-ndk-r20-linux-x86_64/android-ndk-r20"

For Crosscompiling to Raspberry the build system expects the Raspbian toolchain:

- nyumaya_audio_recognition_lib/toolchains/raspbian-armhf
- nyumaya_audio_recognition_lib/toolchains/tools

## How to build the library


- Before cloning this repository clone the official tensorflow repository

```
git clone https://github.com/tensorflow/tensorflow.git 
cd tensorflow
git checkout 51c7df0cfc45d31c2ce2cd61e5c66969d890de2a 
```

- Clone this repository within the top folder of the tensorflow repository


```
git clone https://github.com/nyumaya/nyumaya_audio_recognition_lib.git

```

- Modify the registered ops

```
cd nyumaya_audio_recognition_lib/
git apply --unsafe-paths --directory=../ tflite.patch
```

- Download dependencies

```
../tensorflow/lite/tools/make/download_dependencies.sh 
```


- Make

```
bash make_all.sh
```

