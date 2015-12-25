/*
 * process.h
 *
 *  Created on: 29 бреб 2015
 *      Author: Titan
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include "shms.h"


/* a node in the process queue*/
typedef struct pnode {
	int cid;
	void* data;
	struct pnode* next;
} pnode;


void enq(pnode*);
pnode* deq();
void emptyq();
void createq();

int n_comps;		// the number of monitored components
pnode *pqh;			// process queue head
pnode *pqt;			// process queue tail
int nq;				// queue size

rtems_task Task_Process( rtems_task_argument);
void process_gps(GPS_struct*,int);
void process_sun_s(pnode*);
void process_rw(pnode*);
void process_uhf(pnode*);
void process_ocm(pnode*);


#endif /* PROCESS_H_ */
