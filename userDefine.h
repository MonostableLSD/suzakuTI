#ifndef __DEFINE_H
#define	__DEFINE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "IQmath/IQmathLib.h"

#include "inc/hw_memmap.h"	//外设基址宏定义
#include "inc/hw_types.h"   //位带操作的宏定义 
#include "inc/hw_ints.h"		

#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/rom.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"

#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
#include "driverlib/eeprom.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#define SysTickPeriod 200																							//每(1/SysTickPeriod)==5MS秒进入SysTick中断 

#define	MIN_PWMPULSE  4000
#define	MAX_PWMPULSE  11000

#define UART0Baud     115200
#define UART1Baud     115200
#define UART5Baud			500000


#define MIN_THROTTLE	100
#define MAX_THROTTLE	5000




typedef struct
{
	//基本状态/传感器
	u8 start_ok;
	u8 sensor_ok;
	
	u8 angle_ok;
	u8 gyro_ok;
	u8 OF_loc_ok;  		//确认收到完整的同步的光流信息
	u8 OF_height_ok;	

	
	
//	//控制状态
//	u8 manual_locked;
//	u8 unlock_en;
//	u8 fly_ready;		//unlocked
//	u8 thr_low;
//	u8 locking;

	u8 set_yaw;
	
	u8 ct_att_hold;		//姿态环控制
	u8 ct_alt_hold;		//高度环控制
	u8 ct_loc_hold;		//位置环控制
	
	u8 taking_off;  //纯姿态起飞
	u8 landing;			//纯姿态降落
	u8 all_stop;		//紧急停止

}_flag;
extern _flag flag;

typedef struct
{
	u8 gyro_ok;
	u8 acc_ok;
	u8 mag_ok;
	u8 baro_ok;
	u8 gps_ok;
	u8 sonar_ok;
	u8 tof_ok;
	u8 of_ok;
	
} _sensor_hd_check_st; //Hardware
extern _sensor_hd_check_st sens_hd_check;

extern u16 throttle;
extern float offsetRoll,offsetPitch;

extern float kp_roll, kp_g_roll;																	//pid参数
extern float kp_pitch,kp_g_pitch;
extern float kp_yaw,  kp_g_yaw;
extern float ki_roll, ki_g_roll;
extern float ki_pitch,ki_g_pitch;
extern float ki_yaw,  ki_g_yaw;
extern float kd_g_roll;
extern float kd_g_pitch;
extern float kd_g_yaw;
extern float kp_h,ki_h,kd_h;
extern float kp_x,ki_x,kd_x;
extern float kp_y,ki_y,kd_y;

void saveEEPROMData(void);
extern struct EEPROMData e2prom_value;
struct EEPROMData
{
	float offsetRoll;
	float offsetPitch;
	float kp_roll;
	float kp_pitch;
	float kp_yaw;
	float ki_roll;
	float ki_pitch;
	float ki_yaw;
	float kd_roll;
	float kd_pitch;
	float kd_yaw;
	float kp_g_roll;
	float kp_g_pitch;
	float kp_g_yaw;
	float ki_g_roll;
	float ki_g_pitch;
	float ki_g_yaw;	
	float kd_g_roll;
	float kd_g_pitch;
	float kd_g_yaw;
	float kp_h;
	float ki_h;
	float kd_h;
	float kp_x,ki_x,kd_x;
	float kp_y,ki_y,kd_y;
};



//初始化
u8 All_init(void);
void initLED(void);
void initClock(void);
void initSysTick(void);
void initPWM(void);
void initUART(void);
void initEEPROM(void);
void initKey(void);
void initFlag(void);


//中断
void SysTickIntHandler(void);
void UART0_Handler(void);													
void UART1_Handler(void);
void UART5_Handler(void);

int fputc(int ch, FILE *f);	


//JY901
extern float roll,pitch,yaw;																							//分别对应YXZ轴的角度
extern float accX,accY,accZ;																							//加速度XYZ分量
extern float gyrX,gyrY,gyrZ;																							//角速度XYZ分量
													
void JY901_GetOneByte(uint8_t data);


//电机
extern u32 moto1,moto2,moto3,moto4;
void motoRefresh(u32 moto11,u32 moto22,u32 moto33,u32 moto44);

//LED
void redLed(void);
void blueLed(void);
void greenLed(void);
void landLed(void);
void turnoffled(void);


//PID
extern float tarYaw;
extern u16 tarHeight;

extern float att_cal1,att_cal2,att_cal3,att_cal4;
extern float alt_cal1,alt_cal2,alt_cal3,alt_cal4;
extern float loc_cal1,loc_cal2,loc_cal3,loc_cal4;


void ATT_PID(void);
void LOC_PID(void);
void ALT_PID(void);




//飞行控制
void Auto_take_off(void);
void Auto_land(void);
void Auto_hover(void);
void safetyCheck(void);


//计算
extern float roll_temp,pitch_temp,yaw_temp;																		//分别对应YXZ轴的角度

void motoCal(void);
void ATT_TEMP(void);



//光流
extern uint8_t 	OF_QUA,OF_LIGHT;//光流信息质量：QUA 光照强度：LIGHT

extern int8_t	OF_DX,OF_DY;//原始光流信息，具体意义见光流模块手册

extern int16_t	OF_DX2,OF_DY2,OF_DX2FIX,OF_DY2FIX;//融合后的光流信息，具体意义见光流模块手册

extern uint16_t	OF_ALT,OF_ALT2;//原始高度信息和融合后高度信息

extern int16_t	OF_GYR_X,OF_GYR_Y,OF_GYR_Z;//原始陀螺仪数据

extern int16_t	OF_GYR_X2,OF_GYR_Y2,OF_GYR_Z2;//滤波后陀螺仪数据

extern int16_t	OF_ACC_X,OF_ACC_Y,OF_ACC_Z;//原始加速度数据

extern int16_t	OF_ACC_X2,OF_ACC_Y2,OF_ACC_Z2;	//滤波后加速度数据

extern float	OF_ATT_ROL,OF_ATT_PIT,OF_ATT_YAW;	//欧拉角格式的姿态数据

extern float	OF_ATT_S1,OF_ATT_S2,OF_ATT_S3,OF_ATT_S4;	//四元数格式的姿态数据


void AnoOF_GetOneByte(uint8_t data);//本函数是唯一一个需要外部调用的函数，因为光流模块是串口输出数据
																		//所以本函数需要在串口接收中断中调用，每接收一字节数据，调用本函数一次
void AnoOF_DataAnl_Task(u8 dT_ms);
void AnoOF_Check(u8 dT_ms);


#endif
