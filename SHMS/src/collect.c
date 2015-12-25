/*
 * collect.c
 *
 *  Created on: 28 бреб 2015
 *      Author: Titan
 */


#include "shms.h"
#include "collect.h"
#include "process.h"

void init_vars()
{
	/*-------------------------------*/
	GPS_request = 2301;
	Xpos_request = 2302;
	Ypos_request = 2303;
	Zpos_request = 2304;
	Xval_request = 2305;
	Yval_request = 2306;
	Zval_request = 2307;
	GPSTime_request = 2308;
	gps_off = 2309;
	/*-------------------------------*/

	/*-------------------------------*/
	UHF_request = 2401;
	V33_current_request = 2402;
	V33_voltage_request = 2403;
	V5_current_request = 2404;
	V5_voltage_request = 2405;
	SMPS_temp_request = 2406;
	PA_temp_request = 2407;
	RSSI_request = 2407;
	/*-------------------------------*/

	/*-------------------------------*/
	Sun_Sensor_request = 2501;
	fit_quality_request = 2502;
	geometry_quality_request = 2503;
	/*-------------------------------*/

	/*-------------------------------*/
	OCM_request = 2601;
	sma_request = 2602;
	ecc_request = 2603;
	inc_request = 2604;
	arg_request = 2605;
	raan_request = 2606;
	tra_request = 2607;
	sma_mean_request = 2608;
	ecc_mean_request = 2609;
	inc_mean_request = 2610;
	arg_mean_request = 26011;
	raan_mean_request = 2612;
	true_mean_an_request = 2613;
	R_alpha_beta_vector_request = 2614;
	R_beta_gamma_vector_request = 2615;
	R_gamma_alpha_vector_request = 2616;
	V_alpha_beta_vector_request = 2617;
	V_beta_gamma_vector_request = 2618;
	V_gamma_alpha_vector_request = 2619;
	/*-------------------------------*/

	/*-------------------------------*/
	STX_request = 2701;
	B_Current_W_eigth_RF_request = 2702;
	B_Current_W_quarter_RF_request = 2703;
	B_Current_W_half_RF_request = 2704;
	B_Current_W_1_RF_request = 2705;
	B_voltage_request = 2706;
	PA_current_request = 2707;
	PA_voltage_request = 2708;
	PA_temp_request = 2709;
	RF_out_power_request = 2710;
	top_b_temp_request = 2711;
	bot_b_temp_request = 2712;
	/*-------------------------------*/

	/*-------------------------------*/
	RW_request = 2801;
	D_chip_temp_request = 2802;
	D_chip_current_request = 2803;
	D_chip_voltage_request = 2804;
	/*-------------------------------*/

	safe_mode = 4401;
	}

typedef struct{

	char* port;								//will be provided with open()
	int fd;									//no desc.
	int speed;								//initialized with default uart speed
	int parity;									//need parity?
	unsigned char rx_buffer[RX_BUF_LEN];
	unsigned char tx_buffer[TX_BUF_LEN];
	int dev_status;									//store op. status for a given device

} uart_dev;


uart_dev uart_devices[UART_DEVS_IN_SYSTEM];

