# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "estrategia: 2 messages, 0 services")

set(MSG_I_FLAGS "-Iestrategia:/home/gabriela/open-2015/autobotz_ws/src/estrategia/msg;-Istd_msgs:/opt/ros/hydro/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/hydro/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(estrategia_generate_messages ALL)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(estrategia
  "/home/gabriela/open-2015/autobotz_ws/src/estrategia/msg/trajetoria.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/hydro/share/geometry_msgs/cmake/../msg/Pose2D.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/estrategia
)
_generate_msg_cpp(estrategia
  "/home/gabriela/open-2015/autobotz_ws/src/estrategia/msg/velocidade.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/hydro/share/std_msgs/cmake/../msg/Float32.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/estrategia
)

### Generating Services

### Generating Module File
_generate_module_cpp(estrategia
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/estrategia
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(estrategia_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(estrategia_generate_messages estrategia_generate_messages_cpp)

# target for backward compatibility
add_custom_target(estrategia_gencpp)
add_dependencies(estrategia_gencpp estrategia_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS estrategia_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(estrategia
  "/home/gabriela/open-2015/autobotz_ws/src/estrategia/msg/trajetoria.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/hydro/share/geometry_msgs/cmake/../msg/Pose2D.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/estrategia
)
_generate_msg_lisp(estrategia
  "/home/gabriela/open-2015/autobotz_ws/src/estrategia/msg/velocidade.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/hydro/share/std_msgs/cmake/../msg/Float32.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/estrategia
)

### Generating Services

### Generating Module File
_generate_module_lisp(estrategia
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/estrategia
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(estrategia_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(estrategia_generate_messages estrategia_generate_messages_lisp)

# target for backward compatibility
add_custom_target(estrategia_genlisp)
add_dependencies(estrategia_genlisp estrategia_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS estrategia_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(estrategia
  "/home/gabriela/open-2015/autobotz_ws/src/estrategia/msg/trajetoria.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/hydro/share/geometry_msgs/cmake/../msg/Pose2D.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/estrategia
)
_generate_msg_py(estrategia
  "/home/gabriela/open-2015/autobotz_ws/src/estrategia/msg/velocidade.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/hydro/share/std_msgs/cmake/../msg/Float32.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/estrategia
)

### Generating Services

### Generating Module File
_generate_module_py(estrategia
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/estrategia
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(estrategia_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(estrategia_generate_messages estrategia_generate_messages_py)

# target for backward compatibility
add_custom_target(estrategia_genpy)
add_dependencies(estrategia_genpy estrategia_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS estrategia_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/estrategia)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/estrategia
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(estrategia_generate_messages_cpp std_msgs_generate_messages_cpp)
add_dependencies(estrategia_generate_messages_cpp geometry_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/estrategia)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/estrategia
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(estrategia_generate_messages_lisp std_msgs_generate_messages_lisp)
add_dependencies(estrategia_generate_messages_lisp geometry_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/estrategia)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/estrategia\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/estrategia
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(estrategia_generate_messages_py std_msgs_generate_messages_py)
add_dependencies(estrategia_generate_messages_py geometry_msgs_generate_messages_py)
