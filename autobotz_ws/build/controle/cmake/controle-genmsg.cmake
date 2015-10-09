# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "controle: 1 messages, 0 services")

set(MSG_I_FLAGS "-Icontrole:/home/gustavo/open-2015/autobotz_ws/src/controle/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/indigo/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(controle_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/gustavo/open-2015/autobotz_ws/src/controle/msg/squareCenters.msg" NAME_WE)
add_custom_target(_controle_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "controle" "/home/gustavo/open-2015/autobotz_ws/src/controle/msg/squareCenters.msg" "geometry_msgs/Point"
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(controle
  "/home/gustavo/open-2015/autobotz_ws/src/controle/msg/squareCenters.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/controle
)

### Generating Services

### Generating Module File
_generate_module_cpp(controle
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/controle
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(controle_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(controle_generate_messages controle_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/gustavo/open-2015/autobotz_ws/src/controle/msg/squareCenters.msg" NAME_WE)
add_dependencies(controle_generate_messages_cpp _controle_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(controle_gencpp)
add_dependencies(controle_gencpp controle_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS controle_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(controle
  "/home/gustavo/open-2015/autobotz_ws/src/controle/msg/squareCenters.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/controle
)

### Generating Services

### Generating Module File
_generate_module_lisp(controle
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/controle
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(controle_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(controle_generate_messages controle_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/gustavo/open-2015/autobotz_ws/src/controle/msg/squareCenters.msg" NAME_WE)
add_dependencies(controle_generate_messages_lisp _controle_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(controle_genlisp)
add_dependencies(controle_genlisp controle_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS controle_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(controle
  "/home/gustavo/open-2015/autobotz_ws/src/controle/msg/squareCenters.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/controle
)

### Generating Services

### Generating Module File
_generate_module_py(controle
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/controle
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(controle_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(controle_generate_messages controle_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/gustavo/open-2015/autobotz_ws/src/controle/msg/squareCenters.msg" NAME_WE)
add_dependencies(controle_generate_messages_py _controle_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(controle_genpy)
add_dependencies(controle_genpy controle_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS controle_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/controle)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/controle
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(controle_generate_messages_cpp std_msgs_generate_messages_cpp)
add_dependencies(controle_generate_messages_cpp geometry_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/controle)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/controle
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(controle_generate_messages_lisp std_msgs_generate_messages_lisp)
add_dependencies(controle_generate_messages_lisp geometry_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/controle)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/controle\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/controle
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(controle_generate_messages_py std_msgs_generate_messages_py)
add_dependencies(controle_generate_messages_py geometry_msgs_generate_messages_py)
