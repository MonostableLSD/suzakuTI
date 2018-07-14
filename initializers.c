#include "userDefine.h"

u8 All_init()
{	
	initClock();
	initSysTick();
	initLED();
	initPWM();	
	initKey();
	initEEPROM();	
	initFlag();
	initUART();
	blueLed();//Blue LED means "initAll is ok"
	return 1;
}

void initClock()		//设置系统时钟 200MHz/SYSCTL_SYSDIV_2_5=80MHz,PLL代表200MHz,OSC代表16MHz
{
	ROM_FPUEnable(); //??
	
	ROM_FPULazyStackingEnable();    // Enable lazy stacking for interrupt handlers.  This allows floating-point instructions to be used within interrupt handlers, 
																	// but at the expense of extra stack usage.										
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |  SYSCTL_OSC_MAIN);    
	
	ROM_IntMasterEnable(); //中断总使能
	
}

void initSysTick()
{
	ROM_SysTickPeriodSet(ROM_SysCtlClockGet()/SysTickPeriod);
	ROM_SysTickIntEnable();
	SysTickIntRegister(SysTickIntHandler);//register systick interrupts
	ROM_SysTickEnable();
}

void initLED()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);					// Enable the GPIO port that is used for the on-board LED.
	while(!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			// Check if the peripheral access is enabled.
	
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    // Enable the GPIO pin for the LED (PF1 PF2 PF3).  Set the direction as output, and enable the GPIO pin for digital function.
}

void initPWM()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	while(!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));			// Check if the peripheral access is enabled.
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	while(!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));			// Check if the peripheral access is enabled.
	
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_16);
	
	ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);
  ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1); 
	ROM_GPIOPinConfigure(GPIO_PB4_M0PWM2);
  ROM_GPIOPinConfigure(GPIO_PB5_M0PWM3); 
	ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_4|GPIO_PIN_5);
	
	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_0,																							//一个PWM_GEN控制两个PWM
										PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);	
	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_1,
										PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0,20000);
  ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1,20000);
	
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT|PWM_OUT_1_BIT|PWM_OUT_2_BIT| PWM_OUT_3_BIT, true);
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	
}	
void initKey()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);
	ROM_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4,
                       GPIO_STRENGTH_2MA,     
                       GPIO_PIN_TYPE_STD_WPU);
	
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4,
                       GPIO_STRENGTH_2MA,     
                       GPIO_PIN_TYPE_STD_WPU);
}
void initUART()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);		//配置串口0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
	ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), UART0Baud,
													(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
													 UART_CONFIG_PAR_NONE));
	ROM_IntEnable(INT_UART0);
	UARTFIFODisable(UART0_BASE);
	UARTIntRegister(UART0_BASE,UART0_Handler);
	ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

	
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);			//配置串口1 for jy901
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
	ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
	ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), UART1Baud,
													(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
													 UART_CONFIG_PAR_NONE));
	ROM_IntEnable(INT_UART1);
	UARTFIFODisable(UART1_BASE);
	UARTIntRegister(UART1_BASE,UART1_Handler);
	ROM_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);		
	
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);			//配置串口5 for lightflow
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
	ROM_GPIOPinConfigure(GPIO_PE4_U5RX);
	ROM_GPIOPinConfigure(GPIO_PE5_U5TX);
	ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	ROM_UARTConfigSetExpClk(UART5_BASE, ROM_SysCtlClockGet(), UART5Baud,
													(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
													 UART_CONFIG_PAR_NONE));
	ROM_IntEnable(INT_UART5);
	UARTFIFODisable(UART5_BASE);
	UARTIntRegister(UART5_BASE,UART5_Handler);
	ROM_UARTIntEnable(UART5_BASE, UART_INT_RX | UART_INT_RT);		
}

void initEEPROM()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
	while(!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0));			// Check if the peripheral access is enabled.
	ROM_EEPROMInit(); 	
	EEPROMRead((uint32_t *)&e2prom_value,0x0000,sizeof(e2prom_value));
	
		offsetRoll=e2prom_value.offsetRoll;	
		offsetPitch=e2prom_value.offsetPitch;		
		kp_roll=e2prom_value.kp_roll;
		kp_pitch=e2prom_value.kp_pitch;
		kp_yaw=e2prom_value.kp_yaw;
		ki_roll=e2prom_value.ki_roll;
		ki_pitch=e2prom_value.ki_pitch;
		ki_yaw=e2prom_value.ki_yaw;
	
	
		kp_g_roll=e2prom_value.kp_g_roll;
		kp_g_pitch=e2prom_value.kp_g_pitch;
		kp_g_yaw=e2prom_value.kp_g_yaw;
		ki_g_roll=e2prom_value.ki_g_roll;
		ki_g_pitch=e2prom_value.ki_g_pitch;
		ki_g_yaw=e2prom_value.ki_g_yaw;
		kd_g_roll=e2prom_value.kd_g_roll;
		kd_g_pitch=e2prom_value.kd_g_pitch;
		kd_g_yaw=e2prom_value.kd_g_yaw;	

		kp_h=e2prom_value.kp_h;
		ki_h=e2prom_value.ki_h;
		kd_h=e2prom_value.kd_h;

		kp_x=e2prom_value.kp_x;
		kp_y=e2prom_value.kp_y;

		ki_x=e2prom_value.ki_x;
		ki_y=e2prom_value.ki_y;

		kd_x=e2prom_value.kd_x;
		kd_y=e2prom_value.kd_y;
}


void initFlag()
{
	flag.set_yaw=0;
	flag.landing=0;
	flag.ct_att_hold=0;
	flag.ct_alt_hold=0;
	flag.ct_loc_hold=0;
//	flag.angle_ok=0;
//	flag.gyro_ok=0;
//	flag.OF_height_ok=0;
//	flag.OF_loc_ok=0;
	
}

int fputc(int ch, FILE *f)																							//修改可改变printf发送的串口号
{			
			while(ROM_UARTSpaceAvail(UART0_BASE)==false);
			ROM_UARTCharPutNonBlocking(UART0_BASE, (uint8_t) ch);
			return ch;
}

