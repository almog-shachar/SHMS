/*
 * collect.h
 *
 *  Created on: 28 бреб 2015
 *      Author: Titan
 */

#ifndef COLLECT_H_
#define COLLECT_H_

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

unsigned int GPS_request;
unsigned int Xpos_request;
unsigned int Ypos_request;
unsigned int Zpos_request;
unsigned int Xval_request;
unsigned int Yval_request;
unsigned int Zval_request;
unsigned int GPSTime_request;
unsigned int AX100_request;
unsigned int Sun_Sensor_request;
unsigned int OCM_request;
unsigned int STX_request;
unsigned int RW_request;
unsigned int safe_mode;
unsigned int gps_off;

#define UART_A 0
#define UART_B 1
#define UART_C 2


void rawMode(int dev);
void ReverseInt(int *val);
int set_uart_attribs(int fd, int speed, int parity);
void set_uart_settings(int fd);
void request_info(int dev, int * GPS_info, unsigned int request);
rtems_task Task_Collect(rtems_task_argument);




#endif /* COLLECT_H_ */
