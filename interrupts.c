
#include "userDefine.h"

void SysTickIntHandler()
{		static u8 ms10=0,ms20=0,ms35=0,ms500=0;
		if(flag.start_ok)		
		{
			ms10++;
			ms20++;
			ms35++;
			ms500++;

			if(flag.angle_ok&&flag.gyro_ok&&flag.ct_att_hold)
			{
				flag.gyro_ok=0;
				flag.angle_ok=0;			//手动保证角度数据更新不会被tick打断 虽然感觉效率低了点但是保证不会出现输入pid的不是同步的欧拉角	
		
				ATT_TEMP();						//   缓存	处理零偏
				ATT_PID();					 //将处理后的欧拉角带入计算 得出PID输出 G-PI A-PD
			}
			
				/*重要！！！*/
				motoCal();							 //根据不同的指令以及当前状态update moto1234
				motoRefresh(moto1,moto2,moto3,moto4);//pwmoutput
				
			if(ms10==2) 	//100hz  each 10ms check the safety and refreshMoto
				{	ms10=0;
				safetyCheck();
				}
			
			if(ms20==4) 	//50hz  each 20ms update data of lightflow vxvy	
				{	ms20=0;
					if(flag.OF_loc_ok&&flag.ct_loc_hold)
						{
							flag.OF_loc_ok=0;
							//LOC_cal and update 期望0 PI控制 OUTPUT
							LOC_PID();
						}
				}
			if(ms35==7)		//28hz	each 35ms update data of lightflow height field
				{ ms35=0;
					if(flag.OF_height_ok&&flag.ct_alt_hold)
					{flag.OF_height_ok=0;
						//heightctrl data cal and update
						ALT_PID();
						Auto_land();				//判断自动降落flag 再判断高度 如果小于20 那么flag stop 打开 停在手上
					}
				}
			if(ms500==100)
			{
					ms500=0;
//			//	if(flag.angle_ok&&flag.gyro_ok&&flag.OF_height_ok&&flag.OF_loc_ok)

//					printf("moto1:%d   height:%d  roll:%.2f   pitch:%.2f   yaw:%.2f.\n",moto1,OF_ALT2,roll,pitch,yaw);
//					printf("OF_DX2:%d   OF_DY2:%d  OF_DX2FIX:%d   OF_DY2FIX:%d\n",OF_DX2,OF_DY2,OF_DX2FIX,OF_DY2FIX);	
//					printf("%d,%.2f,%.2f,%.2f,%.2f\n",throttle,loc_cal1,loc_cal2,loc_cal3,loc_cal4);

			}	
	
		}
}

