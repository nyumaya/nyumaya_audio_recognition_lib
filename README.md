# nyumaya_audio_recognition_lib

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/c66fbe7fad2942018121844687ede135)](https://app.codacy.com/app/yodakohl/nyumaya_audio_recognition_lib?utm_source=github.com&utm_medium=referral&utm_content=nyumaya/nyumaya_audio_recognition_lib&utm_campaign=Badge_Grade_Dashboard)
[![Build Status](https://travis-ci.org/nyumaya/nyumaya_audio_recognition_lib.svg?branch=master)](https://travis-ci.org/nyumaya/nyumaya_audio_recognition_lib)
[![GitHub Release](https://github-basic-badges.herokuapp.com/release/nyumaya/nyumaya_audio_recognition_lib.svg)]()
[![Gitter chat](https://badges.gitter.im/gitterHQ/gitter.png)](https://gitter.im/nyumaya_audio_recognition)

**Warning this is still work in progress, especially the CMake file has a hardcoded target for Linux x86/64. Any other platforms will currently not work out of the box and require some tweaking**


## Depedencies
-Cmake

## How to build the library


- Before cloning this repository clone the official tensorflow repository

```
git clone https://github.com/tensorflow/tensorflow.git 
cd tensorflow
git checkout 49372a1c6e4e83f153c740996c88b6bf76ec1abf 
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
