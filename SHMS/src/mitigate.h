/*
 * mitigate.h
 *
 *  Created on: 12 בדצמ 2015
 *      Author: Titan
 */

#ifndef MITIGATE_H_
#define MITIGATE_H_

void CBR_case_resolver();	// resolves the closest case available.
rtems_task Task_Mitigate( rtems_task_argument unused );
enum res reset_gps(int);
enum res reset_rw();
enum res reset_ocm();
enum res reset_stx();
enum res reset_uhf();
enum res reset_sun_s();

typedef struct {
	int CBR_val;
	enum res (*mitigate_func)();
}CBR_task;

int CBR_val = 0;



#endif /* MITIGATE_H_ */
