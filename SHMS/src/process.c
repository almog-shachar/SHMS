#include "shms.h"
#include "process.h"

rtems_task Task_Process( rtems_task_argument unused )
{
	pnode* pr;

	while(1)
	{
		if(pqh == NULL)	// if nothing to process, wait for 5 seconds.
			sleep(5);
		pr = deq();
		switch( pr->cid )
		{
			case GPS1:
				process_gps((GPS_struct*)pr->data,1);
				break;
			case GPS2:
				process_gps((GPS_struct*)pr->data,2);
				break;
			case SUN_S:
				preocess_sun_s(pr);
				break;
			case RW:
				process_rw(pr);
				break;
			case STX:
				process_stx(pr);
				break;
			case UHF:
				process_uhf(pr);
				break;
			case OCM:
				process_ocm(pr);
				break;
		}
	}
}


void process_gps(GPS_struct* GPS_data,int gpsnum)
{
	if(irrisponsive[GPS1])	// gps1 is not responding (communication or data retrieval error)
	{

	}
	else if (GPS_data == NULL)	// some other malfunctioning test related to the gps data
	{

	}
}



/* processing queue management functions. */
void createq()
{
	pqh = pqt = NULL;
}

void enq(pnode* elem)
{
	if( pqh == NULL )
		pqh = pqt = elem;
	else
	{
		pqt->next = elem;
		pqt = pqt->next;
	}
}

pnode* deq()
{
	pnode* pt = pqh;
	pqh = pqh->next;
	pt->next = NULL;
	return pt;
}

void emptyq()
{
	pnode* pt;
	while(pqh != NULL)
	{
		pt = pqh;
		pqh = pqh->next;
		pt->next = NULL;
		free(pt);
	}
	pqt = NULL;
}

pnode* init_node(int cid, void* data)
{
	pnode node = (pnode) malloc (sizeof(pnode));
	node.cid = cid;
	node.data = data;
	node.next = NULL;

	return (&node);

}
