#include "anoPublisher.h"

sMsgImu anoMsgImu;
sMsgMag anoMsgMag;
sMsgSpd anoMsgImuSpd;
sMsgOF anoMsgOF;
sMsgGPS anoMsgGPS;
sMsgAlt anoMsgAlt;
sMsgRC anoMsgRC;
User_Data UserData;//用户数据

/*
初始化发布节点的名称
*/
void AnoPub::readSettings_Pub(void) 
{
	ros::NodeHandle PubSet_n("~"); //创建节点句柄
    try
    {
    	//发布话题名称
        PubSet_n.getParam("PubTopicName_Imu",       this->m_setTopic.pubTopicNameImu);//IMU
        PubSet_n.getParam("PubTopicName_Mag",       this->m_setTopic.pubTopicNameMag);//罗盘
        PubSet_n.getParam("PubTopicName_Tmp",       this->m_setTopic.pubTopicNameTmp);//温度
        PubSet_n.getParam("PubTopicName_Alt",       this->m_setTopic.pubTopicNameAlt);//高度
        PubSet_n.getParam("PubTopicName_Gps",       this->m_setTopic.pubTopicNameGPS);//gps
        PubSet_n.getParam("PubTopicName_OF",        this->m_setTopic.pubTopicNameOF);//光流速度
        PubSet_n.getParam("PubTopicName_RC",        this->m_setTopic.pubTopicNameRC);//遥控
        PubSet_n.getParam("PubTopicName_UserData",  this->m_setTopic.pubTopicNameUser);//用户数据

    	//发布坐标系名称
        PubSet_n.getParam("PubFrameidName_Imu",     this->m_setFrameid.pubFrameidNameImu);
        PubSet_n.getParam("PubFrameidName_Mag",     this->m_setFrameid.pubFrameidNameMag);
        PubSet_n.getParam("PubFrameidName_Alt",     this->m_setFrameid.pubFrameidNameAlt);
        PubSet_n.getParam("PubFrameidName_Gps",     this->m_setFrameid.pubFrameidNameGPS);
        PubSet_n.getParam("PubFrameidName_OF",      this->m_setFrameid.pubFrameidNameOF);
        PubSet_n.getParam("PubFrameidName_RC",      this->m_setFrameid.pubFrameidNameRC);


        ROS_INFO( "settings read ok");
    }
    catch (serial::IOException& e)
	{
	     ROS_INFO("settings read no");
	}   
}

//创建话题发布
void AnoPub::anoPubCreate_Init(void)
{
	ros::NodeHandle PubCre_n("~"); //创建似有节点句柄

	m_pubImu = PubCre_n.advertise<sensor_msgs::Imu>					(this->m_setTopic.pubTopicNameImu, 	200);//创建IMU话题发布者
	m_pubMag = PubCre_n.advertise<sensor_msgs::MagneticField>		(this->m_setTopic.pubTopicNameMag, 	100);//创建罗盘话题发布者
	m_pubGps = PubCre_n.advertise<sensor_msgs::NavSatFix>			(this->m_setTopic.pubTopicNameGPS, 	50);//创建GPS话题发布者
	m_pubVel = PubCre_n.advertise<geometry_msgs::TwistStamped>		(this->m_setTopic.pubTopicNameOF, 	100);//创建速度话题发布者
    m_pubTmp = PubCre_n.advertise<sensor_msgs::Temperature>         (this->m_setTopic.pubTopicNameTmp,  20);//创建温度话题发布者
    m_pubAlt = PubCre_n.advertise<sensor_msgs::Range>               (this->m_setTopic.pubTopicNameAlt,  50);//创建高度话题发布者
    m_pubRC  = PubCre_n.advertise<anorosdt::RemoteControl>          (this->m_setTopic.pubTopicNameRC,   50);//创建遥控话题发布者
    m_pubUser = PubCre_n.advertise<anorosdt::AnoRosDT_User>         (this->m_setTopic.pubTopicNameUser, 20);//创建遥控话题发布者
}

