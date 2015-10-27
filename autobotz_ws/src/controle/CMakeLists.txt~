cmake_minimum_required(VERSION 2.8.3)
project(controle)

#find_package(catkin REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
  genmsg
  message_generation
)
 

add_message_files(
   FILES
   squareCenters.msg
   velocidade.msg
 )

generate_messages(
   DEPENDENCIES
   std_msgs
   geometry_msgs
 )


catkin_package(
 CATKIN_DEPENDS message_runtime 
)

# include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include ${catkin_INCLUDE_DIRS})

#----------------------- CONTROLE  ------------------------#

add_executable(control src/main.cpp src/Funcoes/auxiliares.cpp) 
target_link_libraries(control ${catkin_LIBRARIES}) 

add_executable(control_v2 src/main_v2.cpp src/Funcoes/auxiliares.cpp) 
target_link_libraries(control_v2 ${catkin_LIBRARIES}) 

add_dependencies(control ${controle_EXPORTED_TARGETS})
add_dependencies(control_v2 ${controle_EXPORTED_TARGETS})

