#ifndef _ANOPTV7_H_
#define _ANOPTV7_H_

#include <stdint.h>
#include <string.h>


#define BYTE0(dwTemp) (*((char *)(&dwTemp)))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))

#define M_PI       3.14159265358979323846   // pi


//通信方式定义
#define LinkType_ToRasp		0x01
#define LinkType_ToIMU		0x02
//发送数据帧时临时存储数据帧信息的结构体
typedef struct
{
	uint8_t _isUsed;
	uint8_t _linkType;			
}__attribute__ ((__packed__)) myTPTv7TxStruct;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t ID;
	uint8_t dataLen;
	uint8_t dataBuf[255];
	uint8_t sc1;
	uint8_t sc2;
}__attribute__ ((__packed__)) myTAnoPTv7Struct;	 

typedef union 
{
	myTAnoPTv7Struct frame;
	uint8_t rawBytes[sizeof(myTAnoPTv7Struct)];
}__attribute__ ((__packed__)) myTAnoPTv7Frame;

//用于发送的结构体，data保存帧内容，cmd保存发送参数
typedef struct
{
	myTAnoPTv7Frame data;		
	myTPTv7TxStruct cmd; 			
}__attribute__ ((__packed__)) myTPTv7TxFrame;








#endif










