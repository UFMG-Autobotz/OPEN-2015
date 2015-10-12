# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(WARNING "Invoking generate_messages() without having added any message or service file before.
You should either add add_message_files() and/or add_service_files() calls or remove the invocation of generate_messages().")
message(STATUS "radio_controle: 0 messages, 0 services")

set(MSG_I_FLAGS "-Istd_msgs:/opt/ros/hydro/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(radio_controle_generate_messages ALL)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages

### Generating Services

### Generating Module File
_generate_module_cpp(radio_controle
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/radio_controle
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(radio_controle_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(radio_controle_generate_messages radio_controle_generate_messages_cpp)

# target for backward compatibility
add_custom_target(radio_controle_gencpp)
add_dependencies(radio_controle_gencpp radio_controle_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS radio_controle_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages

### Generating Services

### Generating Module File
_generate_module_lisp(radio_controle
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/radio_controle
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(radio_controle_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(radio_controle_generate_messages radio_controle_generate_messages_lisp)

# target for backward compatibility
add_custom_target(radio_controle_genlisp)
add_dependencies(radio_controle_genlisp radio_controle_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS radio_controle_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages

### Generating Services

### Generating Module File
_generate_module_py(radio_controle
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/radio_controle
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(radio_controle_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(radio_controle_generate_messages radio_controle_generate_messages_py)

# target for backward compatibility
add_custom_target(radio_controle_genpy)
add_dependencies(radio_controle_genpy radio_controle_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS radio_controle_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/radio_controle)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/radio_controle
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(radio_controle_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/radio_controle)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/radio_controle
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(radio_controle_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/radio_controle)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/radio_controle\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/radio_controle
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(radio_controle_generate_messages_py std_msgs_generate_messages_py)
