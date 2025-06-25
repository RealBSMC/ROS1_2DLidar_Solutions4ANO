#ifndef _ANOPUBLISHER_H_
#define _ANOPUBLISHER_H_

#include <AnoPTv7.h>
#include <anoros_dt.h>

class AnoPub
{
	public:
		struct sSetTopic
		{
	        std::string pubTopicNameImu;//imu
	        std::string pubTopicNameMag;
	        std::string pubTopicNameTmp;
	        std::string pubTopicNameAlt;
	        std::string pubTopicNameOF;
	        std::string pubTopicNameGPS;
	        std::string pubTopicNameRC;
	        std::string pubTopicNameUser;

	        std::string pubTopicNameCmdVel;
		};
		struct sSetFrameid
		{
			std::string pubFrameidNameImu;//imu
			std::string pubFrameidNameOF;//of
			std::string pubFrameidNameGPS;//gps
			std::string pubFrameidNameMag;//gps
			std::string pubFrameidNameTmp;//tmp
			std::string pubFrameidNameAlt;//alt
			std::string pubFrameidNameRC;//RC
		};

	public:
		
		sSetTopic	 	m_setTopic;
		sSetFrameid		m_setFrameid;
		
		ros::Publisher m_pubImu,m_pubMag,m_pubAlt,m_pubGps,m_pubOdom,m_pubVel,m_pubTmp,m_pubRC,m_pubUser;

		void readSettings_Pub(void);
		void anoPubCreate_Init(void);
		void pubMsgImu(void);
		void pubMsgGps(void);
		void pubMsgMag(void); 
		void pubMsgTmp(void); 
		void pubMsgOF(void); 
		void pubMsgAlt(void);
		void pubMsgRC(void); 
		void pubMsgUserData(void);
};

#endif






