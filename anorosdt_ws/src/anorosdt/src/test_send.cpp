#include <ros/ros.h>
#include <std_msgs/String.h>
#include "anorosdt/AnoRosDT_KZFC.h"



int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "test_send"); //ROS初始化，并设置节点名称

    ros::NodeHandle n;

    ros::Publisher test_send_Publication ;

    test_send_Publication = n.advertise<anorosdt::AnoRosDT_KZFC>("sendKZFC_data",10);

    ros::Rate loop_rate(10); // 设置循环频率  
    while(ros::ok())
    {

        anorosdt::AnoRosDT_KZFC KZFC; //发布话题消息赋值
        KZFC.ACC_X = 10;
        KZFC.ACC_Y = 10;
        KZFC.ACC_Z = 0;
        KZFC.YAW = -10;

	    test_send_Publication.publish(KZFC);
        
        ros::spinOnce();//监听反馈函数
        loop_rate.sleep();//执行循环
    }
    //ros::spin();

    return 0;
}
