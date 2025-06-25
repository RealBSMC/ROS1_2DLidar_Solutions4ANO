#include "anoSubscriber.h"

sMsgCmdVel cmdVel;

/*
初始化发布节点的名称
*/
void AnoSub::readSettings_Sub(void) 
{
	ros::NodeHandle SubSet_n("~"); //创建节点句柄
    try
    {
    	//发布话题名称
    	SubSet_n.param<std::string>	("subTopicNamePos_T265",	this->m_setTopic.subTopicNamePos_T265,	"/camera/odom/sample");//T265数据
    	SubSet_n.param<std::string>	("subTopicNamePos_Radar",	this->m_setTopic.subTopicNamePos_Radar,	"/tf");//激光雷达定位数据
    	SubSet_n.param<std::string>	("subTopicNameUserData",	this->m_setTopic.subTopicNameUserData,	"/sendKZFC_data");//用户数据

        ROS_INFO( "settings read ok");
    }
    catch (serial::IOException& e)
	{
	     ROS_INFO("settings read no");
	}   
}


void AnoSub::anoSubCreate_Init(void)
{
  ros::NodeHandle sub_n; //创建似有节点句柄

	m_subT265 	= sub_n.subscribe(this->m_setTopic.subTopicNamePos_T265,	 100,	&AnoSub::SubT265Data,this);//获取t265数据
	m_subRadar  = sub_n.subscribe(this->m_setTopic.subTopicNamePos_Radar,	 100,	&AnoSub::SubRadarData,this);//获取雷达建图定位数据
	m_subUserData = sub_n.subscribe(this->m_setTopic.subTopicNameUserData, 50,	&AnoSub::SubUserData,this);//测试代码，主要测试订阅话题并发送数据到下位机
}


/*
功能：通过串口发送数据到下位机，测试代码
*/
void AnoSub::SubUserData(const anorosdt::AnoRosDT_KZFC data)
{
  uint8_t _buf[10];
  uint8_t _cnt = 0;
  int16_t _val = 0;

  _val = data.ACC_X;
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;

  _val = data.ACC_Y;
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;

  _val = data.ACC_Z;
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;

  _val = data.YAW;
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;

  AnoPTv7SendBuf(0x31,_buf,sizeof(_buf));
}

/*
功能：t265数据获取
*/
void AnoSub::SubT265Data(const nav_msgs::Odometry::ConstPtr& msg)
{
  uint8_t _buf[14];
  uint8_t _cnt = 0;
  int16_t _val = 0;

  //位置信息
  _val = msg->pose.pose.position.x*100;
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;
  
  _val = msg->pose.pose.position.y*100;
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;

  _val = msg->pose.pose.position.z*100;
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;

  //速度信息
  _val = msg->twist.twist.linear.x*100;
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;

  _val = msg->twist.twist.linear.y*100;
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;

  _val = msg->twist.twist.linear.z*100;
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;

  AnoPTv7SendBuf(0x32,_buf,sizeof(_buf));
}

/*
功能：激光雷达建图定位数据获取
*/
void AnoSub::SubRadarData(const tf2_msgs::TFMessage::ConstPtr& msg)
{
  uint8_t _buf[8];
  uint8_t _cnt = 0;
  int16_t _val = 0;

  int16_t _val_1[3] = {0};
  int16_t _val_2[3] = {0};

  for (const auto& transform_stamped : msg->transforms)  
  { 
      if (transform_stamped.header.frame_id == "map" &&
      transform_stamped.child_frame_id == "odom" )
   {
      // 提取平移信息  
      const geometry_msgs::Vector3& translation = transform_stamped.transform.translation;  
      _val_1[0] = translation.x * 100;  
      _val_1[1] = translation.y * 100;  
      _val_1[2] = translation.z * 100;  
    } 
	  if (transform_stamped.header.frame_id == "odom" &&
		  transform_stamped.child_frame_id == "laser_frame" )
	 {
			// 提取平移信息  
			const geometry_msgs::Vector3& translation = transform_stamped.transform.translation;  
      _val_2[0] = translation.x * 100;  
      _val_2[1] = translation.y * 100;  
      _val_2[2] = translation.z * 100;  
	  } 
  }  
    //位置信息
  _val = _val_1[0] + _val_2[0];
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;
  
  _val = _val_1[1] + _val_2[1];
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;

  _val = _val_1[2] + _val_2[2];
  memcpy(_buf + _cnt, (uint8_t*)(&_val), 2);
  _cnt += 2;
  AnoPTv7SendBuf(0x33,_buf,sizeof(_buf));
}




