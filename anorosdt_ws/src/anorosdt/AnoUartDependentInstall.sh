#! /bin/bash

UBUNTU_VERSION=$(rosversion -d)
#echo "$UBUNTU_VERSION"

if [ -z "$UBUNTU_VERSION" ]; then
	echo "Unable to get the Ubuntu version"
	exit 1
fi

echo "Your version of Ubuntu is: $UBUNTU_VERSION"
echo "Installing ros-$UBUNTU_VERSION-serial..."
sudo apt-get update
sudo apt-get install -y ros-$UBUNTU_VERSION-serial

if [ $? -eq 0 ];then
	echo "ros-$UBUNTU_VERSION-serial successful installation"
else
	echo "ros-$UBUNTU_VERSION-serial installation failed"
fi


#if ! ls /dev/ | grep -q '^ttyAMA' && ! ls /dev/ | grep -q '^ttyAMA2$'; then
#    echo "dtoverlay=uart2" >> /boot/firmware/usercfg.txt
#    echo "已向/boot/firmware/usercfg.txt添加dtoverlay=uart2" 
#else
#    echo "ttyAMA2设备存在"
#fi