rtems_task Task_Collect( rtems_task_argument unused )
{
	int i;
	init_vars();

	/* monitor all components by default */
	for (i=0; i < N_COMPS; i++)
		monitoredc[i] = 1;

	/* setup communication with GPS  */
	if ( monitoredc[GPS1] )
	{
		init_GPS_name = rtems_build_name( 'G','P','S','I' );
		rtems_task_create(
				init_GPS_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
				RTEMS_DEFAULT_ATTRIBUTES, &init_GPS_id
			  );
		status = rtems_task_start( init_GPS_id, Task_Collect_GPS, 1 );
		/* generate GPS data collection task */
		if ( monitoredc[GPS1] )
		{
			collect_GPS_name = rtems_build_name( 'G','P','S','C' );
			rtems_task_create(
					collect_GPS_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
					RTEMS_DEFAULT_ATTRIBUTES, &collect_GPS_id
				  );
			rtems_task_start( collect_GPS_id, Task_Collect_GPS, 1 );
		}
	}

	/* setup communication with sun sensor  */
	if ( monitoredc[SUN_S] )
	{
		init_SUN_S_name = rtems_build_name( 'S','U','N','I');
		rtems_task_create(
				init_SUN_S_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
				RTEMS_DEFAULT_ATTRIBUTES, &init_SUN_S_id
			  );
		status = rtems_task_start( init_SUN_S_id, Task_Collect_SUN_S, 1 );
		/* generate GPS data collection task */
		if ( monitoredc[SUN_S] )
		{
			collect_SUN_S_name = rtems_build_name( 'S','U','N','C' );
			rtems_task_create(
					collect_SUN_S_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
					RTEMS_DEFAULT_ATTRIBUTES, &collect_SUN_S_id
				  );
			rtems_task_start( collect_SUN_S_id, Task_Collect_SUN_S, 1 );
		}
	}

	/* setup communication with reaction wheel  */
	if ( monitoredc[RW] )
	{
		init_RW_name = rtems_build_name( 'R','W','-','I');
		rtems_task_create(
				init_RW_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
				RTEMS_DEFAULT_ATTRIBUTES, &init_RW_id
			  );
		status = rtems_task_start( init_RW_id, Task_Collect_RW, 1 );
		/* generate GPS data collection task */
		if ( monitoredc[RW] )
		{
			collect_RW_name = rtems_build_name( 'R','W','-','C' );
			rtems_task_create(
					collect_RW_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
					RTEMS_DEFAULT_ATTRIBUTES, &collect_RW_id
				  );
			rtems_task_start( collect_RW_id, Task_Collect_RW, 1 );
		}
	}

	/* setup communication with s-band transmitter  */
	if ( monitoredc[STX] )
	{
		init_STX_name = rtems_build_name( 'S','T','X','I');
		rtems_task_create(
				init_STX_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
				RTEMS_DEFAULT_ATTRIBUTES, &init_STX_id
			  );
		status = rtems_task_start( init_STX_id, Task_Collect_STX, 1 );
		/* generate GPS data collection task */
		if ( monitoredc[STX] )
		{
			collect_STX_name = rtems_build_name( 'S','T','X','C' );
			rtems_task_create(
					collect_STX_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
					RTEMS_DEFAULT_ATTRIBUTES, &collect_STX_id
				  );
			rtems_task_start( collect_STX_id, Task_Collect_STX, 1 );
		}
	}

	/* setup communication with orbit control module  */
	if ( monitoredc[OCM] )
	{
		init_OCM_name = rtems_build_name( 'O','C','M','I');
		rtems_task_create(
				init_OCM_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
				RTEMS_DEFAULT_ATTRIBUTES, &init_OCM_id
			  );
		status = rtems_task_start( init_OCM_id, Task_Collect_OCM, 1 );
		/* generate GPS data collection task */
		if ( monitoredc[OCM] )
		{
			collect_OCM_name = rtems_build_name( 'O','C','M','C' );
			rtems_task_create(
					collect_OCM_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
					RTEMS_DEFAULT_ATTRIBUTES, &collect_OCM_id
				  );
			rtems_task_start( collect_OCM_id, Task_Collect_OCM, 1 );
		}
	}

	/* setup communication with uhf transmitter  */
	if ( monitoredc[UHF] )
	{
		init_UHF_name = rtems_build_name( 'U','H','F','I');
		rtems_task_create(
				init_UHF_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
				RTEMS_DEFAULT_ATTRIBUTES, &init_UHF_id
			  );
		status = rtems_task_start( init_UHF_id, Task_Collect_UHF, 1 );
		/* generate GPS data collection task */
		if ( monitoredc[UHF] )
		{
			collect_UHF_name = rtems_build_name( 'U','H','F','C' );
			rtems_task_create(
					collect_UHF_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
					RTEMS_DEFAULT_ATTRIBUTES, &collect_UHF_id
				  );
			rtems_task_start( collect_UHF_id, Task_Collect_UHF, 1 );
		}
	}

}

