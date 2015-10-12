# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "square_detect: 1 messages, 0 services")

set(MSG_I_FLAGS "-Isquare_detect:/home/gabriela/open-2015/autobotz_ws/src/visao/velho/square_detect/msg;-Istd_msgs:/opt/ros/hydro/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/hydro/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(square_detect_generate_messages ALL)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(square_detect
  "/home/gabriela/open-2015/autobotz_ws/src/visao/velho/square_detect/msg/squareCenters.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/hydro/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/square_detect
)

### Generating Services

### Generating Module File
_generate_module_cpp(square_detect
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/square_detect
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(square_detect_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(square_detect_generate_messages square_detect_generate_messages_cpp)

# target for backward compatibility
add_custom_target(square_detect_gencpp)
add_dependencies(square_detect_gencpp square_detect_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS square_detect_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(square_detect
  "/home/gabriela/open-2015/autobotz_ws/src/visao/velho/square_detect/msg/squareCenters.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/hydro/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/square_detect
)

### Generating Services

### Generating Module File
_generate_module_lisp(square_detect
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/square_detect
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(square_detect_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(square_detect_generate_messages square_detect_generate_messages_lisp)

# target for backward compatibility
add_custom_target(square_detect_genlisp)
add_dependencies(square_detect_genlisp square_detect_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS square_detect_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(square_detect
  "/home/gabriela/open-2015/autobotz_ws/src/visao/velho/square_detect/msg/squareCenters.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/hydro/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/square_detect
)

### Generating Services

### Generating Module File
_generate_module_py(square_detect
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/square_detect
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(square_detect_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(square_detect_generate_messages square_detect_generate_messages_py)

# target for backward compatibility
add_custom_target(square_detect_genpy)
add_dependencies(square_detect_genpy square_detect_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS square_detect_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/square_detect)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/square_detect
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(square_detect_generate_messages_cpp std_msgs_generate_messages_cpp)
add_dependencies(square_detect_generate_messages_cpp geometry_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/square_detect)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/square_detect
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(square_detect_generate_messages_lisp std_msgs_generate_messages_lisp)
add_dependencies(square_detect_generate_messages_lisp geometry_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/square_detect)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/square_detect\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/square_detect
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(square_detect_generate_messages_py std_msgs_generate_messages_py)
add_dependencies(square_detect_generate_messages_py geometry_msgs_generate_messages_py)
