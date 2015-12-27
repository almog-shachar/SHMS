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
				preocess_sun_s((SUN_S_struct*)pr);
				break;
			case RW:
				process_rw((RW_struct*)pr);
				break;
			case STX:
				process_stx((STX_struct*)pr);
				break;
			case UHF:
				process_uhf((UHF_struct*)pr);
				break;
			case OCM:
				process_ocm((OCM_struct*)pr);
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
	if (SUN_S_data == NULL)	// SUN_S_data is set to null means that no data could be extracted from SUN_S component.
			CBR_val = CBR_val | 3; //set the proper sun_s cbr bit on.
	else
	{
		if(SUN_S_data->fit_quality<0)
		{
			CBR_sun_s = CBR_sun_s | 1;
			CBR_sun_s = CBR_sun_s | 2;
		}
		if(SUN_S_data->geometry_quality<0)
		{
			CBR_sun_s = CBR_sun_s | 1;
			CBR_sun_s = CBR_sun_s | 3;
		}
	}
		CBR_case_resolver();	//find the matching case.
}

void process_rw(RW_struct* RW_data)
{
	if (RW_data == NULL)	// RW_data is set to null means that no data could be extracted from RW component.

			CBR_val = CBR_val | 4; //set the proper rw cbr bit on.
	else
	{
		if(RW_data->D_chip_temp>150)
		{
			CBR_rw = CBR_rw | 1;
			CBR_rw = CBR_rw | 2;
		}
		if(RW_data->D_chip_current>3)
		{
			CBR_rw = CBR_rw | 1;
			CBR_rw = CBR_rw | 3;
		}
		if(RW_data->D_chip_voltage<8)
		{
			CBR_rw = CBR_rw | 1;
			CBR_rw = CBR_rw | 4;
		}
	}
		CBR_case_resolver();	//find the matching case.
}

void process_uhf(UHF_struct* UHF_data)
{
	if (UHF_data == NULL)	// UHF_data is set to null means that no data could be extracted from UHF component.
			CBR_val = CBR_val | 5; //set the proper uhf cbr bit on.
	else
	{
		if(UHF_data->V33_current>70 || UHF_data->V33_current<50)
		{
			CBR_uhf = CBR_uhf | 1;
			CBR_uhf = CBR_uhf | 2;
		}

		if(UHF_data->V33_voltage>5 || UHF_data->V33_voltage <2)
		{
			CBR_uhf = CBR_uhf | 1;
			CBR_uhf = CBR_uhf | 3;
		}
		if(UHF_data->V5_current>30)
		{
			CBR_uhf = CBR_uhf | 1;
			CBR_uhf = CBR_uhf | 4;
		}
		if(UHF_data->V5_voltage>15 || UHF_data->V5_voltage <7)
		{
			CBR_uhf = CBR_uhf | 1;
			CBR_uhf = CBR_uhf | 5;
		}
		if(UHF_data->SMPS_temp>85 || UHF_data->SMPS_temp<-45)
		{
			CBR_uhf = CBR_uhf | 1;
			CBR_uhf = CBR_uhf | 6;
		}
		if(UHF_data->RSSI>25 || UHF_data->RSSI<3)
		{
			CBR_uhf = CBR_uhf | 1;
			CBR_uhf = CBR_uhf | 7;
		}
	}
		CBR_case_resolver();	//find the matching case.
}

