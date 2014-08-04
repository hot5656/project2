
#include	<stdio.h>
#include	"M051Series.h"
#include	"M051.h"
#include	"sgpio.h"
#include 	"i2c_slave.h" // kyp(2013.3.13) : add i2c slave

#include "string.h"


// kyp(2013.3.13) : add i2c slave
U8 i2c_data[I2C_REG_MAX];

void work_led(U32 time);


int main(void)
{
	Sysclk_init();       				// SYSTEM CLOCK InitFunction (MUST)

	SysTick_Config(SYSCK_HZ/1000);		// SysTick 1ms InitFunction  (MUST)

	//	Time0_init(1);						// TIME0 1ms InitFunction    (MUST)

	Watchdog_init() ;        			// Watchdog InitFunction     (MUST)  RUN TIME CALL Watchdog()

	// disbel buzzer
	P3_7 = 0 ;

	SGPIO_P4_init(0);					// SGPIO EINT0 interrupt InitFunction
	// kyp(2013.3.13) : add i2c slave
	// i2c slave init
	I2C_S_init(2,i2c_data) ;

	// set i2c_data default
	// fan temp
	i2c_data[0x10] = 20 ;
	i2c_data[0x11] = 30 ;
	i2c_data[0x12] = 40 ;
	i2c_data[0x13] = 50 ;
	// set PWM out
	i2c_data[0x14] = 80 ;

	// fan RPM
	i2c_data[0x19] = 10 ;
	i2c_data[0x1B] = 20 ;
	i2c_data[0x1D] = 30 ;
	i2c_data[0x1F] = 40 ;
	i2c_data[0x21] = 50 ;
	i2c_data[0x23] = 60 ;
	i2c_data[0x25] = 70 ;
	i2c_data[0x27] = 80 ;
	// set fan enable
	i2c_data[0x28] = 0x0f ;
	// set device enable
	i2c_data[0x29] = 0x9e ;
	// set version
	strcpy((char*) &i2c_data[0x80],"demo LED V0.1" ) ;


	while(1)
    {
		work_led(300);
		sgpio_fled();
    }
}




void work_led(U32 time)
{
	static U32 WORK_Time = 0;
	U32 NOW_Time  = ms_cnt();

	if ((NOW_Time - WORK_Time) >= time ) {
		WORK_Time = NOW_Time;

		// kyp(2013.3.13) : add i2c slave
		// i2s slave recover protect
		I2C_S_check();

		Watchdog();
		P4_4 = ~P4_4 ;
	}
}




