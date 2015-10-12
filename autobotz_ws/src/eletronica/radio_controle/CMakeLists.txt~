cmake_minimum_required(VERSION 2.8.3)
project(radio_controle)

find_package(catkin REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
  std_msgs
  rospy  
  genmsg 
  geometry_msgs
  message_generation
)
 
find_package(OpenCV REQUIRED)
 
generate_messages(
   DEPENDENCIES
  std_msgs
)
 
catkin_package(
 CATKIN_DEPENDS message_runtime 
)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include ${catkin_INCLUDE_DIRS})

#----------------------- RÁDIO CONTROLE  ------------------------#

add_executable(radio_talker src/radio/main.cpp ) 
target_link_libraries(radio_talker ${catkin_LIBRARIES}) 

add_executable(radio_listener src/radio/listener.cpp)
target_link_libraries(radio_listener ${catkin_LIBRARIES})
