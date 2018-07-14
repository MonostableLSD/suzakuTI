#include "userDefine.h"

_flag flag;
_sensor_hd_check_st sens_hd_check;

float tarYaw;
u16 tarHeight=30;
u16 throttle=100;

float roll_temp,pitch_temp,yaw_temp;

float att_cal1,att_cal2,att_cal3,att_cal4;
float alt_cal1,alt_cal2,alt_cal3,alt_cal4;
float loc_cal1,loc_cal2,loc_cal3,loc_cal4;

struct EEPROMData e2prom_value;

float kp_roll=0, kp_g_roll=0;																	  //pid参数
float kp_pitch=0,kp_g_pitch=0;
float kp_yaw=0,  kp_g_yaw=0;
float ki_roll=0, ki_g_roll=0;
float ki_pitch=0,ki_g_pitch=0;
float ki_yaw=0,  ki_g_yaw=0;
float kd_g_roll=0;
float kd_g_pitch=0;
float kd_g_yaw=0;
float	kp_x=0,kp_y=0,ki_x,ki_y,kd_x,kd_y;
float kp_h=0,ki_h=0,kd_h=0;

float offsetRoll=0,offsetPitch=0;									//角度零偏，抵消6050焊接不平的误差


//short offsetMagX=0,offsetMagY=0,offsetMagZ=0;		//磁力计偏移量


