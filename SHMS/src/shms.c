/*
 * Health_Monitoring_Task.c
 *
 *    Authors: Almog,Ofri
 *
 */

#include "shms.h"
#include "process.h"



rtems_id   Task_id[ 4 ];         /* array of task ids */
rtems_name Task_name[ 4 ];       /* array of task names */

rtems_task Init( rtems_task_argument ignored)
{

	  rtems_time_of_day time;
	  int i;
	  time.year   = 2015;
	  time.month  = 8;
	  time.day    = 18;
	  time.hour   = 11;
	  time.minute = 25;
	  time.second = 15;
	  time.ticks  = 0;
	  status = rtems_clock_set( &time );

	  createq(); // initialize the processing queue
	  for(i=0; i < N_COMPS; i++)
	  {
		  irrisponsive[i] = 0;
		  monitoredc[i] = 1;
	  }


	  Task_name[ 1 ] = rtems_build_name( 'C', 'L', 'C', 'T' ); // collect
	  Task_name[ 2 ] = rtems_build_name( 'P', 'R', 'C', 'S' ); // process
	  Task_name[ 3 ] = rtems_build_name( 'M', 'T', 'G', 'T' ); // mitigate

	  status = rtems_task_create(
		Task_name[ 1 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
		RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 1 ]
	  );
	  status = rtems_task_create(
		Task_name[ 2 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
		RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 2 ]
	  );
	  status = rtems_task_create(
		Task_name[ 3 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES, // rtems_create task
		RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 3 ]                               // name, priority, stack_size, initial_modes, attribute_set, *id
	  );

	  status = rtems_task_start( Task_id[ 1 ], Task_Collect, 1 );
	  status = rtems_task_start( Task_id[ 2 ], Task_Process, 2 );
	  //status = rtems_task_start( Task_id[ 3 ], Monitoring_task, 3 );

	  status = rtems_task_delete( RTEMS_SELF );
}



