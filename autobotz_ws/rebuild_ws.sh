if [ -d "devel" ]; then
	rm -r devel/
fi

if [ -d "build" ]; then
	rm -r build/
fi

if [ -e "src/CMakeLists.txt" ]; then
	rm src/CMakeLists.txt
fi

source /opt/ros/indigo/setup.bash

catkin_make
catkin_make
