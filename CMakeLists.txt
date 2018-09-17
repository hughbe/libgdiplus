project(Libgdiplus)
include(CheckCXXCompilerFlag)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(TestBigEndian)

cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0003 NEW)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
enable_testing()

# Set variables.
set(VERSION "5.6")

if(MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4018 /wd4305")
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Find GLIB. Minimum: 2.2.3.
find_package(GLIB)
include_directories(${GLIB_INCLUDE_DIRS})

# Find Cairo. Minimum: 1.6.4.
find_package(Cairo REQUIRED)
include_directories(${CAIRO_INCLUDE_DIRS})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(Fontconfig REQUIRED)
include_directories(${Fontconfig_INCLUDE_DIRS})

find_package(JPEG)
set(HAVE_LIBJPEG ${JPEG_FOUND})
include_directories(${JPEG_INCLUDE_DIRS})

find_package(GIF)
set(HAVE_LIBGIF ${GIF_FOUND})
include_directories(${GIF_INCLUDE_DIRS})

find_package(TIFF)
set(HAVE_LIBTIFF ${TIFF_FOUND})
include_directories(${TIFF_INCLUDE_DIRS})

find_package(PNG)
set(HAVE_LIBPNG ${PNG_FOUND})
include_directories(${PNG_INCLUDE_DIRS})

find_package(Threads)
set(HAVE_LIBPTHREAD ${Threads_FOUND})
include_directories(${THREADS_INCLUDE_DIRS})

find_package(X11)
set(HAVE_X11 ${X11_FOUND})
include_directories(${X11_INCLUDE_DIR})

find_package(EXIF)
set(HAVE_LIBEXIF ${EXIF_FOUND})
include_directories(${EXIF_INCLUDE_DIRS})

if(APPLE)
  find_library(IOKit IOKit)
  find_library(CoreFoundation CoreFoundation)
  find_library(CoreGraphics CoreGraphics)
endif()

set(libraries
  ${GLIB_LIBRARIES}
  ${CAIRO_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${Fontconfig_LIBRARIES}
  ${JPEG_LIBRARIES}
  ${PNG_LIBRARIES}
  ${THREADS_LIBRARIES}
  ${EXIF_LIBRARIES}
)

if(HAVE_LIBGIF)
  set(libraries ${libraries} ${GIF_LIBRARIES})
endif()
if(HAVE_LIBTIFF)
  set(libraries ${libraries} ${TIFF_LIBRARIES})
endif()
if(HAVE_X11)
  set(libraries ${libraries} ${X11_LIBRARIES})
endif()
if(HAVE_EXIF)
  set(libraries ${libraries} ${EXIF_LIBRARIES})
endif()

# Set up the config file.
set(CMAKE_EXTRA_INCLUDE_FILES fontconfig.h)
set(CMAKE_REQUIRED_LIBRARIES ${Fontconfig_LIBRARIES})
check_function_exists(FcFini HAVE_FCFINI)
set(CMAKE_EXTRA_INCLUDE_FILES)
set(CMAKE_REQUIRED_LIBRARIES)

test_big_endian(WORDS_BIGENDIAN)

check_cxx_source_compiles("void __attribute__ ((visibility (\"hidden\"))) doit (void) {} int main () { doit (); }" HAVE_VISIBILITY_HIDDEN)

# sigsetjmp is a macro on some platforms, so can't use check_function_exists.
check_cxx_source_compiles("
#include <setjmp.h>
int main() {
  sigjmp_buf env;
  sigsetjmp(env, 0);
}" HAVE_SIGSETJMP)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

option(LIBGDIPLUS_ASAN_BUILD "Use Address Sanitizer" FALSE)
option(LIBGDIPLUS_FUZZER_BUILD "Use LibFuzzer" FALSE)

if (LIBGDIPLUS_ASAN_BUILD OR LIBGDIPLUS_FUZZER_BUILD)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
endif()

add_subdirectory(src)
add_subdirectory(tests)
