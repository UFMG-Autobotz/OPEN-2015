echo -n "Configuring directories..."

#delete /devel and /build if they exist
if [ -d "devel" ]; then
	rm -r devel/
fi

if [ -d "build" ]; then
	rm -r build/
fi

#recreate symlink to instalation
if [ -e "src/CMakeLists.txt" ]; then
	rm src/CMakeLists.txt
fi
source /opt/ros/indigo/setup.bash

#copy vision configuration to autobotz_ws/config
if [ -e "src/visao/blob_detect/config/palette.conf" ]; then
	if [ -e "~/open-2015/autobotz_ws/" ]; then
		mkdir ~/open-2015/autobotz_ws/config >/dev/null 2>&1
		cp src/visao/blob_detect/config/palette.conf ~/open-2015/autobotz_ws/config/palette.conf
	fi
fi

echo "  done"
read -p "Press enter to build workspace..."

catkin_make
catkin_make
