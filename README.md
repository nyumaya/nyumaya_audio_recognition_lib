# nyumaya_audio_recognition_lib



## How to build the library


- Before cloning this repository clone the official tensorflow repository

```
https://github.com/tensorflow/tensorflow.git 4f2a9acaff04bb81684c2b49c955f296315473ac
```

- Clone this repository within the top folder of the tensorflow repository


```
cd tensorflow
git@github.com:nyumaya/nyumaya_audio_recognition_lib.git

```

- Modify two tensorflow files

```
cp download_dependencies.sh ../tensorflow/lite/tools/make/
cp register.cc ../tensorflow/lite/kernels/
```

- Download dependencies

```
../tensorflow/lite/tools/make/download_dependencies.sh 
```

- Make

```
cmake ./
make 
```




## Credits


- The library uses [https://github.com/mborgerding/kissfft](kissfft)
