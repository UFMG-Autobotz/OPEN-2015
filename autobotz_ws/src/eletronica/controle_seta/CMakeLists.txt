
cmake_minimum_required(VERSION 2.8.3)
project(controle_seta)

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

add_message_files(FILES)
 
generate_messages(
   DEPENDENCIES
  std_msgs
)
 
catkin_package(
 CATKIN_DEPENDS message_runtime 
)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include ${catkin_INCLUDE_DIRS})


add_executable(seta src/main.cpp) 
target_link_libraries(seta ${catkin_LIBRARIES})