#rm -r devel/
#rm -r build/
rm src/CMakeLists.txt

source /opt/ros/indigo/setup.bash

catkin_make
catkin_make # solves the compilation problem about the generate message
