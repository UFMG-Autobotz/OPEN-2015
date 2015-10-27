

cmake_minimum_required(VERSION 2.8.3)
project(estrategia)

# find_package(catkin REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  genmsg
  message_generation
  std_msgs
  geometry_msgs
)
 


add_message_files(
   FILES
   velocidade.msg
   trajetoria.msg
   feature.msg
   featureVec.msg
 )


generate_messages(
   DEPENDENCIES
   std_msgs
   geometry_msgs
)

catkin_package(
 #CATKIN_DEPENDS message_runtime 
)



#include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include ${catkin_INCLUDE_DIRS})

#----------------------- ESTRATEGIA  ------------------------ #


add_executable(strategy src/robo.cpp src/pegarBloco.cpp src/transportarBloco.cpp src/estados.cpp src/Funcoes/auxiliares.cpp
src/main.cpp) 
target_link_libraries(strategy ${catkin_LIBRARIES}) 


add_dependencies(strategy ${estrategia_EXPORTED_TARGETS})

