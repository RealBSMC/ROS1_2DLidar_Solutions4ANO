#ifndef _ANOSUBSCRIBER_H_
#define _ANOSUBSCRIBER_H_

#include <AnoPTv7.h>
#include <anoros_dt.h>


class AnoSub
{
	public:
		struct sSetTopic
		{
			std::string subTopicNameCmdVel;
        	std::string subTopicNamePos_T265;
        	std::string subTopicNamePos_Radar;
        	std::string subTopicNameUserData;
		};
	public:
		sSetTopic m_setTopic;

		ros::Subscriber m_subUserData, m_subT265,m_subRadar;//初始化订阅者话题

		void readSettings_Sub(void); 
		void anoSubCreate_Init(void);

		void SubUserData(const anorosdt::AnoRosDT_KZFC data);
        void SubT265Data(const nav_msgs::Odometry::ConstPtr& msg);
        void SubRadarData(const tf2_msgs::TFMessage::ConstPtr& msg);
};
struct sMsgCmdVel
{
    int16_t acc[3];
    int16_t yaw;
    uint8_t sendFalg;
};

extern sMsgCmdVel cmdVel;


#endif

