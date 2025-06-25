#include "anoSerial.h"

serial::Serial Stm32_Serial; //声明串口对象 

static void anoPTv7AddTxFrame(uint8_t linkType, myTAnoPTv7Frame *pFrame);
static void anoDTRaspSendCheck(void);

//接受数据
static myTAnoPTv7Frame rxFrame;

//发送数据缓冲
#define ANOPTV8TXBUFNUM 100
static myTPTv7TxFrame txFrameBuf[ANOPTV8TXBUFNUM];
static uint8_t txBufWAddr = 0;
static uint8_t txBufRAddr = 0;

void Serial_Init(void)
{
  std::string SerialName_FC;//串口名称
  int SerialBaud_FC;//波特率

  ros::NodeHandle PubSet_n("~"); //创建节点句柄
  //获取yaml中的参数
  PubSet_n.getParam("SerialName_FC",SerialName_FC);//串口名称
  PubSet_n.getParam("SerialBaud_FC",SerialBaud_FC);//通讯波特率

  try
  {
    Stm32_Serial.setPort(SerialName_FC);//初始化串口名
    Stm32_Serial.setBaudrate(SerialBaud_FC);//初始化波特率
    serial::Timeout _time = serial::Timeout::simpleTimeout(1000);
    Stm32_Serial.setTimeout(_time);
    Stm32_Serial.open();//打开串口
    Stm32_Serial.flushInput(); //清空缓冲区数据
  }
  catch (serial::IOException& e)
  {
      ROS_INFO("Unable to open port ");
  }
  if(Stm32_Serial.isOpen())//判断串口是否打开
  {
      Stm32_Serial.flushInput(); //清空缓冲区数据
      ROS_INFO("Serial Port opened");
  }
}

//检测是否有数据发送
void AnoDTRaspRunTask1Ms(void)
{
  anoDTRaspSendCheck();
}

//接受数据并解析
bool AnoPTv7RecvOneByte(uint8_t data)
{
	static uint16_t _recv_cnt = 0;
  static uint16_t _data_cnt = 0;
  static uint8_t rxstate = 0;

  //判断帧头是否满足匿名协议的0xAA
  if (rxstate == 0 && data == 0xAA)
  {
      _recv_cnt = 0;
      rxFrame.rawBytes[_recv_cnt++] = data;
      rxstate ++;
  }
  //数据地址字节
  else if (rxstate == 1)
  {
      rxFrame.rawBytes[_recv_cnt++] = data;
      rxstate ++;
  }
  //帧ID
  else if (rxstate == 2)
  {
      rxFrame.rawBytes[_recv_cnt++] = data;
      rxstate ++;
  }
  //接收数据长度
  else if (rxstate == 3)
  {
      rxFrame.rawBytes[_recv_cnt++] = data;
      rxFrame.frame.dataLen = data;
      _data_cnt = 0;
      rxstate ++;
  }
  //接收数据
  else if (rxstate == 4)
  {
      rxFrame.rawBytes[_recv_cnt++] = data;
      _data_cnt++;
      if(_data_cnt >= rxFrame.frame.dataLen)
      {
          rxstate ++;
      }
  }
  //接收SC1
  else  if (rxstate == 5)
  {
      rxFrame.frame.sc1 = data;
      rxstate ++;
  }
  //接收SC2
  else  if (rxstate == 6)
  {
      rxFrame.frame.sc2 = data;
      rxstate = 0;
      //所有数据接收完毕，进行解析
      return anoRosDTRaspDataFrameAnl(rxFrame.rawBytes);
  }
  else
  {
      rxstate = 0;
      return false;
  }
	 return false;
}