void process_ocm(OCM_struct* OCM_data)
{
	if (OCM_data == NULL)	// OCM_data is set to null means that no data could be extracted from OCM component.
			CBR_val = CBR_val | 6; //set the proper ocm cbr bit on.
	else
	{
		if(OCM_data->sma>7200 || OCM_data->sma<6700)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 2;
		}
		if(OCM_data->ecc>0.1 || OCM_data->ecc<0)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 3;
		}
		if(OCM_data->inc>3.14 || OCM_data->inc<0)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 4;
		}
		if(OCM_data->arg>(3.14*2) || OCM_data->arg<0)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 5;
		}
		if(OCM_data->raan>(3.14*2) || OCM_data->raan<0)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 6;
		}
		if(OCM_data->tra>(3.14*2) || OCM_data->tra<0)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 7;
		}
		if(OCM_data->sma_mean>7200 || OCM_data->sma<6700)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 8;
		}
		if(OCM_data->ecc_mean>0.1 || OCM_data->ecc_mean<0)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 9;
		}
		if(OCM_data->inc_mean>3.14 || OCM_data->inc_mean<0)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 10;
		}
		if(OCM_data->arg_mean>(3.14*2) || OCM_data->arg_mean<0)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 11;
		}
		if(OCM_data->raan_mean>(3.14*2) || OCM_data->raan_mean<0)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 12;
		}
		if(OCM_data->true_mean_an>(3.14*2) || OCM_data->true_mean_an<0)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 13;
		}
		if(OCM_data->R_alpha_beta_vector>10000 || OCM_data->R_alpha_beta_vector<-10000)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 14;
		}
		if(OCM_data->R_beta_gamma_vector>10000 || OCM_data->R_beta_gamma_vector<-10000)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 15;
		}
		if(OCM_data-> R_gamma_alpha_vector>10000 || OCM_data-> R_gamma_alpha_vector<-10000)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 16;
		}
		if(OCM_data->V_alpha_beta_vector>10 || OCM_data->V_alpha_beta_vector<-10)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 17;
		}
		if(OCM_data->V_beta_gamma_vector>10 || OCM_data->V_beta_gamma_vector<-10)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 18;
		}
		if(OCM_data->V_gamma_alpha_vector>10 || OCM_data->V_gamma_alpha_vector<-10)
		{
			CBR_ocm = CBR_ocm | 1;
			CBR_ocm = CBR_ocm | 19;
		}
	}
		CBR_case_resolver();	//find the matching case.
}

void process_stx(STX_struct* STX_data)
{
	if (STX_data == NULL)	// STX_data is set to null means that no data could be extracted from STX component.
			CBR_val = CBR_val | 7; //set the proper stx cbr bit on.
	else
	{
		if(STX_data->B_Current_W_eigth_RF!=31)
		{
			CBR_stx = CBR_stx | 1;
			CBR_stx = CBR_stx | 2;
		}
		if(STX_data->B_Current_W_quarter_RF!=42)
		{
			CBR_stx = CBR_stx | 1;
			CBR_stx = CBR_stx | 3;
		}
		if(STX_data->B_Current_W_half_RF!=55)
		{
			CBR_stx = CBR_stx | 1;
			CBR_stx = CBR_stx | 4;
		}
		if(STX_data->B_Current_W_1_RF!=83)
		{
			CBR_stx = CBR_stx | 1;
			CBR_stx = CBR_stx | 5;
		}
		if(STX_data->B_voltage>12 || STX_data->B_voltage<6)
		{
			CBR_stx = CBR_stx | 1;
			CBR_stx = CBR_stx | 6;
		}
		if(STX_data->PA_current>83 || STX_data->PA_current<31)
		{
			CBR_stx = CBR_stx | 1;
			CBR_stx = CBR_stx | 7;
		}
		if(STX_data->PA_voltage!=14)
		{
			CBR_stx = CBR_stx | 1;
			CBR_stx = CBR_stx | 8;
		}
		if(STX_data->PA_temp>85 || STX_data->PA_temp<-40)
		{
			CBR_stx = CBR_stx | 1;
			CBR_stx = CBR_stx | 9;
		}
		if(STX_data->RF_out_power>27 || STX_data->RF_out_power<23)
		{
			CBR_stx = CBR_stx | 1;
			CBR_stx = CBR_stx | 10;
		}
		if(STX_data->top_b_temp>61 || STX_data->top_b_temp<-25)
		{
			CBR_stx = CBR_stx | 1;
			CBR_stx = CBR_stx | 11;
		}
		if(STX_data->bot_b_temp>61 || STX_data->bot_b_temp<-25)
		{
			CBR_stx = CBR_stx | 1;
			CBR_stx = CBR_stx | 12;
		}
	}
		CBR_case_resolver();	//find the matching case.
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
