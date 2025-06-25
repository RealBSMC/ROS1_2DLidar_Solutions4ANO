#include <ros/ros.h>
#include <std_msgs/String.h>
#include <anorosdt/AnoRosDT_User.h>

//回调函数
void doData(const anorosdt::AnoRosDT_User data)
{
    //测试下位机发送过来的数据是否收到
    int8_t s81 = 0,s82 = 0,s83 = 0;
    int16_t s161 = 0,s162 = 0,s163 = 0;

    s81 = data.s81;
    s82 = data.s82;
    s83 = data.s83;

    s161 = data.s161;
    s162 = data.s162;
    s163 = data.s163;

    ROS_INFO("s81 = %d , s82 = %d ,s83 = %d",s81,s82,s83);
    ROS_INFO("s161 = %d , s162 = %d ,s163 = %d",s161,s162,s163);
}

int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "UserData_test1"); //ROS初始化，并设置节点名称

    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/anoros_dt/UserData",20,doData);
    ros::spin();

    return 0;
}