/*
功能：解析ROS端发送的数据
*/
bool anoRosDTRaspDataFrameAnl(uint8_t *data)
{
 uint8_t check_sum1 = 0, check_sum2 = 0;
  //根据收到的数据计算校验字节1和2
  for (uint16_t i = 0; i < (rxFrame.frame.dataLen + 4); i++)
  {
      check_sum1 += rxFrame.rawBytes[i];
      check_sum2 += check_sum1;
  }
  //计算出的校验字节和收到的校验字节做对比，完全一致代表本帧数据合法，不一致则跳出解析函数
  if ((check_sum1 != rxFrame.frame.sc1) || (check_sum2 != rxFrame.frame.sc2)) //判断sum校验
      return false;

  /*******************************************/
    switch(rxFrame.frame.ID)
    {
    case 0x01:
    	{
			 anoMsgImu.acc[0] = (float)*((int16_t*)(rxFrame.frame.dataBuf+0)) / 100;
			 anoMsgImu.acc[1] = (float)*((int16_t*)(rxFrame.frame.dataBuf+2)) / 100;
			 anoMsgImu.acc[2] = (float)*((int16_t*)(rxFrame.frame.dataBuf+4)) / 100;

			 anoMsgImu.gyr[0] = (float)*((int16_t*)(rxFrame.frame.dataBuf+6)) / 16.384 * M_PI / 180; //获取IMU的X轴角速度  
			 anoMsgImu.gyr[1] = (float)*((int16_t*)(rxFrame.frame.dataBuf+8)) / 16.384 * M_PI / 180; //获取IMU的Y轴角速度  
			 anoMsgImu.gyr[2] = (float)*((int16_t*)(rxFrame.frame.dataBuf+10)) / 16.384 * M_PI / 180; //获取IMU的Z轴角速度  

			 anoMsgImu.ori[0] = (float)*((int16_t*)(rxFrame.frame.dataBuf+12)) / 10000;//获取IMU解析出来的四元素
			 anoMsgImu.ori[1] = (float)*((int16_t*)(rxFrame.frame.dataBuf+14)) / 10000;
			 anoMsgImu.ori[2] = (float)*((int16_t*)(rxFrame.frame.dataBuf+16)) / 10000;
			 anoMsgImu.ori[3] = (float)*((int16_t*)(rxFrame.frame.dataBuf+18)) / 10000;
			 return true;
    	}break;
    case 0x02:
      {
        anoMsgMag.x   = (float)*((int16_t*)(rxFrame.frame.dataBuf+0));
        anoMsgMag.y   = (float)*((int16_t*)(rxFrame.frame.dataBuf+2));
        anoMsgMag.z   = (float)*((int16_t*)(rxFrame.frame.dataBuf+4));
        anoMsgMag.Tmp = (float)*((int16_t*)(rxFrame.frame.dataBuf+6)) / 100;

        return true;
      }break;
    case 0x05:
      {

        anoMsgAlt.Alt = (float)*((int16_t*)(rxFrame.frame.dataBuf+0)) / 100;
        return true;
      }break;
    case 0x07:
    	{
    		anoMsgOF.x = (float)*((int16_t*)(rxFrame.frame.dataBuf+0)) / 100;//获取运动速度 转换位m单位
			  anoMsgOF.y = (float)*((int16_t*)(rxFrame.frame.dataBuf+2)) / 100;
			  anoMsgOF.z = (float)*((int16_t*)(rxFrame.frame.dataBuf+4)) / 100;
			  return true;
    	}break;
    case 0x30:
    	{
    		anoMsgGPS.Gps_LNG = (float)*((int32_t*)(rxFrame.frame.dataBuf+0)) / 10000000;
			  anoMsgGPS.Gps_LAT = (float)*((int32_t*)(rxFrame.frame.dataBuf+4)) / 10000000;
			  anoMsgGPS.Gps_ALT = (float)*((int32_t*)(rxFrame.frame.dataBuf+8)) / 10000000; //10000000
			  return true;
    	}break;
    case 0x40:
      {
        anoMsgRC.ch1_rol = (uint16_t)*((int16_t*)(rxFrame.frame.dataBuf+0));
        anoMsgRC.ch2_pit = (uint16_t)*((int16_t*)(rxFrame.frame.dataBuf+2));
        anoMsgRC.ch3_thr = (uint16_t)*((int16_t*)(rxFrame.frame.dataBuf+4));
        anoMsgRC.ch4_yaw = (uint16_t)*((int16_t*)(rxFrame.frame.dataBuf+6));
        anoMsgRC.ch5_aux1 = (uint16_t)*((int16_t*)(rxFrame.frame.dataBuf+8));
        anoMsgRC.ch6_aux2 = (uint16_t)*((int16_t*)(rxFrame.frame.dataBuf+10));
        anoMsgRC.ch7_aux3 = (uint16_t)*((int16_t*)(rxFrame.frame.dataBuf+12));
        anoMsgRC.ch8_aux4 = (uint16_t)*((int16_t*)(rxFrame.frame.dataBuf+14));
        anoMsgRC.ch9_aux5 = (uint16_t)*((int16_t*)(rxFrame.frame.dataBuf+16));
        anoMsgRC.ch10_aux6 = (uint16_t)*((int16_t*)(rxFrame.frame.dataBuf+18));
        return true;
      }break;
    case 0xF1:
    	{
    		UserData.s81 = (uint8_t)*((uint8_t*)(rxFrame.frame.dataBuf+0));//用户数据
			  UserData.s82 = (uint8_t)*((uint8_t*)(rxFrame.frame.dataBuf+1));
			  UserData.s83 = (uint8_t)*((uint8_t*)(rxFrame.frame.dataBuf+2));
			  UserData.s161 = (int16_t)*((int16_t*)(rxFrame.frame.dataBuf+3));
			  UserData.s162 = (int16_t)*((int16_t*)(rxFrame.frame.dataBuf+5));
			  UserData.s163 = (int16_t)*((int16_t*)(rxFrame.frame.dataBuf+7));
			  return true;
    	}break;
    default:
    	{
    		return false;
    	}break;
    }
}


