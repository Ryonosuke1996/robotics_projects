#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>
#include "leds.h"
#include "chprintf.h"
#include "usbcfg.h"
#include "spi_comm.h"
#include "sensors/proximity.h"
#include "motors.h"





messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

int main(void)
{
	messagebus_init(&bus, &bus_lock, &bus_condvar);
// initialisation
     halInit();
     chSysInit();
     mpu_init();
	 clear_leds();
	 spi_comm_start();
	 motors_init();


		unsigned int prox[9];
		unsigned int op=1;
		unsigned int v=700;
		unsigned int vel_left ,vel_right,vel_left1,vel_left2,vel_left3,vel_left4,vel_right1,vel_right2,vel_right3,vel_right4;
		unsigned int threshold1=1;
		unsigned int threshold2=100;

		usb_start();
		proximity_start();
		calibrate_ir();








//sensing and actuation

    /* Infinite loop. */
    while (1) {
    	//waits 1 second
    	chThdSleepMilliseconds(100);
		if (op==0){

					// set the RGB leds TO GREEN
					set_rgb_led(LED2,0,10,0);
					set_rgb_led(LED4,0,10,0);
					set_rgb_led(LED6,0,10,0);
					set_rgb_led(LED8,0,10,0);
					set_front_led(1);

				prox[0]=get_calibrated_prox(0);
				chThdSleepMilliseconds(100);
				prox[1]=get_calibrated_prox(1);
				chThdSleepMilliseconds(100);
				prox[2]=get_calibrated_prox(2);
				chThdSleepMilliseconds(100);
				prox[3]=get_calibrated_prox(3);
				chThdSleepMilliseconds(100);
				prox[4]=get_calibrated_prox(4);
				chThdSleepMilliseconds(100);
				prox[5]=get_calibrated_prox(5);
				chThdSleepMilliseconds(100);
				prox[6]=get_calibrated_prox(6);
				chThdSleepMilliseconds(100);
				prox[7]=get_calibrated_prox(7);
				chThdSleepMilliseconds(100);
				if (SDU1.config->usbp->state==USB_ACTIVE) {chprintf((BaseSequentialStream*)&SDU1, "%4d,", prox[0]);}



			 if(prox[0]>=threshold1 ||prox[1]>=threshold1 || prox[2]>=threshold1|| prox[3]>=threshold1 || prox[4]>=threshold1 || prox[5]>=threshold1 || prox[6]>=threshold1 || prox[7]>=threshold1)
				{
				if ( prox[0]|| prox[7]>0){
					vel_left= 0.5*v;
					vel_right=-0.5*v;
					}
				if (prox[6] ||prox[5] || prox[4]>0){
					vel_left=0.5*v;
					vel_right=0.1*v;
					}

				if ( prox[1] || prox[2] || prox[3]>0){
					vel_left = 0.1*v;
					vel_right = 0.5*v;
					}


				}
			else{
					vel_left = 0.5*v;
					vel_right = 0.5*v;
				}

				vel_left=vel_left1+vel_left2+vel_left3+vel_left4;
				vel_right=vel_right1+vel_right2+vel_right3+vel_right4;
				left_motor_set_speed(vel_left);
				right_motor_set_speed(vel_right);
			}
		else{
				set_rgb_led(LED2,0,0,10);
				set_rgb_led(LED4,0,0,10);
				set_rgb_led(LED6,0,0,10);
				set_rgb_led(LED8,0,0,10);
				set_front_led(1);
				prox[0]=get_calibrated_prox(0);
				chThdSleepMilliseconds(100);
				prox[1]=get_calibrated_prox(1);
				chThdSleepMilliseconds(100);
				prox[2]=get_calibrated_prox(2);
				chThdSleepMilliseconds(100);
				prox[3]=get_calibrated_prox(3);
				chThdSleepMilliseconds(100);
				prox[4]=get_calibrated_prox(4);
				chThdSleepMilliseconds(100);
				prox[5]=get_calibrated_prox(5);
				chThdSleepMilliseconds(100);
				prox[6]=get_calibrated_prox(6);
				chThdSleepMilliseconds(100);
				prox[7]=get_calibrated_prox(7);
				chThdSleepMilliseconds(100);

				  if (prox[0]+prox[1]+prox[2]+prox[3]>0){
					  vel_left= v;
					  vel_right=0.5*v;

				  }
				  if (prox[7]+prox[6]+prox[5]+prox[4]>0) {
				  					  vel_left= 0.5*v;
				  					  vel_right=v;}
				  if (prox[0]+prox[1]+prox[2]+prox[3]+prox[7]+prox[6]+prox[5]+prox[4]>0){
					  	  	  	  	 vel_left  = 0;
					  		         vel_right = 0;}

			 else
			    { vel_left= 0;
				  vel_right=0;
			    }


			 				left_motor_set_speed(vel_left);
			 				right_motor_set_speed(vel_right);


		}

    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}