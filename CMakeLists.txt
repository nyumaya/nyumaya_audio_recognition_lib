cmake_minimum_required (VERSION 2.8.11)
project (nyumaya)


option(BUILD_BENCHMARK "Build Benchmark" OFF)
option(STATIC_CLIB     "Build Static CLib" ON)
option(BUILD_LIB       "Build nyumaya lib" ON)
option(BUILD_TEST      "Build Feature Extraction Tests" ON)

option(STRIP_ANDROID   "Execute Strip Command for Android" OFF)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set (CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed -Wl,--gc-sections -fvisibility=hidden -Wl,--exclude-libs,ALL -Wl,--no-export-dynamic")
if(APPLE)
  set (CMAKE_SHARED_LINKER_FLAGS "-Wl -fvisibility=hidden ")
endif(APPLE)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include("tensorflow_sources")


if(BUILD_LIB)

	add_library(nyumaya SHARED ${SOURCES} lib.cpp libjni.h AudioRecognitionImpl.cpp ModelLoader.cpp feature_extract.cpp )

	if(APPLE)
		target_link_libraries(nyumaya -static-libstdc++ -latomic)
	else()
		if(STATIC_CLIB)
			target_link_libraries(nyumaya -static-libstdc++ -latomic ) #-static-libgcc
		else()
			target_link_libraries(nyumaya -latomic )
		endif()

	endif()
	target_include_directories(nyumaya PUBLIC $ENV{JAVA_HOME}/include)
	target_include_directories(nyumaya PUBLIC $ENV{JAVA_HOME}/include/linux)
	target_include_directories(nyumaya ${INCLUDE_DIRS} )

endif(BUILD_LIB)

if(STRIP_ANDROID)
	add_custom_command(TARGET nyumaya POST_BUILD
            COMMAND "${ANDROID_TOOLCHAIN_PREFIX}strip" -g -S -d --strip-debug --verbose
            "libnyumaya.so"
            COMMENT "Strip debug symbols done on final binary.")

endif(STRIP_ANDROID)


if(BUILD_BENCHMARK)
	add_executable(benchmark  ${SOURCES}  ${BENCHMARK_SOURCES})
	target_link_libraries(benchmark Threads::Threads)
	target_link_libraries(benchmark m)
	target_link_libraries(benchmark z  -latomic )

	target_include_directories(benchmark
		PUBLIC ../tensorflow/lite/tools/benchmark/
		${INCLUDE_DIRS} 
	)

endif(BUILD_BENCHMARK)




if(BUILD_TEST)

	add_executable(test_fft test.cpp)
	target_link_libraries(test_fft nyumaya)	
	#target_link_libraries(test_fft Threads::Threads)
	#target_link_libraries(test_fft -static-libstdc++)


	#add_executable(util_test ${CMAKE_SOURCE_DIR}/../tensorflow/lite/util_test.cc)
	#target_include_directories(util_test ${INCLUDE_DIRS} )
endif(BUILD_TEST)