rtems_task Task_Init_GPS()
{
	uart_devices[UART_C].port = "/dev/console_c";
	uart_devices[UART_C].fd = open (uart_devices[UART_C].port, O_RDWR);

	if (uart_devices[UART_C].fd < 0)
	{
		printf("Error on open");
		strerror(errno);
		irrisponsive[GPS1] = 1;

	}

	uart_devices[UART_C].speed = B38400;
	uart_devices[UART_C].parity = 0;
	set_uart_attribs(uart_devices[UART_C].fd, uart_devices[UART_C].speed, uart_devices[UART_C].parity);
	set_uart_settings(uart_devices[UART_C].fd);

}

/* collect GPS data every 3 minutes */
rtems_task Task_Collect_GPS ()
{
	GPS_struct* GPS_data;
	while( monitoredc[GPS1] )
	{
		if( (GPS_data = (GPS_struct*) malloc (sizeof(GPS_struct))) != NULL)	// allocate memory for gps data node
		{
			if( 	(request_info(UART_C, &(GPS_data->Xpos), Xpos_request) == FAILURE) ||
					(request_info(UART_C, &(GPS_data->Ypos), Ypos_request) == FAILURE) ||
					(request_info(UART_C, &(GPS_data->Zpos), Zpos_request) == FAILURE) ||
					(request_info(UART_C, &(GPS_data->Xval), Xval_request) == FAILURE) ||
					(request_info(UART_C, &(GPS_data->Yval), Yval_request) == FAILURE) ||
					(request_info(UART_C, &(GPS_data->Zval), Zval_request) == FAILURE) ||
					(request_info(UART_C, &(GPS_data->GPSTime), GPSTime_request) == FAILURE) 	)			// retrieve gps data
					GPS_data = NULL;
			else
				enq((void*)GPS_data); // enqueue for processing
		}
		sleep(180); // next collection phase in 3 minutes.
	}
}

rtems_task Task_Init_SUN_S()
{
	uart_devices[UART_C].port = "/dev/console_c";
	uart_devices[UART_C].fd = open (uart_devices[UART_C].port, O_RDWR);

	if (uart_devices[UART_C].fd < 0)
	{
		printf("Error on open");
		strerror(errno);
		irrisponsive[SUN_S] = 1;

	}

	uart_devices[UART_C].speed = B38400;
	uart_devices[UART_C].parity = 0;
	set_uart_attribs(uart_devices[UART_C].fd, uart_devices[UART_C].speed, uart_devices[UART_C].parity);
	set_uart_settings(uart_devices[UART_C].fd);
}

rtems_task Task_Collect_SUN_S ()
{
	SUN_S_struct* SUN_S_data;
	while( monitoredc[SUN_S] )
	{
		if( (SUN_S_data = (SUN_S_struct*)malloc(sizeof(SUN_S_struct))) != NULL)	// allocate memory for SUN_S data node
		{
			request_info(UART_C, &(SUN_S_data->fit_quality), fit_quality_request);	// retrieve fit data
			request_info(UART_C, &(SUN_S_data->geometry_quality), geometry_quality_request);	// retrieve geometry data
		}
		enq((void*)SUN_S_data); // enqueue for processing
		sleep(180); // next collection phase in 3 minutes.
	}
}

rtems_task Task_Init_RW()
{
	uart_devices[UART_C].port = "/dev/console_c";
	uart_devices[UART_C].fd = open (uart_devices[UART_C].port, O_RDWR);

	if (uart_devices[UART_C].fd < 0)
	{
		printf("Error on open");
		strerror(errno);
		irrisponsive[RW] = 1;

	}

	uart_devices[UART_C].speed = B38400;
	uart_devices[UART_C].parity = 0;
	set_uart_attribs(uart_devices[UART_C].fd, uart_devices[UART_C].speed, uart_devices[UART_C].parity);
	set_uart_settings(uart_devices[UART_C].fd);
}

