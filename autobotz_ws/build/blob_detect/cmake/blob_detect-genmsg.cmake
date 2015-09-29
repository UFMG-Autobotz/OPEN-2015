# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "blob_detect: 1 messages, 0 services")

set(MSG_I_FLAGS "-Iblob_detect:/home/husky/open-2015/autobotz_ws/src/blob_detect/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/indigo/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(blob_detect_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/husky/open-2015/autobotz_ws/src/blob_detect/msg/squareCenters.msg" NAME_WE)
add_custom_target(_blob_detect_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "blob_detect" "/home/husky/open-2015/autobotz_ws/src/blob_detect/msg/squareCenters.msg" "geometry_msgs/Point"
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(blob_detect
  "/home/husky/open-2015/autobotz_ws/src/blob_detect/msg/squareCenters.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/blob_detect
)

### Generating Services

### Generating Module File
_generate_module_cpp(blob_detect
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/blob_detect
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(blob_detect_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(blob_detect_generate_messages blob_detect_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/husky/open-2015/autobotz_ws/src/blob_detect/msg/squareCenters.msg" NAME_WE)
add_dependencies(blob_detect_generate_messages_cpp _blob_detect_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(blob_detect_gencpp)
add_dependencies(blob_detect_gencpp blob_detect_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS blob_detect_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(blob_detect
  "/home/husky/open-2015/autobotz_ws/src/blob_detect/msg/squareCenters.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/blob_detect
)

### Generating Services

### Generating Module File
_generate_module_lisp(blob_detect
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/blob_detect
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(blob_detect_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(blob_detect_generate_messages blob_detect_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/husky/open-2015/autobotz_ws/src/blob_detect/msg/squareCenters.msg" NAME_WE)
add_dependencies(blob_detect_generate_messages_lisp _blob_detect_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(blob_detect_genlisp)
add_dependencies(blob_detect_genlisp blob_detect_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS blob_detect_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(blob_detect
  "/home/husky/open-2015/autobotz_ws/src/blob_detect/msg/squareCenters.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/blob_detect
)

### Generating Services

### Generating Module File
_generate_module_py(blob_detect
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/blob_detect
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(blob_detect_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(blob_detect_generate_messages blob_detect_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/husky/open-2015/autobotz_ws/src/blob_detect/msg/squareCenters.msg" NAME_WE)
add_dependencies(blob_detect_generate_messages_py _blob_detect_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(blob_detect_genpy)
add_dependencies(blob_detect_genpy blob_detect_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS blob_detect_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/blob_detect)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/blob_detect
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(blob_detect_generate_messages_cpp std_msgs_generate_messages_cpp)
add_dependencies(blob_detect_generate_messages_cpp geometry_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/blob_detect)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/blob_detect
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(blob_detect_generate_messages_lisp std_msgs_generate_messages_lisp)
add_dependencies(blob_detect_generate_messages_lisp geometry_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/blob_detect)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/blob_detect\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/blob_detect
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(blob_detect_generate_messages_py std_msgs_generate_messages_py)
add_dependencies(blob_detect_generate_messages_py geometry_msgs_generate_messages_py)
