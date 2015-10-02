cmake_minimum_required(VERSION 2.8.3)
project(estrategia)

#find_package(catkin REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)
 

catkin_package()
# CATKIN_DEPENDS message_runtime 
#)

#include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include ${catkin_INCLUDE_DIRS})

#----------------------- ESTRATEGIA  ------------------------#

add_executable(main src/main.cpp src/estados.h) 
target_link_libraries(main ${catkin_LIBRARIES}) 

