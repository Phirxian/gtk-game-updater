cmake_minimum_required(VERSION 3.0)
project(Launcher)

SET(CMAKE_CXX_FLAGS_RELEASE "-O6 -ffast-math -Os")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g2 -pg -fno-inline-functions -fno-inline ")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g2 -pg -fno-inline-functions -fno-inline")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/app)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/app)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/app)

set(CMAKE_USE_PTHREADS_INIT true)
set(CMAKE_USE_PTHREADS_INIT ON)

###############################################################################
################################### defines ###################################
###############################################################################

if( NOT cmake_build_type_tolower STREQUAL "release" )
  add_definitions(-DDEBUG)
endif()

include(CheckCXXCompilerFlag)
if (NOT MSVC)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG(-std=c++0x COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
  endif()
endif()

string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")
add_definitions (-D_GLIBCXX_USE_CXX11_ABI=1)

###############################################################################
################################ dependencies #################################
###############################################################################

find_package(PkgConfig)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

pkg_check_modules(WEBKIT2 REQUIRED webkit2gtk-3.0)
include_directories(${WEBKIT2_INCLUDE_DIRS})
link_directories(${WEBKIT2_LIBRARY_DIRS})

execute_process(COMMAND mysql_config --cflags OUTPUT_VARIABLE MYSQL_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND mysql_config --libs OUTPUT_VARIABLE MYSQL_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories("src/")
include_directories("src/data")

###############################################################################
################################ project app ##################################
###############################################################################

file(GLOB_RECURSE application  src/*.cpp src/*.c src/*.h)

add_executable(launcher src/main.cpp ${application})
target_include_directories(launcher PUBLIC src/app/)
target_include_directories(launcher PUBLIC src/sleek/)

target_link_libraries(
    launcher
    ${GTKMM_LIBRARIES}
    ${WEBKIT2_LIBRARIES}
    ${MYSQL_LIBS}
    -lstdc++fs -lzip -larchive -lpthread -lcurl
)
