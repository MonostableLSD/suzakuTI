#include "userDefine.h"


u32 moto1=0,moto2=0,moto3=0,moto4=0;

void motoRefresh(uint32_t moto11,uint32_t moto22,uint32_t moto33,uint32_t moto44)
{
		if(moto11>MAX_PWMPULSE)moto11=MAX_PWMPULSE;															//оч╥Ы			
		if(moto22>MAX_PWMPULSE)moto22=MAX_PWMPULSE;															
		if(moto33>MAX_PWMPULSE)moto33=MAX_PWMPULSE;
		if(moto44>MAX_PWMPULSE)moto44=MAX_PWMPULSE;
		if(moto11<MIN_PWMPULSE)moto11=MIN_PWMPULSE;
		if(moto22<MIN_PWMPULSE)moto22=MIN_PWMPULSE;
		if(moto33<MIN_PWMPULSE)moto33=MIN_PWMPULSE;
		if(moto44<MIN_PWMPULSE)moto44=MIN_PWMPULSE;
	
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, moto11);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, moto22);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, moto33);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, moto44);
}