rtems_task Task_Collect_RW ()
{
	RW_struct* RW_data;
	while( monitoredc[RW] )
	{
		if( (RW_data = (RW_struct*)malloc(sizeof(RW_struct))) != NULL)	// allocate memory for RW data node
		{
			request_info(UART_C, &(RW_data->D_chip_temp), D_chip_temp_request);	// retrieve RW data
			request_info(UART_C, &(RW_data->D_chip_current), D_chip_current_request);
			request_info(UART_C, &(RW_data->D_chip_voltage), D_chip_voltage_request);
		}
		enq((void*)RW_data); // enqueue for processing
		sleep(180); // next collection phase in 3 minutes.
	}
}

rtems_task Task_Init_STX()
{
	uart_devices[UART_C].port = "/dev/console_c";
	uart_devices[UART_C].fd = open (uart_devices[UART_C].port, O_RDWR);

	if (uart_devices[UART_C].fd < 0)
	{
		printf("Error on open");
		strerror(errno);
		irrisponsive[STX] = 1;

	}

	uart_devices[UART_C].speed = B38400;
	uart_devices[UART_C].parity = 0;
	set_uart_attribs(uart_devices[UART_C].fd, uart_devices[UART_C].speed, uart_devices[UART_C].parity);
	set_uart_settings(uart_devices[UART_C].fd);
}

rtems_task Task_Collect_STX ()
{
	STX_struct* STX_data;
	while( monitoredc[STX] )
	{
		if( (STX_data = (STX_struct*) malloc (sizeof(STX_struct))) != NULL)	// allocate memory for STX data node
		{
			request_info(UART_C, &(STX_data->B_Current_W_eigth_RF), B_Current_W_eigth_RF_request);	// retrieve STX data
			request_info(UART_C, &(STX_data->B_Current_W_quarter_RF), B_Current_W_quarter_RF_request);	// retrieve STX data
			request_info(UART_C, &(STX_data->B_Current_W_half_RF), B_Current_W_half_RF_request);	// retrieve STX data
			request_info(UART_C, &(STX_data->B_Current_W_1_RF), B_Current_W_1_RF_request);	// retrieve STX data
			request_info(UART_C, &(STX_data->B_voltage), B_voltage_request);	// retrieve STX data
			request_info(UART_C, &(STX_data->PA_current), PA_current_request);	// retrieve STX data
			request_info(UART_C, &(STX_data->PA_voltage), PA_voltage_request);	// retrieve STX data
			request_info(UART_C, &(STX_data->PA_temp), PA_temp_request);	// retrieve STX data
			request_info(UART_C, &(STX_data->RF_out_power), RF_out_power_request);	// retrieve STX data
			request_info(UART_C, &(STX_data->top_b_temp), top_b_temp_request);	// retrieve STX data
			request_info(UART_C, &(STX_data->bot_b_temp), bot_b_temp_request);	// retrieve STX data
		}
		enq((void*)STX_data); // enqueue for processing
		sleep(180); // next collection phase in 3 minutes.
	}
}



rtems_task Task_Collect_OCM ()
{
	OCM_struct* OCM_data;
	while( monitoredc[OCM] )
	{
		if( (OCM_data = (OCM_struct*) malloc (sizeof(OCM_struct))) != NULL)	// allocate memory for OCM data node
		{
			request_info(UART_C, &(OCM_data->sma), sma_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->ecc), ecc_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->inc), inc_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->arg), arg_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->raan), raan_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->tra), tra_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->sma_mean), sma_mean_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->ecc_mean), ecc_mean_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->inc_mean), inc_mean_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->arg_mean), arg_mean_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->raan_mean), raan_mean_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->true_mean_an), true_mean_an_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->R_alpha_beta_vector), R_alpha_beta_vector_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->R_beta_gamma_vector), R_beta_gamma_vector_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->R_gamma_alpha_vector), R_gamma_alpha_vector_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->V_alpha_beta_vector), V_alpha_beta_vector_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->V_beta_gamma_vector), V_beta_gamma_vector_request);	// retrieve OCM data
			request_info(UART_C, &(OCM_data->V_gamma_alpha_vector), V_gamma_alpha_vector_request);	// retrieve OCM data
		}
		enq((void*)OCM_data); // enqueue for processing
		sleep(180); // next collection phase in 3 minutes.
	}
}