void UART0_Handler(void)
{
	u32 ITStatus; 
	u8  temp_data;
	ITStatus=ROM_UARTIntStatus(UART0_BASE,1);	//得到哪个中断标志位
	ROM_UARTIntClear(UART0_BASE,ITStatus);		//清除中断标志位
	temp_data=ROM_UARTCharGetNonBlocking(UART0_BASE);		//取出串口中的数据，无论数据是否用到，此句必需，作用猜测为清空接收缓冲区

		switch(temp_data)
			{
				case 0x46:
				redLed();//Red LED means "all stop"
				motoRefresh(MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE);
				ROM_IntMasterDisable();
				break;
				case 0x41:
				//	flag.taking_off=1;greenLed();
				flag.ct_att_hold=1;
				flag.ct_alt_hold=0;
				flag.ct_loc_hold=0;
				printf("att turn on\n");
				
				break;
				case 0x42:
				//flag.landing=1;		landLed();
				flag.ct_att_hold=0;
				flag.ct_alt_hold=0;
				flag.ct_loc_hold=1;
				break;
				case 0x43:
					//redLed();motoRefresh(MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE);ROM_IntMasterDisable();
				flag.ct_att_hold=0;
				flag.ct_alt_hold=1;
				flag.ct_loc_hold=0;
				break;
				case 0x44:
				flag.ct_att_hold=1;
				flag.ct_alt_hold=1;
				flag.ct_loc_hold=0;
					break;
				case 0x45:
				flag.ct_att_hold=1;
				flag.ct_alt_hold=1;
				flag.ct_loc_hold=1;
					break;
				
			case 0x10:															//获取全部参数型数据
			printf("offsetRoll:%.1f  offsetPitch:%.1f \n",offsetRoll,offsetPitch);
			printf("kp_roll:%.2f  ki_roll:%.4f  kp_g_roll:%.2f  ki_g_roll:%.2f  kd_g_roll:%.2f\n",kp_roll,ki_roll,kp_g_roll,ki_g_roll,kd_g_roll);
			printf("kp_pitch:%.2f  ki_pitch:%.4f  kp_g_pitch:%.2f  ki_g_pitch:%.2f  kd_g_pitch:%.2f\n",kp_pitch,ki_pitch,kp_g_pitch,ki_g_pitch,kd_g_pitch);
			printf("kp_yaw:%.2f  ki_yaw:%.4f  kp_g_yaw:%.2f  ki_g_yaw:%.2f  kd_g_yaw:%.2f\n",kp_yaw,ki_yaw,kp_g_yaw,ki_g_yaw,kd_g_yaw);
			printf("kp_h:%.2f  ki_h:%.4f  kd_h:%.2f\n",kp_h,ki_h,kd_h);
			printf("kp_x:%.2f  kp_y:%.2f  ki_x:%.2f  ki_y:%.2f",kp_x,kp_y,ki_x,ki_y);break;
			
			//height
			case 0x11:kp_h+=0.1f;printf("Now kp_h is %.2f.\r\n",kp_h);break;
			case 0x12:kp_h-=0.1f;printf("Now kp_h is %.2f.\r\n",kp_h);break;
			case 0x13:ki_h+=0.1f;printf("Now ki_h is %.2f.\r\n",ki_h);break;
			case 0x14:ki_h-=0.1f;printf("Now ki_h is %.2f.\r\n",ki_h);break;
			case 0x15:kd_h+=0.1f;printf("Now kd_h is %.2f.\r\n",kd_h);break;
			case 0x16:kd_h-=0.1f;printf("Now kd_h is %.2f.\r\n",kd_h);break;
			
			
			
			case 0x22:offsetRoll+=0.1f;printf("offsetRoll is %.1f.\r\n",offsetRoll);break;
			case 0x23:offsetRoll-=0.1f;printf("offsetRoll is %.1f.\r\n",offsetRoll);break;
			case 0x24:offsetPitch+=0.1f;printf("offsetPitch is %.1f.\r\n",offsetPitch);break;
			case 0x25:offsetPitch-=0.1f;printf("offsetPitch is %.1f.\r\n",offsetPitch);break;
			case 0x26:throttle+=100;if(throttle>=MAX_THROTTLE)throttle=MAX_THROTTLE;printf ("Now throttle is %d.\r\n",throttle);break;
			case 0x27:throttle-=100;if(throttle<=MIN_THROTTLE)throttle=MIN_THROTTLE;printf ("Now throttle is %d.\r\n",throttle);break;
			
			//roll
			case 0x30:kp_roll+=0.1f;printf ("Now kp_roll is %.2f.\r\n",kp_roll);break;
			case 0x31:kp_roll-=0.1f;printf ("Now kp_roll is %.2f.\r\n",kp_roll);break;
			case 0x32:ki_roll+=0.1f;printf ("Now ki_roll is %.4f.\r\n",ki_roll);break;
			case 0x33:ki_roll-=0.1f;printf ("Now ki_roll is %.4f.\r\n",ki_roll);break;

			case 0x34:kp_g_roll+=0.1f;printf ("Now kp_gyro_roll is %.2f.\r\n",kp_g_roll);break;	 
			case 0x35:kp_g_roll-=0.1f;printf ("Now kp_gyro_roll is %.2f.\r\n",kp_g_roll);break;
			case 0x36:kd_g_roll+=0.1f;printf ("Now kd_gyro_roll is %.2f.\r\n",kd_g_roll);break;
			case 0x37:kd_g_roll-=0.1f;printf ("Now kd_gyro_roll is %.2f.\r\n",kd_g_roll);break;
			
			case 0x38:ki_g_roll+=0.01f;printf ("Now ki_gyro_roll is %.2f.\r\n",ki_g_roll);break;
			case 0x39:ki_g_roll-=0.01f;printf ("Now ki_gyro_roll is %.2f.\r\n",ki_g_roll);break;
		
			
		//pitch	
			case 0x50:kp_pitch+=0.1f;printf ("Now kp_pitch is %.2f.\r\n",kp_pitch);break;
			case 0x51:kp_pitch-=0.1f;printf ("Now kp_pitch is %.2f.\r\n",kp_pitch);break;
			case 0x52:ki_pitch+=0.1f;printf ("Now ki_pitch is %.4f.\r\n",ki_pitch);break;
			case 0x53:ki_pitch-=0.1f;printf ("Now ki_pitch is %.4f.\r\n",ki_pitch);break;

			case 0x54:kp_g_pitch+=0.1f;printf ("Now kp_gyro_pitch is %.2f.\r\n",kp_g_pitch);break;	 
			case 0x55:kp_g_pitch-=0.1f;printf ("Now kp_gyro_pitch is %.2f.\r\n",kp_g_pitch);break;
			case 0x56:kd_g_pitch+=0.1f;printf ("Now kd_gyro_pitch is %.2f.\r\n",kd_g_pitch);break;
			case 0x57:kd_g_pitch-=0.1f;printf ("Now kd_gyro_pitch is %.2f.\r\n",kd_g_pitch);break;
			
			case 0x58:ki_g_pitch+=0.01f;printf ("Now ki_gyro_pitch is %.2f.\r\n",ki_g_pitch);break;
			case 0x59:ki_g_pitch-=0.01f;printf ("Now ki_gyro_pitch is %.2f.\r\n",ki_g_pitch);break;


		//yaw
			case 0x60:kp_yaw+=1.0f;printf ("Now kp_yaw is %.2f.\r\n",kp_yaw);break;
			case 0x61:kp_yaw-=1.0f;printf ("Now kp_yaw is %.2f.\r\n",kp_yaw);break;
			case 0x62:ki_yaw+=0.1f;printf ("Now ki_yaw is %.4f.\r\n",ki_yaw);break;
			case 0x63:ki_yaw-=0.1f;printf ("Now ki_yaw is %.4f.\r\n",ki_yaw);break;

			case 0x64:kp_g_yaw+=1.0f;printf ("Now kp_gyro_yaw is %.2f.\r\n",kp_g_yaw);break;	 
			case 0x65:kp_g_yaw-=1.0f;printf ("Now kp_gyro_yaw is %.2f.\r\n",kp_g_yaw);break;
			case 0x66:kd_g_yaw+=0.1f;printf ("Now kd_gyro_yaw is %.2f.\r\n",kd_g_yaw);break;
			case 0x67:kd_g_yaw-=0.1f;printf ("Now kd_gyro_yaw is %.2f.\r\n",kd_g_yaw);break;
			
			case 0x68:ki_g_yaw+=0.01f;printf ("Now ki_gyro_yaw is %.2f.\r\n",ki_g_yaw);break;
			case 0x69:ki_g_yaw-=0.01f;printf ("Now ki_gyro_yaw is %.2f.\r\n",ki_g_yaw);break;
			
		//xy 速度 位置	
			case 0x70:kp_x+=0.1f;printf("Now kp_x is %.2f.\r\n",kp_x);break;
			case 0x71:kp_x-=0.1f;printf("Now kp_x is %.2f.\r\n",kp_x);break;
			case 0x72:kp_y+=0.1f;printf("Now kp_y is %.2f.\r\n",kp_y);break;
			case 0x73:kp_y-=0.1f;printf("Now kp_y is %.2f.\r\n",kp_y);break;
			
			case 0x74:ki_x+=0.1f;printf("Now ki_x is %.2f.\r\n",ki_x);break;
			case 0x75:ki_x-=0.1f;printf("Now ki_x is %.2f.\r\n",ki_x);break;
			case 0x76:ki_y+=0.1f;printf("Now ki_y is %.2f.\r\n",ki_y);break;
			case 0x77:ki_y-=0.1f;printf("Now ki_y is %.2f.\r\n",ki_y);break;
			
			}

//		/*************分割线**********************分割线*******************************************/
//		case 0x30:kp_angle_roll+=1.0f;printf ("Now kp_angle_roll is %.2f.\r\n",kp_angle_roll);break;
//		case 0x31:kp_angle_roll-=1.0f;printf ("Now kp_angle_roll is %.2f.\r\n",kp_angle_roll);break;
////		case 0x32:ki_angle_roll+=0.1f;printf ("Now ki_angle_roll is %.4f.\r\n",ki_angle_roll);break;
////		case 0x33:ki_angle_roll-=0.1f;printf ("Now ki_angle_roll is %.4f.\r\n",ki_angle_roll);break;
			
			
//		case 0x34:ki_gyro_roll+=0.01f;printf ("Now ki_gyro_roll is %.2f.\r\n",ki_gyro_roll);break;
//		case 0x35:ki_gyro_roll-=0.01f;printf ("Now ki_gyro_roll is %.2f.\r\n",ki_gyro_roll);break;
//		case 0x36:kp_gyro_roll+=1.0f;printf ("Now kp_gyro_roll is %.2f.\r\n",kp_gyro_roll);break;	 
//		case 0x37:kp_gyro_roll-=1.0f;printf ("Now kp_gyro_roll is %.2f.\r\n",kp_gyro_roll);break;
//		case 0x38:kd_gyro_roll+=0.1f;printf ("Now kd_gyro_roll is %.2f.\r\n",kd_gyro_roll);break;
//		case 0x39:kd_gyro_roll-=0.1f;printf ("Now kd_gyro_roll is %.2f.\r\n",kd_gyro_roll);break;
//		
//		case 0x40:kp_angle_roll+=0.1f;printf ("Now kp_angle_roll is %.2f.\r\n",kp_angle_roll);break;
//		case 0x41:kp_angle_roll-=0.1f;printf ("Now kp_angle_roll is %.2f.\r\n",kp_angle_roll);break;
////		case 0x42:ki_angle_roll+=0.001f;printf ("Now ki_angle_roll is %.4f.\r\n",ki_angle_roll);break;
////		case 0x43:ki_angle_roll-=0.001f;printf ("Now ki_angle_roll is %.4f.\r\n",ki_angle_roll);break;
//		case 0x44:ki_gyro_roll+=0.1f;printf ("Now ki_gyro_roll is %.2f.\r\n",ki_gyro_roll);break;
//		case 0x45:ki_gyro_roll-=0.1f;printf ("Now ki_gyro_roll is %.2f.\r\n",ki_gyro_roll);break;
//		case 0x46:kp_gyro_roll+=0.1f;printf ("Now kp_gyro_roll is %.2f.\r\n",kp_gyro_roll);break;						
//		case 0x47:kp_gyro_roll-=0.1f;printf ("Now kp_gyro_roll is %.2f.\r\n",kp_gyro_roll);break;
//		case 0x48:kd_gyro_roll+=0.1f;printf ("Now kd_gyro_roll is %.2f.\r\n",kd_gyro_roll);break;
//		case 0x49:kd_gyro_roll-=0.1f;printf ("Now kd_gyro_roll is %.2f.\r\n",kd_gyro_roll);break;
//		/**************分割线**********************分割线*********************************************/
//		case 0x50:kp_angle_pitch+=1.0f;printf ("Now kp_angle_pitch is %.2f.\r\n",kp_angle_pitch);break;
//		case 0x51:kp_angle_pitch-=1.0f;printf ("Now kp_angle_pitch is %.2f.\r\n",kp_angle_pitch);break;
////		case 0x52:ki_angle_pitch+=0.01f;printf ("Now ki_angle_pitch is %.4f.\r\n",ki_angle_pitch);break;
////		case 0x53:ki_angle_pitch-=0.01f;printf ("Now ki_angle_pitch is %.4f.\r\n",ki_angle_pitch);break;
//		case 0x54:ki_gyro_pitch+=1.0f;printf ("Now ki_gyro_pitch is %.2f.\r\n",ki_gyro_pitch);break;
//		case 0x55:ki_gyro_pitch-=1.0f;printf ("Now ki_gyro_pitch is %.2f.\r\n",ki_gyro_pitch);break;
//		case 0x56:kp_gyro_pitch+=1.0f;printf ("Now kp_gyro_pitch is %.2f.\r\n",kp_gyro_pitch);break;					
//		case 0x57:kp_gyro_pitch-=1.0f;printf ("Now kp_gyro_pitch is %.2f.\r\n",kp_gyro_pitch);break;
//		case 0x58:kd_gyro_pitch+=1.0f;printf ("Now kd_gyro_pitch is %.2f.\r\n",kd_gyro_pitch);break;
//		case 0x59:kd_gyro_pitch-=1.0f;printf ("Now kd_gyro_pitch is %.2f.\r\n",kd_gyro_pitch);break;
//		
//		case 0x60:kp_angle_pitch+=0.1f;printf ("Now kp_angle_pitch is %.2f.\r\n",kp_angle_pitch);break;
//		case 0x61:kp_angle_pitch-=0.1f;printf ("Now kp_angle_pitch is %.2f.\r\n",kp_angle_pitch);break;
////		case 0x62:ki_angle_pitch+=0.001f;printf ("Now ki_angle_pitch is %.4f.\r\n",ki_angle_pitch);break;
////		case 0x63:ki_angle_pitch-=0.001f;printf ("Now ki_angle_pitch is %.4f.\r\n",ki_angle_pitch);break;
//		case 0x64:ki_gyro_pitch+=0.1f;printf ("Now ki_gyro_pitch is %.2f.\r\n",ki_gyro_pitch);break;
//		case 0x65:ki_gyro_pitch-=0.1f;printf ("Now ki_gyro_pitch is %.2f.\r\n",ki_gyro_pitch);break;
//		case 0x66:kp_gyro_pitch+=0.1f;printf ("Now kp_gyro_pitch is %.2f.\r\n",kp_gyro_pitch);break;						
//		case 0x67:kp_gyro_pitch-=0.1f;printf ("Now kp_gyro_pitch is %.2f.\r\n",kp_gyro_pitch);break;
//		case 0x68:kd_gyro_pitch+=0.1f;printf ("Now kd_gyro_pitch is %.2f.\r\n",kd_gyro_pitch);break;
//		case 0x69:kd_gyro_pitch-=0.1f;printf ("Now kd_gyro_pitch is %.2f.\r\n",kd_gyro_pitch);break;		
//		/***************分割线***********************分割线*******************************************/
//		case 0x70:kp_angle_yaw+=1.0f;printf ("Now kp_angle_yaw is %.2f.\r\n",kp_angle_yaw);break;
//		case 0x71:kp_angle_yaw-=1.0f;printf ("Now kp_angle_yaw is %.2f.\r\n",kp_angle_yaw);break;
//		case 0x72:ki_angle_yaw+=0.01f;printf ("Now ki_angle_yaw is %.4f.\r\n",ki_angle_yaw);break;
//		case 0x73:ki_angle_yaw-=0.01f;printf ("Now ki_angle_yaw is %.4f.\r\n",ki_angle_yaw);break;
//		case 0x74:kd_angle_yaw+=1.0f;printf ("Now kd_angle_yaw is %.2f.\r\n",kd_angle_yaw);break;
//		case 0x75:kd_angle_yaw-=1.0f;printf ("Now kd_angle_yaw is %.2f.\r\n",kd_angle_yaw);break;
//		case 0x76:kp_gyro_yaw+=1.0f;printf ("Now kp_gyro_yaw is %.2f.\r\n",kp_gyro_yaw);break;					
//		case 0x77:kp_gyro_yaw-=1.0f;printf ("Now kp_gyro_yaw is %.2f.\r\n",kp_gyro_yaw);break;
//		case 0x78:kd_gyro_yaw+=1.0f;printf ("Now kd_gyro_yaw is %.2f.\r\n",kd_gyro_yaw);break;
//		case 0x79:kd_gyro_yaw-=1.0f;printf ("Now kd_gyro_yaw is %.2f.\r\n",kd_gyro_yaw);break;
//		
//		case 0x80:kp_angle_yaw+=0.1f;printf ("Now kp_angle_yaw is %.2f.\r\n",kp_angle_yaw);break;
//		case 0x81:kp_angle_yaw-=0.1f;printf ("Now kp_angle_yaw is %.2f.\r\n",kp_angle_yaw);break;
//		case 0x82:ki_angle_yaw+=0.001f;printf ("Now ki_angle_yaw is %.4f.\r\n",ki_angle_yaw);break;
//		case 0x83:ki_angle_yaw-=0.001f;printf ("Now ki_angle_yaw is %.4f.\r\n",ki_angle_yaw);break;
//		case 0x84:kd_angle_yaw+=0.1f;printf ("Now kd_angle_yaw is %.2f.\r\n",kd_angle_yaw);break;
//		case 0x85:kd_angle_yaw-=0.1f;printf ("Now kd_angle_yaw is %.2f.\r\n",kd_angle_yaw);break;
//		case 0x86:kp_gyro_yaw+=0.1f;printf ("Now kp_gyro_yaw is %.2f.\r\n",kp_gyro_yaw);break;					
//		case 0x87:kp_gyro_yaw-=0.1f;printf ("Now kp_gyro_yaw is %.2f.\r\n",kp_gyro_yaw);break;
//		case 0x88:kd_gyro_yaw+=0.1f;printf ("Now kd_gyro_yaw is %.2f.\r\n",kd_gyro_yaw);break;
//		case 0x89:kd_gyro_yaw-=0.1f;printf ("Now kd_gyro_yaw is %.2f.\r\n",kd_gyro_yaw);break;
//		/***************分割线***********************分割线*******************************************/
//		case 0x90:kp_height+=0.1f;printf ("Now kp_height is %.2f.\r\n",kp_height);break;
//		case 0x91:kp_height-=0.1f;printf ("Now kp_height is %.2f.\r\n",kp_height);break;
//		case 0x92:ki_height+=0.1f;printf ("Now ki_height is %.2f.\r\n",ki_height);break;
//		case 0x93:ki_height-=0.1f;printf ("Now ki_height is %.2f.\r\n",ki_height);break;
//		case 0x94:kd_height+=0.1f;printf ("Now kd_height is %.2f.\r\n",kd_height);break;
//		case 0x95:kd_height-=0.1f;printf ("Now kd_height is %.2f.\r\n",kd_height);break;
//		
//		case 0xa0:kp_height+=0.01f;printf ("Now kp_height is %.2f.\r\n",kp_height);break;
//		case 0xa1:kp_height-=0.01f;printf ("Now kp_height is %.2f.\r\n",kp_height);break;
//		case 0xa2:ki_height+=0.01f;printf ("Now ki_height is %.2f.\r\n",ki_height);break;
//		case 0xa3:ki_height-=0.01f;printf ("Now ki_height is %.2f.\r\n",ki_height);break;
//		case 0xa4:kd_height+=0.01f;printf ("Now kd_height is %.2f.\r\n",kd_height);break;
//		case 0xa5:kd_height-=0.01f;printf ("Now kd_height is %.2f.\r\n",kd_height);break;
//		/***以下为自定义区********分割线***********************分割线****************************/
//		case 0xb0:																														//参数重置
//			throttle=MIN_PWMPULSE;
//			kp_angle_roll=0,kp_gyro_roll=0,kp_angle_pitch=0,kp_gyro_pitch=0,kp_angle_yaw=0,  kp_gyro_yaw=0,
//			ki_angle_roll=0, ki_gyro_roll=0,ki_angle_pitch=0,ki_gyro_pitch=0,ki_angle_yaw=0,  ki_gyro_yaw=0,
//			kd_angle_roll=0, kd_gyro_roll=0,kd_angle_pitch=0,kd_gyro_pitch=0,kd_angle_yaw=0,  kd_gyro_yaw=0,
//			kp_height=0,ki_height=0,kd_height=0,tar_height=0;
//			printf("Throttle、PID、height have been reset.\n");
//			break;
//		case 0xb1:																														//角度补偿重置
//			offsetAngRoll=0,offsetAngPitch=0,offsetAngYaw=0;
//			printf("All angleOffsets have been reset.\n");
//			break;
//		case 0xc0:kp_angle_roll+=0.1f;kp_angle_pitch+=0.1f;printf ("Now kp_angle_roll is %.2f   kp_angle_pitch is %.2f.\r\n",kp_angle_roll,kp_angle_pitch);break;
//		case 0xc1:kp_angle_roll-=0.1f;kp_angle_pitch-=0.1f;printf ("Now kp_angle_roll is %.2f   kp_angle_pitch is %.2f.\r\n",kp_angle_roll,kp_angle_pitch);break;
//		case 0xc2:ki_angle_roll+=0.1f;ki_angle_pitch+=0.1f;printf ("Now ki_angle_roll is %.2f   ki_angle_pitch is %.2f.\r\n",ki_angle_roll,ki_angle_pitch);break;
//		case 0xc3:ki_angle_roll-=0.1f;ki_angle_pitch-=0.1f;printf ("Now ki_angle_roll is %.2f   ki_angle_pitch is %.2f.\r\n",ki_angle_roll,ki_angle_pitch);break;
//		case 0xc4:kp_gyro_roll+=1.0f;kp_gyro_pitch+=1.0f;printf ("Now kp_gyro_roll is %.2f   kp_gyro_pitch is %.2f.\r\n",kp_gyro_roll,kp_gyro_pitch);break;
//		case 0xc5:kp_gyro_roll-=1.0f;kp_gyro_pitch-=1.0f;printf ("Now kp_gyro_roll is %.2f   kp_gyro_pitch is %.2f.\r\n",kp_gyro_roll,kp_gyro_pitch);break;
//		case 0xc6:ki_gyro_roll+=0.1f;ki_gyro_pitch+=0.1f;printf ("Now ki_gyro_roll is %.2f   ki_gyro_pitch is %.2f.\r\n",ki_gyro_roll,ki_gyro_pitch);break;
//		case 0xc7:ki_gyro_roll-=0.1f;ki_gyro_pitch-=0.1f;printf ("Now ki_gyro_roll is %.2f   ki_gyro_pitch is %.2f.\r\n",ki_gyro_roll,ki_gyro_pitch);break;
//		case 0xc8:kd_gyro_roll+=0.01f;kd_gyro_pitch+=0.01f;printf ("Now kd_gyro_roll is %.2f   kd_gyro_pitch is %.2f.\r\n",kd_gyro_roll,kd_gyro_pitch);break;
//		case 0xc9:kd_gyro_roll-=0.01f;kd_gyro_pitch-=0.01f;printf ("Now kd_gyro_roll is %.2f   kd_gyro_pitch is %.2f.\r\n",kd_gyro_roll,kd_gyro_pitch);break;
//	
//	
//offsetRoll:0.0  offsetPitch:0.0 
//kp_roll:2.50  ki_roll:0.0000  kp_g_roll:3.00  ki_g_roll:0.04  kd_g_roll:1.10
//kp_pitch:1.80  ki_pitch:0.0000  kp_g_pitch:2.80  ki_g_pitch:0.02  kd_g_pitch:1.10
//kp_yaw:0.00  ki_yaw:0.0000  kp_g_yaw:0.00  ki_g_yaw:0.00  kd_g_yaw:0.00
//kp_h:2.80  ki_h:0.1000  kd_h:1.00
//kp_x:2.00  kp_y:2.00  ki_x:0.00  ki_y:0.00}
EEPROMMassErase();
saveEEPROMData();
}
void UART1_Handler(void)																									//JY901数据接收、处理函数
{
		u32 ITStatus;  
		u8  temp_data;
		ITStatus=ROM_UARTIntStatus(UART1_BASE,true);																//得到哪个中断标志位
		ROM_UARTIntClear(UART1_BASE,ITStatus);																	//清除中断标志位
		temp_data=ROM_UARTCharGetNonBlocking(UART1_BASE);
		JY901_GetOneByte(temp_data);
}



void UART5_Handler(void)
{
		u32 ITStatus; 
		u8  temp_data;
		ITStatus=ROM_UARTIntStatus(UART5_BASE,1);	//得到哪个中断标志位
		ROM_UARTIntClear(UART5_BASE,ITStatus);		//清除中断标志位
		temp_data=ROM_UARTCharGetNonBlocking(UART5_BASE);		//取出串口中的数据，无论数据是否用到，此句必需，作用猜测为清空接收缓冲区
		AnoOF_GetOneByte(temp_data);
}
