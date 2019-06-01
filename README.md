# nyumaya_audio_recognition_lib

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/c66fbe7fad2942018121844687ede135)](https://app.codacy.com/app/yodakohl/nyumaya_audio_recognition_lib?utm_source=github.com&utm_medium=referral&utm_content=nyumaya/nyumaya_audio_recognition_lib&utm_campaign=Badge_Grade_Dashboard)
[![Build Status](https://travis-ci.org/nyumaya/nyumaya_audio_recognition_lib.svg?branch=master)](https://travis-ci.org/nyumaya/nyumaya_audio_recognition_lib)
[![GitHub Release](https://github-basic-badges.herokuapp.com/release/nyumaya/nyumaya_audio_recognition_lib.svg)]()
[![Gitter chat](https://badges.gitter.im/gitterHQ/gitter.png)](https://gitter.im/nyumaya_audio_recognition)

**Warning this is still work in progress, especially the CMake file has a hardcoded target for Linux x86/64. Any other platforms will currently not work out of the box and require some tweaking**

**You don't have to build this library by yourself if you want to run the nyumaya_audio_recognition on the Raspberry Pi
,macOS or Linux x86-64. There are prebuilt libraries [here](https://github.com/nyumaya/nyumaya_audio_recognition/tree/master/lib).**

## Depedencies
-Cmake

## How to build the library


- Before cloning this repository clone the official tensorflow repository

```
git clone https://github.com/tensorflow/tensorflow.git 
cd tensorflow
git checkout 270305c6d5ddded1295c7cf7d05891b119af52c0 
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

- Modifying CMakeLists.txt for your platform

Depending on your platform (like x86-64 desktop, arm on RPi...) you have to set the correct CXX_FLAGS and C_FLAGS. 
Examples for x86-64, RPI3 and RPI_0 are given. To use those architectures comment out the default Flags and comment in
the flags for the chosen platform. The default build is a generic x86-64 version.


- Make

```
cmake ./
make 
```

## Credits


- The library uses [https://github.com/mborgerding/kissfft](kissfft)