//串口发送数据
void AnoPTv7DrvSend(uint8_t linktype, uint8_t *buf, uint16_t len)
{
  Stm32_Serial.write(buf,len);
}
//
//将需要发送的帧写入缓冲区
static void anoPTv7AddTxFrame(uint8_t linkType, myTAnoPTv7Frame *pFrame)
{
  if(txFrameBuf[txBufWAddr].cmd._isUsed)
    return;
  txFrameBuf[txBufWAddr].cmd._isUsed = 1;
  txFrameBuf[txBufWAddr].cmd._linkType = linkType;
  for(uint16_t i = 0; i < (pFrame->frame.dataLen + 6); i++)
    txFrameBuf[txBufWAddr].data.rawBytes[i] = pFrame->rawBytes[i];
  
  txFrameBuf[txBufWAddr].cmd._isUsed = 2;
  
  txBufWAddr++;
  if(txBufWAddr >= ANOPTV8TXBUFNUM)
    txBufWAddr = 0;
}
//
static void anoDTRaspSendCheck(void)
{
  //如果发送缓冲区有需要发送的数据，执行发送
  for(uint8_t i=0; i<ANOPTV8TXBUFNUM; i++)
  {
    if(txFrameBuf[txBufRAddr].cmd._isUsed == 2)
    {
      //数据写入完毕，调用发送
      AnoPTv7DrvSend(txFrameBuf[txBufRAddr].cmd._linkType, txFrameBuf[txBufRAddr].data.rawBytes, txFrameBuf[txBufRAddr].data.frame.dataLen+6);
      txFrameBuf[txBufRAddr].cmd._isUsed = 0;
    }
    txBufRAddr++;
    if(txBufRAddr >= ANOPTV8TXBUFNUM)
    txBufRAddr = 0;
  }
}

void AnoPTv7SendBuf(const uint8_t ID,const uint8_t* buf, const uint8_t len)
{
  
  myTAnoPTv7Frame txFrame;
  uint8_t _cnt = 0;

  txFrame.frame.head = 0xAA;
  txFrame.frame.addr = 0xFF;
  txFrame.frame.ID = ID;
  for(uint8_t i = 0; i < len; i++)
  {
    txFrame.frame.dataBuf[_cnt++] = *(buf + i);
  }
  txFrame.frame.dataLen = _cnt;
  uint8_t sc1 = 0;
  uint8_t sc2 = 0;
  for(uint16_t i=0; i<(txFrame.frame.dataLen+4); i++)
  {
    sc1 += txFrame.rawBytes[i];
    sc2 += sc1;
  }
  txFrame.rawBytes[txFrame.frame.dataLen+4] = sc1;
  txFrame.rawBytes[txFrame.frame.dataLen+5] = sc2;

  anoPTv7AddTxFrame(LinkType_ToRasp, &txFrame);
}