rtems_task Task_Init_UHF()
{
	uart_devices[UART_C].port = "/dev/console_c";
	uart_devices[UART_C].fd = open (uart_devices[UART_C].port, O_RDWR);

	if (uart_devices[UART_C].fd < 0)
	{
		printf("Error on open");
		strerror(errno);
		irrisponsive[UHF] = 1;

	}

	uart_devices[UART_C].speed = B38400;
	uart_devices[UART_C].parity = 0;
	set_uart_attribs(uart_devices[UART_C].fd, uart_devices[UART_C].speed, uart_devices[UART_C].parity);
	set_uart_settings(uart_devices[UART_C].fd);
}

rtems_task Task_Collect_UHF ()
{
	UHF_struct* UHF_data;
	while( monitoredc[UHF] )
	{
		if( (UHF_data = (UHF_struct*)malloc(sizeof(UHF_struct))) != NULL)	// allocate memory for gps data node
		{
			request_info(UART_C, &(UHF_data->V33_current), V33_current_request);	// retrieve gps data
			request_info(UART_C, &(UHF_data->V33_voltage), V33_voltage_request);
			request_info(UART_C, &(UHF_data->V5_current), V5_current_request);
			request_info(UART_C, &(UHF_data->V5_voltage), V5_voltage_request);
			request_info(UART_C, &(UHF_data->SMPS_temp), SMPS_temp_request);
			request_info(UART_C, &(UHF_data->PA_temp), PA_temp_request);
			request_info(UART_C, &(UHF_data->RSSI), RSSI_request);
		}
		enq((void*)UHF_data); // enqueue for processing
		sleep(180); // next collection phase in 3 minutes.
	}
}



