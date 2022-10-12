cmake_minimum_required(VERSION 3.20)
project(tmpgfn)

set(CMAKE_CXX_STANDARD 17)

add_library(tmpgfn INTERFACE)
target_include_directories(tmpgfn INTERFACE include)

add_executable(test_tmpgfn
  test_tmpgfn.cpp
)
target_link_libraries(test_tmpgfn PRIVATE
  tmpgfn
)
