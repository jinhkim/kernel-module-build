#!/bin/bash

OS_VERSION=$(echo $RESIN_HOST_OS_VERSION | cut -d " " -f 2)
mod_dir="egalax_touchscreen_driver_${RESIN_DEVICE_TYPE}_${OS_VERSION}*"

insmod $mod_dir/$DRIVER_NAME.ko
result=$?
lsmod | grep $DRIVER_NAME
#rmmod $DRIVER_NAME
if [ $result == 0 ]; then
	echo "egalax touchscreen kernel module loaded!"
else
	echo "ERROR could not load $DRIVER_NAME touchscreen kernel!!!"
fi

xinit /xinitrc -- :0 -nocursor -dpms -s off -s noblank
