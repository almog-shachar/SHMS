#include "process.h"
#include "mitigate.h"

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
	if (GPS_data == NULL)	// GPS_data is set to null means that no data could be extracted from GPS component.
	{
		if(gpsnum == GPS1)
		{
			CBR_val = CBR_val | 1; //set the proper gps cbr bit on.
		}
		else if(gpsnum == GPS2)
		{
			CBR_val = CBR_val | 2; //set the proper gps cbr bit on.
		}
	}

	else
	{
		if(gpsnum == GPS1)
		{
			if(GPS_data->Xpos>80 || GPS_data->Xpos<20)
			{
				CBR_gps1 = CBR_gps1 | 1;
				CBR_gps1 = CBR_gps1 | 2;
			}

			if(GPS_data->Ypos>90 || GPS_data->Ypos <19)
			{
				CBR_gps1 = CBR_gps1 | 1;
				CBR_gps1 = CBR_gps1 | 3;
			}
			if(GPS_data->Zpos>85 || GPS_data->Zpos <22)
			{
				CBR_gps1 = CBR_gps1 | 1;
				CBR_gps1 = CBR_gps1 | 4;
			}
			if(GPS_data->Xval>50 || GPS_data->Xval <12)
			{
				CBR_gps1 = CBR_gps1 | 1;
				CBR_gps1 = CBR_gps1 | 5;
			}
			if(GPS_data->Yval>55 || GPS_data->Yval<10)
			{
				CBR_gps1 = CBR_gps1 | 1;
				CBR_gps1 = CBR_gps1 | 6;
			}
			if(GPS_data->Zval>70 || GPS_data->Zval<18)
			{
				CBR_gps1 = CBR_gps1 | 1;
				CBR_gps1 = CBR_gps1 | 7;
			}
		}
		else if(gpsnum == GPS2)
		{
			if(GPS_data->Xpos>80 || GPS_data->Xpos<20)
			{
				CBR_gps2 = CBR_gps2 | 1;
				CBR_gps2 = CBR_gps2 | 2;
			}

			if(GPS_data->Ypos>90 || GPS_data->Ypos <19)
			{
				CBR_gps2 = CBR_gps2 | 1;
				CBR_gps2 = CBR_gps2 | 3;
			}
			if(GPS_data->Zpos>85 || GPS_data->Zpos <22)
			{
				CBR_gps2 = CBR_gps2 | 1;
				CBR_gps2 = CBR_gps2 | 4;
			}
			if(GPS_data->Xval>50 || GPS_data->Xval <12)
			{
				CBR_gps2 = CBR_gps2 | 1;
				CBR_gps2 = CBR_gps2 | 5;
			}
			if(GPS_data->Yval>55 || GPS_data->Yval<10)
			{
				CBR_gps2 = CBR_gps2 | 1;
				CBR_gps2 = CBR_gps2 | 6;
			}
			if(GPS_data->Zval>70 || GPS_data->Zval<18)
			{
				CBR_gps2 = CBR_gps2 | 1;
				CBR_gps2 = CBR_gps2 | 7;
			}
		}
	}
		CBR_case_resolver();	//find the matching case.
}

void process_sun_s(SUN_S_struct* SUN_S_data)
{

}

void process_rw(RW_struct* RW_data)
{

}

void process_uhf(UHF_struct* UHF_data)
{

}

void process_ocm(OCM_struct* OCM_data)
{

}

void process_stx(STX_struct* STX_data)
{

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