int set_uart_attribs(int fd, int speed, int parity)
{
	        struct termios tty;
	        memset (&tty, 0, sizeof tty);
	        if (tcgetattr (fd, &tty) != 0)
	        {
	        	strerror(errno);
	                return -1;
	        }

	        cfsetospeed (&tty, speed);
	        cfsetispeed (&tty, speed);



	        tty.c_iflag &= IGNBRK;          // disable break processing
	        tty.c_lflag = 0;                // no signaling chars, no echo,
	                                        // no canonical processing
	        tty.c_iflag |= ISTRIP;
	        tty.c_oflag = 0;                // no remapping, no delays
	        tty.c_cc[VMIN]  = 4;               // read block
	        tty.c_cc[VTIME] = 1;            // 0.5 seconds read timeout

	        tty.c_iflag &= ~(IXON | IXOFF | IXANY);

	        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,enable reading


	        tty.c_cflag     &=  ~PARENB;        // Make 8n1
			tty.c_cflag     &=  ~CSTOPB;
			tty.c_cflag     &=  ~CSIZE;
			tty.c_cflag     |=  CS8;
			tty.c_cflag     &=  ~CRTSCTS;       // no flow control

	        if (tcsetattr (fd, TCSANOW, &tty) != 0)
	        {
	        	strerror(errno);

	                return -1;
	        }
	        return 0;
}

	void set_uart_settings(int fd)
	{
		struct termios term;
		/* Get current configuration */
		tcgetattr(fd, &term);

		/* Set Console baud to 38400, default is 38400 */
		cfsetospeed(&term, B38400);
		cfsetispeed(&term, B38400);

		term.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);

		/* Do not echo chars */
		term.c_lflag &= ~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ECHOPRT|ECHOCTL|ECHOKE);

		term.c_cc[VMIN]  = 4;               // read block
		term.c_cc[VTIME] = 1;

		/* Turn off flow control */
		term.c_cflag |= CLOCAL;

		/* Update driver's settings */
		tcsetattr(fd, TCSANOW, &term);
	}

	void rawMode(int dev)
	{
		/* *** Configure Port *** */
		    struct termios tty;
		    memset (&tty, 0, sizeof tty);

		    /* Error Handling */
		    if ( tcgetattr ( dev, &tty ) != 0 )
		    {
		        //cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
		        perror("tcgerattr ");
		    }

		    /* Set Baud Rate */
		    cfsetospeed (&tty, B38400);
		    cfsetispeed (&tty, B38400);

		    /* Setting other Port Stuff */
		    tty.c_cflag     &=  ~PARENB;        // Make 8n1
		    tty.c_cflag     &=  ~CSTOPB;
		    tty.c_cflag     &=  ~CSIZE;
		    tty.c_cflag     |=  CS8;
		    tty.c_cflag     &=  ~CRTSCTS;       // no flow control
		    tty.c_lflag     =   0;          // no signaling chars, no echo, no canonical processing
		    tty.c_oflag     =   0;                  // no remapping, no delays
		    tty.c_cc[VMIN]      =   1;                  // read doesn't block
		    tty.c_cc[VTIME]     =   5;                  // 0.5 seconds read timeout

		    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
		    tty.c_iflag     &=  ~(IXON | IXOFF | IXANY);// turn off s/w flow ctrl
		    tty.c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG); // make raw
		    tty.c_oflag     &=  ~OPOST;              // make raw

		    /* Flush Port, then applies attributes */
		    tcflush( dev, TCIFLUSH );

		    if ( tcsetattr ( dev, TCSANOW, &tty ) != 0)
		    {
		        //cout << "Error " << errno << " from tcsetattr" << endl;
		    	printf("setattr error\n");
		    }

	}



	void ReverseInt(int *val)
	{
		unsigned int res = 0;
		unsigned int mask = 2147483648;
		unsigned int exp = 1;
		int i;


		for(i = 0; i < 32; i++)
		{
			if( ((*val) & mask) != 0 )
				res+=exp;
			exp*=2;
			mask = mask >> 1;
		}

		*val = res;

	}


	enum res request_info(int dev, int *data, unsigned int request)
	{
		int n;
		memcpy(uart_devices[dev].tx_buffer, (unsigned char *) &request ,sizeof(int));

		if(write(uart_devices[dev].fd, uart_devices[dev].tx_buffer,sizeof(int)) == -1)
		{
			printf("Error while writing \n");
			exit(EXIT_FAILURE);
		}
		n = read (uart_devices[dev].fd, data, sizeof(int));
		if(n == -1)
		{
				printf("Read failed\n");
				return FAILURE;
		}
		ReverseInt(data);
		return SUCCESS;
	}

	enum res request_info_mock(int dev, int *data, unsigned int request)
	{
		time_t t;

		/* Intializes random number generator */
		srand((unsigned) time(&t));


		if(request == Xpos_request)
		{
			*data = rand() % 100;
			return SUCCESS;
		}
		else if (request == Ypos_request)
		{
			*data = rand() % 100;
			return SUCCESS;
		}
		else if (request == Zpos_request)
		{
			*data = rand() % 100;
			return SUCCESS;
		}
		else if (request == Xval_request)
		{
			*data = rand() % 100;
			return SUCCESS;
		}
		else if (request == Yval_request)
		{
			*data = rand() % 100;
			return SUCCESS;
		}
		else if (request == Zval_request)
		{
			*data = rand() % 100;
			return SUCCESS;
		}
		else if (request == GPSTime_request)
		{
			*data = rand() % 100;
			return SUCCESS;
		}


		*data = -1;
		return FAILURE;

	}

/*CODE DUMP*/
/*
 * 	while(1)
	{




static inline uint32_t get_ticks_per_second( void )
{
  rtems_interval ticks_per_second;
  (void) rtems_clock_get( RTEMS_CLOCK_GET_TICKS_PER_SECOND, &ticks_per_second );  return ticks_per_second;
}
 */
