cmake_minimum_required(VERSION 3.20)
project(image_convolution_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(imgcore STATIC
  src/core/image.cpp
  src/core/convolver.cpp
  src/core/kernel.cpp
  src/core/gaussian.cpp
  src/core/gaussian2d.cpp
)

target_include_directories(imgcore PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(imgconv
  src/cli/main.cpp
  src/cli/io_stb.cpp
)
target_link_libraries(imgconv PRIVATE imgcore)

# ---- Benchmark binary ----
add_executable(imgbench
  src/bench/bench_main.cpp
  src/bench/bench_metrics.cpp
  src/cli/io_stb.cpp
)
target_link_libraries(imgbench PRIVATE imgcore)

enable_testing()

add_executable(test_image tests/test_image.cpp)
target_link_libraries(test_image PRIVATE imgcore)
add_test(NAME test_image COMMAND test_image)

add_executable(test_padding tests/test_padding.cpp)
target_link_libraries(test_padding PRIVATE imgcore)
add_test(NAME test_padding COMMAND test_padding)

add_executable(test_kernel tests/test_kernel.cpp)
target_link_libraries(test_kernel PRIVATE imgcore)
add_test(NAME test_kernel COMMAND test_kernel)

add_executable(test_convolve2d tests/test_convolve2d.cpp)
target_link_libraries(test_convolve2d PRIVATE imgcore)
add_test(NAME test_convolve2d COMMAND test_convolve2d)
