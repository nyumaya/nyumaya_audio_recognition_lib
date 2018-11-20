cmake_minimum_required (VERSION 2.8.11)
project (nyumaya)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS_RELEASE " -O3 -march=x86-64 -mtune=generic -no-pie -funsafe-math-optimizations -DGEMMLOWP_ALLOW_SLOW_SCALAR_FALLBACK -s")

# PiZero set(CMAKE_CXX_FLAGS_RELEASE " -O3 -march=native -no-pie -funsafe-math-optimizations -s")
# Pi3    set(CMAKE_CXX_FLAGS_RELEASE " -O3 -march=native -no-pie -funsafe-math-optimizations -mfpu=neon-vfpv4 -DUSE_NEON -s")

set (CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed -Wl,--gc-sections -Wl,--exclude-libs,ALL -Wl,--no-export-dynamic")
if(APPLE)
  set (CMAKE_SHARED_LINKER_FLAGS "-Wl")
endif(APPLE)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

Set(BENCHMARK_SOURCES

     	${CMAKE_SOURCE_DIR}/../tensorflow/lite/tools/benchmark/benchmark_tflite_model.cc
     	${CMAKE_SOURCE_DIR}/../tensorflow/lite/tools/benchmark/benchmark_params.cc
     	${CMAKE_SOURCE_DIR}/../tensorflow/lite/tools/benchmark/benchmark_model.cc
     	${CMAKE_SOURCE_DIR}/../tensorflow/lite/tools/benchmark/benchmark_main.cc
     	${CMAKE_SOURCE_DIR}/../tensorflow/lite/tools/benchmark/command_line_flags.cc
     	#${CMAKE_SOURCE_DIR}/../tensorflow/lite/testing/init_tensorflow.cc
)

Set(SOURCES


		${CMAKE_SOURCE_DIR}/../tensorflow/lite/profiling/time.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/profiling/profile_summarizer.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/core/util/stats_calculator.cc


		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/tools/make/downloads/farmhash/src/farmhash.cc
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/tools/make/downloads/fft2d/fftsg.c


		${CMAKE_SOURCE_DIR}/../tensorflow/lite/allocation.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/arena_planner.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/graph_info.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/interpreter.cc
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/mmap_allocation.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/mmap_allocation_disabled.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/model.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/mutable_op_resolver.cc
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/nnapi_delegate.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/nnapi_delegate_disabled.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/optional_debug_tools.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/simple_memory_arena.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/stderr_reporter.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/string_util.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/util.cc

		${CMAKE_SOURCE_DIR}/../tensorflow/lite/c/c_api_internal.c

		${CMAKE_SOURCE_DIR}/../tensorflow/lite/core/api/error_reporter.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/core/api/flatbuffer_conversions.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/core/api/op_resolver.cc


		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/activations.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/add.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/arg_min_max.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/basic_rnn.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/batch_to_space_nd.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/bidirectional_sequence_lstm.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/bidirectional_sequence_rnn.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/cast.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/comparisons.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/concatenation.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/conv.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/depthwise_conv.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/dequantize.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/detection_postprocess.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/div.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/eigen_support.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/elementwise.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/embedding_lookup.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/embedding_lookup_sparse.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/expand_dims.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/exp.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/fake_quant.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/floor.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/floor_div.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/fully_connected.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/gather.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/gemm_support.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/hashtable_lookup.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/kernel_util.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/l2norm.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/layer_norm_lstm.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/local_response_norm.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/logical.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/lsh_projection.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/lstm.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/lstm_eval.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/maximum_minimum.cc

		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/mul.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/neg.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/one_hot.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/pack.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/pad.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/pooling.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/pow.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/reduce.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/register.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/relu1.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/reshape.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/resize_bilinear.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/select.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/shape.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/skip_gram.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/slice.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/space_to_batch_nd.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/space_to_depth.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/sparse_to_dense.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/split.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/squeeze.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/strided_slice.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/sub.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/svdf.cc

		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/tile.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/topk_v2.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/transpose.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/transpose_conv.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/unidirectional_sequence_lstm.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/unidirectional_sequence_rnn.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/unpack.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/zeros_like.cc

		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/kernel_utils.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/quantization_util.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/tensor_utils.cc

		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/optimized/neon_tensor_utils.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/reference/portable_tensor_utils.cc
		${CMAKE_SOURCE_DIR}/../tensorflow/lite/tools/make/downloads/farmhash/src/farmhash.cc
		
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/test_util.cc
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/test_util.cc
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/mfcc.cc
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/mfcc_dct.cc
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/mfcc_dct.h
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/mfcc.h
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/mfcc_mel_filterbank.cc
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/mfcc_mel_filterbank.h
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/internal/spectrogram.cc
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/mfcc.cc
		#${CMAKE_SOURCE_DIR}/../tensorflow/lite/kernels/audio_spectrogram.cc
		
		${CMAKE_SOURCE_DIR}/kissfft/kiss_fft.c
		${CMAKE_SOURCE_DIR}/kissfft/tools/kiss_fftr.c

)



add_library(nyumaya SHARED ${SOURCES} lib.cpp AudioRecognitionImpl.cpp feature_extract.cpp)

add_executable(benchmark  ${SOURCES}  ${BENCHMARK_SOURCES})

target_link_libraries(nyumaya Threads::Threads)
if(APPLE)
target_link_libraries(nyumaya m -static-libstdc++ )
else()
target_link_libraries(nyumaya m -static-libgcc -static-libstdc++ )
endif()
target_link_libraries(nyumaya z)


target_link_libraries(benchmark Threads::Threads)
target_link_libraries(benchmark m)
target_link_libraries(benchmark z)



target_include_directories(nyumaya
	PUBLIC ../
	INTERFACE ../tensorflow/lite/
	PUBLIC ../tensorflow/lite/tools/make/downloads/flatbuffers/include
	PUBLIC ../tensorflow/lite/tools/make/downloads/eigen
	PUBLIC ../tensorflow/lite/tools/make/downloads/gemmlowp
	PUBLIC ../tensorflow/lite/tools/make/downloads/farmhash/src
	PUBLIC ../tensorflow/lite/tools/make/downloads/neon_2_sse
	PUBLIC ../tensorflow/lite/tools/make/downloads/absl
	PUBLIC ./kissfft
)


target_include_directories(benchmark
	PUBLIC ../
	PUBLIC ../tensorflow/lite/tools/benchmark/
	INTERFACE ../tensorflow/lite/
	PUBLIC ../tensorflow/lite/tools/make/downloads/flatbuffers/include
	PUBLIC ../tensorflow/lite/tools/make/downloads/eigen
	PUBLIC ../tensorflow/lite/tools/make/downloads/gemmlowp
	PUBLIC ../tensorflow/lite/tools/make/downloads/farmhash/src
	PUBLIC ../tensorflow/lite/tools/make/downloads/neon_2_sse
	PUBLIC ../tensorflow/lite/tools/make/downloads/absl
	PUBLIC ./kissfft
)





