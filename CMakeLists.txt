cmake_minimum_required(VERSION 3.7.0)

project(pulse VERSION 1.0.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()
add_compile_options(-Wall -Wconversion -Wextra -pedantic)
add_library(cpptimer STATIC src/CppTimer.cpp)
TARGET_LINK_LIBRARIES(cpptimer rt fcgi ${CMAKE_THREAD_LIBS_INIT} curl)
set_target_properties(cpptimer PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(QCustomPlot)
find_library(wiringPi_LIB wiringPi)
find_package(Threads REQUIRED)
find_package( CURL )

add_executable(drmhk
  src/CppTimer.cpp
  include/mainwindow.cpp
  main/main.cpp
)

target_link_libraries(drmhk
  Qt5::Widgets Qt5::PrintSupport Threads::Threads cpptimer ${QCustomPlot_LIBRARIES} ${wiringPi_LIB}  fcgi rt ${CMAKE_THREAD_LIBS_INIT} curl
  )

