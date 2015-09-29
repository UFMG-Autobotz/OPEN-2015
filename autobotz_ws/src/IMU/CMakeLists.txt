
cmake_minimum_required(VERSION 2.8.3)
project(beginner_tutorials)

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

add_message_files(FILES Num.msg)
 
generate_messages(
   DEPENDENCIES
  std_msgs
)
 
catkin_package(
 CATKIN_DEPENDS message_runtime 
)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include ${catkin_INCLUDE_DIRS})

#--------------------- IMU + PROCESSAMENTO ----------------------#

add_executable(arducom src/arducom/IMU_Data.cpp ) ##usada na IMU
target_link_libraries(arducom ${catkin_LIBRARIES}) 

add_executable(listener src/Processamento/main.cpp) ##usada no processamento
target_link_libraries(listener ${catkin_LIBRARIES})


