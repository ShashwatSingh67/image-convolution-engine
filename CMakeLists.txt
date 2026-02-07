cmake_minimum_required(VERSION 3.20)
project(image_convolution_engine LANGUAGES CXX)

#settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#warning and complier flasgs
if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# core folder
add_library(imgcore
  src/core/image.cpp
)
target_include_directories(imgcore PUBLIC src)

# CLI folder
add_executable(imgconv
  src/cli/main.cpp
)
target_link_libraries(imgconv PRIVATE imgcore)

#testign folder
enable_testing()
add_executable(test_image
  tests/test_image.cpp
)
target_link_libraries(test_image PRIVATE imgcore)

add_test(NAME test_image COMMAND test_image)
