rm -r devel/
rm -r build/
rm src/CMakeLists.h

source /opt/ros/indigo/setup.bash

catkin_make
