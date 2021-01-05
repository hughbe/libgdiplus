set(fuzzers
  fuzzimage
  fuzzregion
)

include_directories("${CMAKE_SOURCE_DIR}/src")
#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")

foreach(fuzzer ${fuzzers})

  add_executable(${fuzzer} "${fuzzer}.cpp")
  target_compile_options(${fuzzer} PRIVATE -g -O1 -fsanitize=fuzzer,undefined)
  target_link_options(${fuzzer} PRIVATE -fsanitize=fuzzer,undefined)
  target_link_libraries(${fuzzer} libgdiplus)
  
endforeach()
