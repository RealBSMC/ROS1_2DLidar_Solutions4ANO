#include "anoros_dt.h"
#include "Quaternion_Solution.h"

//创建实例
AnoPub anoPub;
AnoSub anoSub;

uint8_t read_buf[2048];

Vel_Pos_Data PosData;
Send_Data SendData;

/*
功能：主函数，ROS初始化，通过AnoRos_DT类创建DT_Control对象并自动调用构造函数初始化
*/
int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "AnoRos_DT"); //ROS初始化，并设置节点名称
    AnoRos_DT anoRos_DT;
		anoRos_DT.Control();//循环执行数据采集和发布话题等操作
    return 0;
}

/*
功能：发布里程计话题
*/
void AnoRos_DT::Publish_Odom()
{

	nav_msgs::Odometry odom;
	odom.header.stamp = ros::Time::now();
	odom.header.frame_id = odom_frame_id;
	odom.pose.pose.position.x = PosData.X;
	odom.pose.pose.position.y = PosData.Y;
	odom.pose.pose.position.z = PosData.Z;

	odom.pose.pose.orientation.w = anoMsgImu.ori[0];
	odom.pose.pose.orientation.x = anoMsgImu.ori[1];
	odom.pose.pose.orientation.y = anoMsgImu.ori[2];
	odom.pose.pose.orientation.z = anoMsgImu.ori[3];

	odom.child_frame_id = robot_frame_id;
	odom.twist.twist.linear.x = anoMsgOF.x;
	odom.twist.twist.linear.y = anoMsgOF.y;
	odom.twist.twist.angular.z = anoMsgOF.z;

	if(anoMsgOF.x == 0 && anoMsgOF.y == 0 && anoMsgOF.z == 0)
	{
		memcpy(&odom.pose.covariance,odom_pose_covariance2,sizeof(odom_pose_covariance2));
		memcpy(&odom.twist.covariance,odom_twist_covariance2,sizeof(odom_twist_covariance2));
	}
	else
	{
		memcpy(&odom.pose.covariance,odom_pose_covariance,sizeof(odom_pose_covariance));
		memcpy(&odom.twist.covariance,odom_twist_covariance,sizeof(odom_twist_covariance));
	}
	odom_publisher.publish(odom);
}

/*
功能：通过串口读取并逐帧校验下位机发送过来的数据
*/
bool AnoRos_DT::Get_Sensor_Data_NOW()
{	
  //串口数据解析：IMU数据、应答返回数据等
  int _cnt=Stm32_Serial.available();
  if(_cnt!=0)
  {
      Stm32_Serial.read(read_buf,_cnt);
      for(int16_t i=0;i<_cnt;i++)
      {
        if(AnoPTv7RecvOneByte(read_buf[i]))
        {
        	return true;
        }
      }
  }
  return false;
}

void AnoRos_DT::Control()
{
	ros::Rate loop_rate(100); // 设置循环频率  
	while(ros::ok)
	{
		if (true == Get_Sensor_Data_NOW())//串口获取数据
		{
			anoPub.pubMsgImu();
			anoPub.pubMsgGps();
			anoPub.pubMsgMag();
			anoPub.pubMsgOF();
			anoPub.pubMsgTmp();
			anoPub.pubMsgAlt();
			anoPub.pubMsgRC();
			anoPub.pubMsgUserData();

			//发送数据到下位机
			AnoDTRaspRunTask1Ms();
		}
	ros::spinOnce();//监听反馈函数
  loop_rate.sleep();//执行循环
	}
	//ros::spin();//循环等待回调函数
}
/*
功能：构造函数，只执行一次，用于初始化
*/
AnoRos_DT::AnoRos_DT():Sampling_Time(0)
{
	//清除数据
	memset(&anoMsgImu, 		0, sizeof(anoMsgImu));
	memset(&anoMsgMag, 		0, sizeof(anoMsgMag));
	memset(&anoMsgImuSpd, 0, sizeof(anoMsgImuSpd));
	memset(&anoMsgOF,			0, sizeof(anoMsgOF));
	memset(&anoMsgGPS,		0, sizeof(anoMsgGPS));

	cmdVel.sendFalg = 0;

	anoPub.readSettings_Pub();
	anoPub.anoPubCreate_Init();
	anoSub.readSettings_Sub();
	anoSub.anoSubCreate_Init();

	//串口初始化
	Serial_Init();
}

/*
功能：析构函数，只执行一次，当对象结束其生命周期时系统会调用这个函数
*/
AnoRos_DT::~AnoRos_DT()
{
	Stm32_Serial.close();//关闭串口
}
