//FUSION CONTROLLER BY AEMAETH2501

#include "userDefine.h"

void ATT_PID() //姿态串级PID
{	
	
	//以下为角度环 PI控制	
	
	static float out_ki_angle_roll=0,out_ki_angle_pitch=0,out_ki_angle_yaw=0,
							 out_angle_roll=0,out_angle_pitch=0,out_angle_yaw=0;
	if(roll_temp>-5&&roll_temp<5)
	out_ki_angle_roll+=ki_roll*roll_temp*0.01f;
	else
	out_ki_angle_roll=0;	
	
	if(pitch_temp>-5&&pitch_temp<5)
	out_ki_angle_pitch+=ki_pitch*pitch_temp*0.01f;
	else
	out_ki_angle_pitch=0;	
	
	if(yaw_temp>-5&&yaw_temp<5)
	out_ki_angle_yaw+=ki_yaw*yaw_temp*0.01f;
	else
	out_ki_angle_yaw=0;	
	
	if(out_ki_angle_roll>500)out_ki_angle_roll=500;										//积分限幅
	else if(out_ki_angle_roll<-500)out_ki_angle_roll=-500;
	if(out_ki_angle_pitch>500)out_ki_angle_pitch=500;										//积分限幅
	else if(out_ki_angle_pitch<-500)out_ki_angle_pitch=-500;
	if(out_ki_angle_yaw>500)out_ki_angle_yaw=500;										//积分限幅
	else if(out_ki_angle_yaw<-500)out_ki_angle_yaw=-500;

	out_angle_roll=kp_roll*roll_temp+out_ki_angle_roll;
	out_angle_pitch=kp_pitch*pitch_temp+out_ki_angle_pitch;
	out_angle_yaw=kp_yaw*yaw_temp+out_ki_angle_yaw;
	
	//以下为角速度环

	static float	Dvalue_gyro_roll=0,Dvalue_gyro_pitch=0,Dvalue_gyro_yaw=0,
								out_kp_gyro_roll=0,out_kp_gyro_pitch=0,out_kp_gyro_yaw=0,
								out_ki_gyro_roll=0,out_ki_gyro_pitch=0,out_ki_gyro_yaw=0,
								out_kd_gyro_roll=0,out_kd_gyro_pitch=0,out_kd_gyro_yaw=0,
								old_Dvalue_gyro_roll=0,old_Dvalue_gyro_pitch=0,old_Dvalue_gyro_yaw=0,
								out_gyro_roll=0,out_gyro_pitch=0,out_gyro_yaw=0;
	
	Dvalue_gyro_roll=(out_angle_roll*4.0f+gyrX*1.0f);
	Dvalue_gyro_pitch=(out_angle_pitch*4.0f+gyrY*1.0f);
	Dvalue_gyro_yaw=(out_angle_yaw*4.0f+gyrZ*1.0f);	
	
	out_kp_gyro_roll=kp_g_roll*Dvalue_gyro_roll;
	out_kp_gyro_pitch=kp_g_pitch*Dvalue_gyro_pitch;
	out_kp_gyro_yaw=kp_g_yaw*Dvalue_gyro_yaw;
	
	out_ki_gyro_roll+=ki_g_roll*Dvalue_gyro_roll*0.5f;
	out_ki_gyro_pitch+=ki_g_pitch*Dvalue_gyro_pitch*0.5f;
	out_ki_gyro_yaw+=ki_g_yaw*Dvalue_gyro_yaw*0.5f;
	
	if(out_ki_gyro_roll>500)out_ki_gyro_roll=500;										//积分限幅
	else if(out_ki_gyro_roll<-500)out_ki_gyro_roll=-500;
	if(out_ki_gyro_pitch>500)out_ki_gyro_pitch=500;										//积分限幅
	else if(out_ki_gyro_pitch<-500)out_ki_gyro_pitch=-500;
	if(out_ki_gyro_yaw>500)out_ki_gyro_yaw=500;										//积分限幅
	else if(out_ki_gyro_yaw<-500)out_ki_gyro_yaw=-500;
	
	out_kd_gyro_roll=kd_g_roll*(Dvalue_gyro_roll-old_Dvalue_gyro_roll)*20.0f;
	out_kd_gyro_pitch=kd_g_pitch*(Dvalue_gyro_pitch-old_Dvalue_gyro_pitch)*20.0f;
	out_kd_gyro_yaw=kd_g_yaw*(Dvalue_gyro_yaw-old_Dvalue_gyro_yaw)*20.0f;
	old_Dvalue_gyro_roll=Dvalue_gyro_roll;
	old_Dvalue_gyro_pitch=Dvalue_gyro_pitch;
	old_Dvalue_gyro_yaw=Dvalue_gyro_yaw;
	
	out_gyro_roll=out_kp_gyro_roll+out_ki_gyro_roll+out_kd_gyro_roll;
	out_gyro_pitch=out_kp_gyro_pitch+out_ki_gyro_pitch+out_kd_gyro_pitch;
	out_gyro_yaw=out_kp_gyro_yaw+out_ki_gyro_yaw+out_kd_gyro_yaw;
	
	att_cal1=out_gyro_roll-out_gyro_pitch+out_gyro_yaw;
	att_cal2=out_gyro_roll+out_gyro_pitch-out_gyro_yaw;
	att_cal3=(-out_gyro_roll-out_gyro_pitch-out_gyro_yaw);
	att_cal4=(-out_gyro_roll+out_gyro_pitch+out_gyro_yaw);
	
}



void LOC_PID() //位置环  PI
{
	//OF_DX2,OF_DY2,OF_DX2FIX,OF_DY2FIX   CM/S
	
	static int16_t out_i_x=0,out_i_y=0,out_x=0,out_y=0;
	if(OF_DX2FIX>-50&&OF_DX2FIX<50&&OF_DY2FIX>-50&&OF_DY2FIX<50)
	{
	out_i_x+=OF_DX2FIX;
	out_i_y+=OF_DY2FIX;
	
	if(out_i_x>300)out_i_x=300;										//积分限幅
	else if(out_i_x<-300)out_i_x=-300;
	if(out_i_y>300)out_i_y=300;										//积分限幅
	else if(out_i_y<-300)out_i_y=-300;
	
	
	out_x=kp_x*OF_DX2FIX+ki_x*out_i_x;
	out_y=kp_y*OF_DY2FIX+ki_y*out_i_y;
	
	}
	loc_cal1=(-out_y-out_x);
	loc_cal2=(out_x-out_y);
	loc_cal3=(out_y-out_x);
	loc_cal4=(out_x+out_y);
	
}

void ALT_PID()//高度环PID
{
	static int16_t Err=0,out_i_Err=0,old_Err=0;
	
	Err=tarHeight-OF_ALT2;
	if(Err>-50&&Err<50) //误差超过50cm 放弃高度控制 以免意外
		{
			out_i_Err+=Err;
			
	if(out_i_Err>100)out_i_Err=100;										//积分限幅
	else if(out_i_Err<-100)out_i_Err=-100;
	}
	alt_cal1=kp_h*Err+ki_h*out_i_Err+kd_h*(Err-old_Err);
	alt_cal2=kp_h*Err+ki_h*out_i_Err+kd_h*(Err-old_Err);
	alt_cal3=kp_h*Err+ki_h*out_i_Err+kd_h*(Err-old_Err);
	alt_cal4=kp_h*Err+ki_h*out_i_Err+kd_h*(Err-old_Err);
	
	old_Err=Err;
		
}



