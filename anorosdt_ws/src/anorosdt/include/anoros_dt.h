#ifndef _ANOROS_DT_H_
#define _ANOROS_DT_H_

#include <ros/ros.h>
#include <iostream>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Int16.h>
#include <string.h>
#include <string>
#include <math.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf2_msgs/TFMessage.h>  
#include <tf2/LinearMath/Transform.h>  
#include <tf2_ros/transform_listener.h>  
#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/MagneticField.h>
#include <sensor_msgs/Temperature.h>
#include <sensor_msgs/Range.h>


#include <anorosdt/RemoteControl.h>
#include <anorosdt/AnoRosDT_User.h>
#include <anorosdt/AnoRosDT_KZFC.h>

#include <AnoPTv7.h>
#include <anoSerial.h>
#include <anoPublisher.h>
#include <anoSubscriber.h>




//协方差矩阵，用于里程计话题数据，用于robt_pose_ekf功能包
const double odom_pose_covariance[36]   = {1e-3,    0,    0,   0,   0,    0, 
										      0, 1e-3,    0,   0,   0,    0,
										      0,    0,  1e6,   0,   0,    0,
										      0,    0,    0, 1e6,   0,    0,
										      0,    0,    0,   0, 1e6,    0,
										      0,    0,    0,   0,   0,  1e3 };

const double odom_pose_covariance2[36]  = {1e-9,    0,    0,   0,   0,    0, 
										      0, 1e-3, 1e-9,   0,   0,    0,
										      0,    0,  1e6,   0,   0,    0,
										      0,    0,    0, 1e6,   0,    0,
										      0,    0,    0,   0, 1e6,    0,
										      0,    0,    0,   0,   0, 1e-9 };

const double odom_twist_covariance[36]  = {1e-3,    0,    0,   0,   0,    0, 
										      0, 1e-3,    0,   0,   0,    0,
										      0,    0,  1e6,   0,   0,    0,
										      0,    0,    0, 1e6,   0,    0,
										      0,    0,    0,   0, 1e6,    0,
										      0,    0,    0,   0,   0,  1e3 };
										      
const double odom_twist_covariance2[36] = {1e-9,    0,    0,   0,   0,    0, 
										      0, 1e-3, 1e-9,   0,   0,    0,
										      0,    0,  1e6,   0,   0,    0,
										      0,    0,    0, 1e6,   0,    0,
										      0,    0,    0,   0, 1e6,    0,
										      0,    0,    0,   0,   0, 1e-9} ;


//速度位置数据结构体
typedef struct Vel_Pos_Data_st
{
    double X;
    double Y;
    double Z;
}Vel_Pos_Data;

typedef struct SendData_ST
{
    float t265_acc[3]; //t265加速度
    float t265_loc[3]; //位置
    float vel[3];//速度
    float loc[3];//建图位置
}Send_Data;


//串口接收数据结构体
extern Vel_Pos_Data PosData;//位置数据

extern Send_Data SendData;


class AnoRos_DT
{
    public:
        AnoRos_DT();//构造函数
        ~AnoRos_DT();//析构函数
        void Control();
    private: /* data */
        
        ros::Time _Now,_Last_Time;
        float Sampling_Time;
        
        ros::Publisher odom_publisher,userData_publisher; //初始化话题发布者 
        void Publish_Odom();//发布里程计话题
                
        std::string serial_port, robot_frame_id, odom_frame_id;//定义相关变量
	    int baud_rate;

        //从串口获取数据
        bool Get_Sensor_Data_NOW();
};


#endif
