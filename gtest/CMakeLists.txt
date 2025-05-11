set(target "gtest")
file(GLOB src "gtest-all.cc")
add_library(${target} STATIC gtest-all.cc)