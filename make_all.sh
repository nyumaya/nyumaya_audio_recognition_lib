#!/bin/bash

generic_cxx_flags=" -O3 -DABSL_FLAGS_STRIP_NAMES -DNDEBUG -ffunction-sections -fomit-frame-pointer -fvisibility-inlines-hidden -fvisibility=hidden -ffast-math -fPIC -funsafe-math-optimizations "
generic_c_flags=" -O3 -DABSL_FLAGS_STRIP_NAMES -DNDEBUG -ffunction-sections -fomit-frame-pointer -fvisibility-inlines-hidden -fvisibility=hidden -ffast-math -fPIC -funsafe-math-optimizations  "

build_base="release"


clean_build=true
debug_build=false


if [ -x "$(command -v /usr/bin/clang)" ]; then
	export CC=/usr/bin/clang
fi

if [ -x "$(command -v /usr/bin/clang)" ]; then
	export CXX=/usr/bin/clang++
fi

if [ -x "$(command -v /usr/bin/clang-6.0)" ]; then
	export CC=/usr/bin/clang-6.0
fi

if [ -x "$(command -v /usr/bin/clang++-6.0)" ]; then
	export CXX=/usr/bin/clang++-6.0
fi


if [ "$debug_build" = true ] ; then
	generic_cxx_flags=" -O0 -g -fPIC  "
	generic_c_flags=" -O0 -g -fPIC "
	build_base="debug"
fi



function clean_build_dir {
	if [ "$clean_build" = true ] ; then
		rm -rf $1
	fi
}

function build_linux_generic {

	build_dir="build/${build_base}/linux_generic"
	clean_build_dir "${build_dir}"
	mkdir -p "${build_dir}"  
	cd "${build_dir}"

	cxx_flags="${generic_cxx_flags} -march=x86-64 -mtune=generic -DPFFFT_SIMD_DISABLE "
	c_flags="${generic_c_flags}  -march=x86-64 -mtune=generic -DPFFFT_SIMD_DISABLE "

	export CFLAGS="${c_flags}"
	export CXXFLAGS="${cxx_flags}"

	cmake ../../../
	make -w
	cd ../../../
}

function build_linux_native {

	build_dir="build/${build_base}/linux_native"
	clean_build_dir "${build_dir}"
	mkdir -p "${build_dir}"  
	cd "${build_dir}"

	cxx_flags="${generic_cxx_flags} -march=native -mtune=native "
	c_flags="${generic_c_flags} -march=native -mtune=native "

	export CFLAGS="${c_flags}"
	export CXXFLAGS="${cxx_flags}"

	cmake ../../../
	make -w
	cd ../../../
}



function build_pi_zero {


	build_dir="build/${build_base}/linux_rpi_zero"
	clean_build_dir "${build_dir}"
	mkdir -p "${build_dir}"  
	cd "${build_dir}"

	cxx_flags="${generic_cxx_flags} -mfpu=vfp -mfloat-abi=hard -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -DPFFFT_SIMD_DISABLE " #DTFLITE_WITH_RUY for pi zero slow
	c_flags="${generic_c_flags} -mfpu=vfp -mfloat-abi=hard -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -DPFFFT_SIMD_DISABLE "

	export CFLAGS="${c_flags}"
	export CXXFLAGS="${cxx_flags}"

	cmake -DCMAKE_TOOLCHAIN_FILE=./boards/raspberry_pi_zero.cmake ../../../ 


	make -w
	cd ../../../
}


function build_pi_3 {

	build_dir="build/${build_base}/linux_rpi_3"
	clean_build_dir "${build_dir}"
	mkdir -p "${build_dir}"  
	cd "${build_dir}"

	cxx_flags="${generic_cxx_flags}  -mcpu=cortex-a53 -mfloat-abi=hard -mtune=cortex-a53 -mfpu=neon -DUSE_NEON "
	c_flags="${generic_c_flags}  -mcpu=cortex-a53 -mfloat-abi=hard -mtune=cortex-a53 -mfpu=neon -DUSE_NEON "

	export CFLAGS="${c_flags}"
	export CXXFLAGS="${cxx_flags}"

	cmake -DCMAKE_TOOLCHAIN_FILE=./boards/raspberry_pi_3.cmake ../../../
	make
	cd ../../../

}


function build_android {

	ABI=$1
	NEON=$2
	MINSDKVERSION=21

	build_dir="build/${build_base}/android_${ABI}"
	clean_build_dir "${build_dir}"
	mkdir -p "${build_dir}"  
	cd "${build_dir}"

	cxx_flags="${generic_cxx_flags}  "
	c_flags="${generic_c_flags}  "

	export cFlags="${c_flags} -s"
	export cppFlags="${cxx_flags} -s"

	STRIP=OFF
	
	if [ "$debug_build" = true ] ; then
		BUILD_TYPE=Debug
	else 
		BUILD_TYPE=Release
		STRIP=ON
	fi

	cmake \
		-DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
		-DANDROID_ABI=$ABI \
		-DSTRIP_ANDROID=${STRIP} \
		-DSTATIC_CLIB=ON \
		-DBUILD_TEST=OFF \
		-DANDROID_ARM_NEON=${NEON} \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE}  \
		-DANDROID_NATIVE_API_LEVEL=$MINSDKVERSION ../../../



	make -w

	cd ../../../

}

build_linux_generic
build_linux_native
build_pi_zero
build_pi_3
build_android "x86" "OFF"
build_android "arm64-v8a" "ON"
build_android "x86_64" "OFF"
build_android "armeabi-v7a with NEON" "ON"

#build_android "armeabi-v7a" "OFF" #NOT WORKING






