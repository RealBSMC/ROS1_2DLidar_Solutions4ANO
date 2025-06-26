# 使用说明：

**想要无脑使用这篇教程必须保证没有重命名任何文件或者文件夹名字！**

并且把压缩包的文件全部放到主目录下(也就是/home/<你的用户名>)

而且你需要提前安装好ROS和相关配套，注意是ROS而不是ROS2

​	如果你没有安装过之前，可以使用一键安装脚本或者自行搜索鱼香ROS安装

​	脚本使用教程：

```bash
sudo chmod 777 create_user.sh
sudo chmod 777 auto_install.sh
./create_user.sh root root
./auto_install.sh
```

​	username和password都是root，输入密码的时候不显示是正常现象，不必慌张

​	你如果装过中文，会有中文提示的，输入索引号回车等就完事了

​	如果你出现乱码说明你没有装过中文，自己打开浏览器输入桌面类型（LXDE或者XFCE或者GNOME或者其他）+终端安装中文这两个关键字随便找一篇靠谱的抄就完事了，这地方写累了，不想赘述。

​	如果不知道桌面类型是什么，sudo apt install neofetch

​	然后neofetch ，观察DE那一行你就知道了

​	然后记得给自己添加到串口用户组然后logout一下，不然你没有权限访问串口，或者使用chmod临时添加一下串口权限

# 安装教程：

## 安装依赖：

```bash
sudo apt-get update
sudo apt-get install libceres-dev libgoogle-glog-dev libgflags-dev libatlas-base-dev libsuitesparse-dev -y 	#ceres依赖
sudo apt-get install liblua5.3-dev -y	#lua依赖
sudo apt-get install stow -y	#abseil安装依赖
sudo apt-get install ninja-build -y	#安装ninja
sudo apt-get install git -y	#安装git
sudo apt-get install libpcap-dev -y	#安装雷达编译依赖
```

**注意**：

**以下依赖需要在安装完成ROS后才能进行安装**

```bash
sudo apt-get install ros-<version_alias>-serial -y #ros串口依赖
#比如20.04ROS1就应该是
sudo apt-get install ros-noetic-serial
```

## 安装abseil:

```bash
cd ~/cartographer_ws/src/cartographer/scripts
sudo ./install_abseil.sh
```

## 安装通信包：

```bash
cd ~/anorosdt_ws
catkin_make
```

**注意：**

飞控和机载电脑通信采用的是串口通信方式，需要现确定好串口号，然后修改
anorosdt_ws/src/anorosdt/config/中的.yaml文件使SerialName_FC: "XXX"这一行正确对应

## 安装雷达功能包：

```bash
cd ~/catkin_ws
catkin_make
```

**注意：**

如果是串口雷达，同样要确定好串口端口号，如果你的雷达是N10P，那么应该/dev/ttyACM0,否则自行通过ls确认，然后在catkin_ws/src/Lslidar_ROS1_driver/lslidar_driver/launch/lslidar_serial.launch中修改

```xml
<param name="serial_port" value="/dev/ttyACM0"/>                                  #雷达连接的串口
```

还有anorosdt_ws/src/anorosdt/launch/Lidar.launch中的：

```xml
<param name="serial_port" value="/dev/ttyACM0"/>                       <!--雷达连接的串口-->
```

安装cartographer算法包：

**注意：**

如果你的运行内存<=4GB，你需要启用swap技术来增大你的运行内存，实测Mem+Swap=8G时可以顺利通过编译

启用swap:

```bash
cd
./enable_swap.sh
```

**注意：**

这个脚本默认会帮你抽取4G存储空间当作内存用，如果你有更大的Swap需要，或者你没有那么多的存储空间，你需要自己编辑这个脚本，调整容量大小，如果你打开脚本发现你不会改，说明你是个弱智。

```bash
cd ~/cartographer_ws
./cartographer.sh
```

**注意：**

这个脚本里面的第二行是关键，后面是一些强制指定路径的CMake修饰，如果你的相关配置和强制指定的路径不同则会造成错误，请自行修改指定路径或者删掉相关配置使用系统默认，这个地方只是ELF2搭配我自己魔改的系统有默认找不到的问题，正常不需要手动强制指定

这个编译时间非常长，不要让电脑休眠，随便干点别的去，只能硬等

## 配置ROS启动相关索引：

```bash
vim ~/.bashrc	#vim的使用教程自己找，如果实在不会用自己去用apt下载一个gedit，把前面的vim换成gedit就行了
```

在最后一行之后添加：

```bash
source /home/<你的用户名>/catkin_ws/devel/setup.bash --extend
source /home/<你的用户名>/anorosdt_ws/devel/setup.bash --extend
source /home/<你的用户名>/cartographer_ws/devel_isolated/setup.bash --extend
```

**注意：**

extend关键字是强制加载并且不覆盖前一个source变量，如果extend变量造成冲突问题，尝试取消修饰或者移动源文件来解决冲突问题

# 快速使用说明：

```bash
roslanch anorosdt cartographer_imu_dt.launch  #启用融合imu数据的建图
```

# 还有一点！

**以上的操作我只在自己魔改好的镜像测试使用过**，魔改的镜像我会放到我的另一个仓库