/*
 * collect.h
 *
 *  Created on: 28 бреб 2015
 *      Author: Titan
 */

#ifndef COLLECT_H_
#define COLLECT_H_
/*-------------------------------*/
rtems_name collect_GPS_name;
rtems_name init_GPS_name;
rtems_name collect_SUN_S_name;
rtems_name init_SUN_S_name;
rtems_name collect_RW_name;
rtems_name init_RW_name;
rtems_name collect_STX_name;
rtems_name init_STX_name;
rtems_name collect_OCM_name;
rtems_name init_OCM_name;
rtems_name collect_UHF_name;
rtems_name init_UHF_name;
/*-------------------------------*/

/*-------------------------------*/
rtems_id init_GPS_id;
rtems_id init_SUN_S_id;
rtems_id collect_GPS_id;
rtems_id collect_SUN_S_id;
rtems_id init_RW_id;
rtems_id collect_RW_id;
rtems_id init_STX_id;
rtems_id collect_STX_id;
rtems_id init_OCM_id;
rtems_id collect_OCM_id;
rtems_id init_UHF_id;
rtems_id collect_UHF_id;
/*-------------------------------*/
unsigned int GPS_request;
	unsigned int Xpos_request;
	unsigned int Ypos_request;
	unsigned int Zpos_request;
	unsigned int Xval_request;
	unsigned int Yval_request;
	unsigned int Zval_request;
	unsigned int GPSTime_request;
	unsigned int gps_off;
/*-------------------------------*/

/*-------------------------------*/
unsigned int Sun_Sensor_request;
	unsigned int fit_quality_request;
	unsigned int geometry_quality_request;
/*-------------------------------*/

/*-------------------------------*/
unsigned int UHF_request;
	unsigned int V33_current_request;
	unsigned int V33_voltage_request;
	unsigned int V5_current_request;
	unsigned int V5_voltage_request;
	unsigned int SMPS_temp_request;
	unsigned int PA_temp_request;
	unsigned int RSSI_request;
/*-------------------------------*/

/*-------------------------------*/
unsigned int OCM_request;
	unsigned int sma_request;
	unsigned int ecc_request;
	unsigned int inc_request;
	unsigned int arg_request;
	unsigned int raan_request;
	unsigned int tra_request;
	unsigned int sma_mean_request;
	unsigned int ecc_mean_request;
	unsigned int inc_mean_request;
	unsigned int arg_mean_request;
	unsigned int raan_mean_request;
	unsigned int true_mean_an_request;
	unsigned int R_alpha_beta_vector_request;
	unsigned int R_beta_gamma_vector_request;
	unsigned int R_gamma_alpha_vector_request;
	unsigned int V_alpha_beta_vector_request;
	unsigned int V_beta_gamma_vector_request;
	unsigned int V_gamma_alpha_vector_request;
/*-------------------------------*/

/*-------------------------------*/
unsigned int STX_request;
	unsigned int B_Current_W_eigth_RF_request;
	unsigned int B_Current_W_quarter_RF_request;
	unsigned int B_Current_W_half_RF_request;
	unsigned int B_Current_W_1_RF_request;
	unsigned int B_voltage_request;
	unsigned int PA_current_request;
	unsigned int PA_voltage_request;
	unsigned int PA_temp_request;
	unsigned int RF_out_power_request;
	unsigned int top_b_temp_request;
	unsigned int bot_b_temp_request;
/*-------------------------------*/

/*-------------------------------*/
unsigned int RW_request;
	unsigned int D_chip_temp_request;
	unsigned int D_chip_current_request;
	unsigned int D_chip_voltage_request;
/*-------------------------------*/

unsigned int safe_mode;

#define UART_A 0
#define UART_B 1
#define UART_C 2
rtems_task Task_Init_GPS();
rtems_task Task_Collect_GPS ();
rtems_task Task_Init_SUN_S();
rtems_task Task_Collect_SUN_S();
rtems_task Task_Init_RW();
rtems_task Task_Collect_RW ();
rtems_task Task_Init_STX();
rtems_task Task_Collect_STX ();
rtems_task Task_Collect_OCM();
rtems_task Task_Init_UHF();
rtems_task Task_Collect_UHF();

enum res request_info(int, int* , unsigned int);



#endif /* COLLECT_H_ */
