/*
 * shms.h
 *
 *  Created on: 15 áðåá 2015
 *      Author: Titan
 */

#ifndef SHMS_H_
#define SHMS_H_

#include <rtems.h>
#include <apbuart.h>
#include <bsp.h> /* for device driver prototypes */
#include <rtems/confdefs.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

rtems_task Init( rtems_task_argument argument);

#define CONFIGURE_INIT
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MAXIMUM_TASKS             4
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)
#define UART_DEVS_IN_SYSTEM 3
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS	20
#define RX_BUF_LEN 4		//in bytes
#define TX_BUF_LEN 4
#define N_COMPS 6

#define put_name( _name, _crlf ) \
  do { \
    uint32_t c0, c1, c2, c3; \
    \
    c0 = ((_name) >> 24) & 0xff; \
    c1 = ((_name) >> 16) & 0xff; \
    c2 = ((_name) >> 8) & 0xff; \
    c3 = (_name) & 0xff; \
    putchar( (char)c0 ); \
    if ( c1 ) putchar( (char)c1 ); \
    if ( c2 ) putchar( (char)c2 ); \
    if ( c3 ) putchar( (char)c3 ); \
    if ( (_crlf) ) \
      putchar( '\n' ); \
  } while (0)

#define print_time( _tb, _s2) \
  do { \
    printf( "%02d:%02d:%02d   %02d/%02d/%04d%s", \
       (_tb)->hour, (_tb)->minute, (_tb)->second, \
       (_tb)->month, (_tb)->day, (_tb)->year, _s2 ); \
    fflush(stdout); \
  } while ( 0 )

#define Swap4Bytes(val) \
 ( (((val) >> 24) & 0x000000FF) | (((val) >>  8) & 0x0000FF00) | \
   (((val) <<  8) & 0x00FF0000) | (((val) << 24) & 0xFF000000) )

#define task_number( tid ) \
  ( rtems_object_id_get_index( tid ) - \
     rtems_configuration_get_rtems_api_configuration()->number_of_initialization_tasks )



enum monitored_components {GPS1,GPS2,SUN_S,RW,STX,UHF,OCM};
enum res {SUCCESS,FAILURE};
extern rtems_id   Task_id[ 4 ];         /* array of task ids */
extern rtems_name Task_name[ 4 ];       /* array of task names */

volatile enum monitored_components monitoredc[N_COMPS];
volatile enum monitored_components irrisponsive[N_COMPS];

typedef struct{
	int Xpos;
	int Ypos;
	int Zpos;
	int Xval;
	int Yval;
	int Zval;
	int GPSTime;
	int ready;
} GPS_struct;

typedef struct{
	int V33_current;
	int V33_voltage;
	int V5_current;
	int V5_voltage;
	int SMPS_temp;
	int PA_temp;
	int RSSI;
}UHF_struct;

typedef struct{
  	int sma;
  	int ecc;
  	int inc;
  	int arg;
  	int raan;
  	int tra;
  	int sma_mean;
  	int ecc_mean;
  	int inc_mean;
  	int arg_mean;
  	int raan_mean;
  	int true_mean_an;
  	int R_alpha_beta_vector;
  	int R_beta_gamma_vector;
  	int R_gamma_alpha_vector;
  	int V_alpha_beta_vector;
  	int V_beta_gamma_vector;
  	int V_gamma_alpha_vector;
}OCM_struct;

typedef struct{
	int B_Current_W_eigth_RF;
	int B_Current_W_quarter_RF;
	int B_Current_W_half_RF;
	int B_Current_W_1_RF;
	int B_voltage;
	int PA_current;
	int PA_voltage;
	int PA_temp;
	int RF_out_power;
	int top_b_temp;
	int bot_b_temp;

}STX_struct;

typedef struct{
	int fit_quality;
	int geometry_quality;
}SUN_S_struct;

typedef struct{
	int D_chip_temp;
	int D_chip_current;
	int D_chip_voltage;
}RW_struct;


#endif /* SHMS_H_ */

