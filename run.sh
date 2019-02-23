#!/bin/sh
echo -e "RUNNING RUN.SH"
#. /home/eis/ecoationROS/devel/setup.sh && \
	#ROSCONSOLE_FORMAT='[${severity}] [${node}] [${time}]: ${message}' \
	#UUID=00000000-0000-0000-iris-00000000000 \
	#roslaunch ecoation_bringup ecoation_system.launch

echo "run current process: $$"
xinit ./xinitrc -- :0 -nocursor -dpms -s off -s noblank
