execute_process(COMMAND "/home/gabriela/open-2015/autobotz_ws/build/rosserial/rosserial_xbee/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/gabriela/open-2015/autobotz_ws/build/rosserial/rosserial_xbee/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
