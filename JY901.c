#include "userDefine.h"

float roll=0,pitch=0,yaw=0;																							//分别对应YXZ轴的角度
float accX=0,accY=0,accZ=0;																				//加速度XYZ分量
float gyrX=0,gyrY=0,gyrZ=0;																							//角速度XYZ分量

void JY901_GetOneByte(u8 data)
{
		static u8 ucRxBuffer[12];
		static u8 ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=data;
	if(ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;																																	  
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])
		{
			
			case 0x51:	
						accX=(float)((int16_t)(ucRxBuffer[3]<<8)|ucRxBuffer[2])/32768*16*9.8f;
						accY=(float)((int16_t)(ucRxBuffer[5]<<8)|ucRxBuffer[4])/32768*16*9.8f;
						accZ=(float)((int16_t)(ucRxBuffer[7]<<8)|ucRxBuffer[6])/32768*16*9.8f;
						break;
			case 0x52:	
						gyrX=(float)((int16_t)(ucRxBuffer[3]<<8)|ucRxBuffer[2])/32768*2000;
						gyrY=(float)((int16_t)(ucRxBuffer[5]<<8)|ucRxBuffer[4])/32768*2000;
						gyrZ=(float)((int16_t)(ucRxBuffer[7]<<8)|ucRxBuffer[6])/32768*2000;
						flag.gyro_ok=1;
						break;
			case 0x53:	
						roll	=(float)((int16_t)(ucRxBuffer[3]<<8)|ucRxBuffer[2])/32768*180;
						pitch =(float)((int16_t)(ucRxBuffer[5]<<8)|ucRxBuffer[4])/32768*180;
						yaw 	=(float)((int16_t)(ucRxBuffer[7]<<8)|ucRxBuffer[6])/32768*180;
//						printf("roll:%.2f pitch:%.2f yaw:%.2f \n",roll,pitch,yaw);
						flag.angle_ok=1;
						break;
		}
		//safetyCheck();
		ucRxCnt=0;
	}
}