//发布IMU数据
void AnoPub::pubMsgImu(void)
{
	sensor_msgs::Imu message;

    message.header.stamp = ros::Time::now();
    message.header.frame_id = this->m_setFrameid.pubFrameidNameImu;

    // Fill in IMU data
    message.orientation.w = anoMsgImu.ori[0];
    message.orientation.x = anoMsgImu.ori[1];
    message.orientation.y = anoMsgImu.ori[2];
    message.orientation.z = anoMsgImu.ori[3];

    message.angular_velocity.x = anoMsgImu.gyr[0];  // Radians/sec
    message.angular_velocity.y = anoMsgImu.gyr[1];  // Radians/sec
    message.angular_velocity.z = anoMsgImu.gyr[2];  // Radians/sec

    message.linear_acceleration.x = anoMsgImu.acc[0];  // m/s^2
    message.linear_acceleration.y = anoMsgImu.acc[1];  // m/s^2
    message.linear_acceleration.z = anoMsgImu.acc[2];  // m/s^2

    m_pubImu.publish(message);
}

//发布gps数据
void AnoPub::pubMsgGps(void)
{
	sensor_msgs::NavSatFix gps_msg;  

  	gps_msg.header.stamp = ros::Time::now();  
 	gps_msg.header.frame_id = this->m_setFrameid.pubFrameidNameGPS;  

  	gps_msg.latitude 	= anoMsgGPS.Gps_LAT;  
  	gps_msg.longitude 	= anoMsgGPS.Gps_LNG;  
  	gps_msg.altitude 	= anoMsgGPS.Gps_ALT;  
  
  	m_pubGps.publish(gps_msg);  
}

//发布罗盘数据
void AnoPub::pubMsgMag(void) 
{
    sensor_msgs::MagneticField message;

    message.header.stamp = ros::Time::now();  
    message.header.frame_id = this->m_setFrameid.pubFrameidNameMag;
	
    message.magnetic_field.x = anoMsgMag.x;
    message.magnetic_field.y = anoMsgMag.y;
    message.magnetic_field.z = anoMsgMag.z;

    m_pubMag.publish(message);
}

//发布温度数据
void AnoPub::pubMsgTmp(void) 
{
    sensor_msgs::Temperature message;
    message.header.stamp = ros::Time::now(); 
    message.header.frame_id = this->m_setFrameid.pubFrameidNameTmp;

    message.temperature = anoMsgMag.Tmp;

    m_pubTmp.publish(message);
}

//发布速度数据
void AnoPub::pubMsgOF(void) 
{
    geometry_msgs::TwistStamped message;

    message.header.stamp = ros::Time::now();  
    message.header.frame_id = this->m_setFrameid.pubFrameidNameOF;

    message.twist.linear.x = anoMsgOF.x;
    message.twist.linear.y = anoMsgOF.y;
    message.twist.linear.z = anoMsgOF.z;

    m_pubVel.publish(message);
}

//发布高度数据
void AnoPub::pubMsgAlt(void) 
{
    sensor_msgs::Range message;
    message.header.stamp = ros::Time::now();  
    message.header.frame_id = this->m_setFrameid.pubFrameidNameAlt;
    message.radiation_type = 0;
    message.min_range = 0;
    message.max_range = 99999;
    message.range = anoMsgAlt.Alt;
    m_pubAlt.publish(message);

}

//发布遥控数据
void AnoPub::pubMsgRC(void)
{
    anorosdt::RemoteControl message;
    message.header.stamp = ros::Time::now();  
    message.header.frame_id = this->m_setFrameid.pubFrameidNameRC;
    message.ch1_rol = anoMsgRC.ch1_rol;
    message.ch2_pit = anoMsgRC.ch2_pit;
    message.ch3_thr = anoMsgRC.ch3_thr;
    message.ch4_yaw = anoMsgRC.ch4_yaw;
    message.ch5_aux1 = anoMsgRC.ch5_aux1;
    message.ch6_aux2 = anoMsgRC.ch6_aux2;
    message.ch7_aux3 = anoMsgRC.ch7_aux3;
    message.ch8_aux4 = anoMsgRC.ch8_aux4;
    message.ch9_aux5 = anoMsgRC.ch9_aux5;
    message.ch10_aux6 = anoMsgRC.ch10_aux6;
    m_pubRC.publish(message);

}


/*
功能：发布用户数据话题
*/
void AnoPub::pubMsgUserData(void)
{
    anorosdt::AnoRosDT_User userData; //发布话题消息赋值
    userData.s81 = UserData.s81;
    userData.s82 = UserData.s82;
    userData.s83 = UserData.s83;
    userData.s161 = UserData.s161;
    userData.s162 = UserData.s162;
    userData.s163 = UserData.s163;

    m_pubUser.publish(userData);
}



