#! /bin/bash
catkin_make_isolated --install --use-ninja \
  --cmake-args \
  -DCMAKE_PREFIX_PATH="/home/cat/cartographer_ws/install_isolated;/opt/ros/noetic;/usr"
