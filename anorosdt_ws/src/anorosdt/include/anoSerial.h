#ifndef _ANOSERIAL_H_
#define _ANOSERIAL_H_

#include <AnoPTv7.h>
#include <anoros_dt.h>


struct sMsgImu 
{
    float acc[3];
    float gyr[3];
    float ori[4];
    bool flagMemsReady;
    bool flagOriReady;
};
struct sMsgMag 
{
    float x;
    float y;
    float z;
    float Tmp;
};
struct sMsgSpd 
{
    float x;
    float y;
    float z;
};
struct sMsgOF 
{
    float x;
    float y;
    float z;
};

struct sMsgGPS 
{
    float Gps_LNG;
    float Gps_LAT;
    float Gps_ALT;
};

struct sMsgAlt
{
    float Alt;

};

struct sMsgRC
{
    uint16_t ch1_rol;
    uint16_t ch2_pit;
    uint16_t ch3_thr;
    uint16_t ch4_yaw;
    uint16_t ch5_aux1;
    uint16_t ch6_aux2;
    uint16_t ch7_aux3;
    uint16_t ch8_aux4;
    uint16_t ch9_aux5;
    uint16_t ch10_aux6;
};

//用户数据结构体
struct User_Data
{
    int8_t s81;
    int8_t s82;
    int8_t s83;
    int16_t s161;
    int16_t s162;
    int16_t s163;
};


extern sMsgImu anoMsgImu;
extern sMsgMag anoMsgMag;
extern sMsgSpd anoMsgImuSpd;
extern sMsgOF  anoMsgOF;
extern sMsgGPS anoMsgGPS;
extern sMsgAlt anoMsgAlt;
extern sMsgRC  anoMsgRC;
extern User_Data UserData;//用户数据


extern serial::Serial Stm32_Serial; //声明串口对象

void Serial_Init(void);

void AnoDTRaspRunTask1Ms(void);

bool AnoPTv7RecvOneByte(uint8_t data);
bool anoRosDTRaspDataFrameAnl(uint8_t *data);

//通用发送函数
void AnoPTv7SendBuf(const uint8_t ID,const uint8_t* buf, const uint8_t len);

#endif




