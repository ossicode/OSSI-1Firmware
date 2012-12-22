/*
 * gps.c
 *
 *  Created on: 2012. 12. 21.
 *      Author: OSSI
 *      Inspired by: http://rev0proto.com/wiki/index.php/Senior_Project / Justin Kenny
 */

#include "gps.h"

static volatile uint8_t gps_ready_flag = 0;

uint8_t rawgps[82];
uint8_t gpsgga[82];
uint8_t gpsindex = 0;
uint8_t gpsdone = 0;
Gps gpsdata = {"3518.5230", "12039.6090", "123456.789", "00", "000000.0", 'N', 'W', '0'};

uint8_t gps_array_data[100]={0}; // 59 = sizeof(gpsdata) + spacer character

void gps_set_readyFlag(void)
{
	gps_ready_flag = 1;
}

void gps_clear_readyFlag(void)
{
	gps_ready_flag = 0;
}

uint8_t gps_is_ready(void)
{
	if(gps_ready_flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

void gps_parse_nmea(void)
{
	IE2 &= ~UCA0RXIE; //Disable receive interrupt
	uint8_t commas[MAXCOMMAS];
	uint8_t i = 0, n;
	for(n=0;n<MAXCOMMAS;n++) //Find the positions of all commas in the NMEA sentence, put positions in commas[]
	{
	   for(;rawgps[i]!=','||i>MAXGPS;i++); //Find next comma; continue stepping through  the array until we find 0x2C (,)
	   commas[n] = i; //Store the index in commas[] array
	   i++;
	}
	if(rawgps[commas[5]+1] != '0') //Make sure we have GPS fix; 0 = invalid
	{
	   for(i=commas[1]+1;i<commas[2];i++)
	   {
			   gpsdata.lat[i-(commas[1]+1)] = rawgps[i]; //Load latitude into lat[] array from stored NMEA string
	   }
	   //gpsdata.lat[i-(commas[1])-1] = '\0';
	   gpsdata.ns = rawgps[commas[2]+1];

	   for(i=commas[3]+1;i<commas[4];i++)
	   {
			   gpsdata.lng[i-(commas[3]+1)] = rawgps[i]; //Load longitude into lng[] array from stored NMEA string
	   }
	   //gpsdata.lng[i-(commas[3])-1] = '\0';
	   gpsdata.ew = rawgps[commas[4]+1];

	   for(i=commas[6]+1;i<commas[7];i++)
	   {
			   gpsdata.sats[i-(commas[6]+1)] = rawgps[i]; //Load sats into sats[] array from stored NMEA string
	   }
	   //gpsdata.sats[i-(commas[6])-1] = '\0';

	   for(i=commas[8]+1;i<commas[9];i++)
	   {
			   gpsdata.alti[i-(commas[8]+1)] = rawgps[i]; //Load alt into alti[] array from stored NMEA string
	   }
	   //gpsdata.alti[i-(commas[8])-1] = '\0';

	   for(i=commas[0]+1;i<commas[1];i++)
	   {
			   gpsdata.time[i-(commas[0]+1)] = rawgps[i]; //Load time into time[] array from stored NMEA string
	   }
	   //gpsdata.time[i-(commas[0])-1] = '\0';
	   gpsdata.valid = rawgps[commas[5]+1];
	}
	else //Else update the timestamp, but retain old GPS data
	{
	   for(i=commas[0]+1;i<commas[1];i++)
	   {
			   gpsdata.time[i-(commas[0]+1)] = rawgps[i];
	   }
	   //gpsdata.time[i-(commas[0])-1] = '\0';

	   for(i=commas[6]+1;i<commas[7];i++)
	   {
			   gpsdata.sats[i-(commas[6]+1)] = rawgps[i]; //Load sats into sats[] array from stored NMEA string
	   }
	   //gpsdata.sats[i-(commas[6])-1] = '\0';
	   gpsdata.valid = '0';
	}
	IE2 |= UCA0RXIE; //Enable receive interrupt
}

void gps_make_packet(void)
{
	volatile uint8_t i;
	volatile uint8_t step = 0;


	gps_array_data[step] ='C';
	step++;
	gps_array_data[step] ='Q';
	step++;
	gps_array_data[step] ='C';
	step++;
	gps_array_data[step] ='Q';
	step++;
	gps_array_data[step] ='C';
	step++;
	gps_array_data[step] ='Q';
	step++;

	gps_array_data[step] =' ';
	step++;

	gps_array_data[step] ='D';
	step++;

	gps_array_data[step] ='E';
	step++;

	gps_array_data[step] =' ';
	step++;

	gps_array_data[step] ='O';
	step++;

	gps_array_data[step] ='S';
	step++;

	gps_array_data[step] ='S';
	step++;

	gps_array_data[step] ='I';
	step++;

	gps_array_data[step] =' ';
	step++;


	for (i = 0; i < 9 ; i++) {
		gps_array_data[step] = gpsdata.lat[i];
		step++;
	}

	gps_array_data[step] =' ';
	step++;

	for (i = 0; i < 10 ; i++) {
		gps_array_data[step] = gpsdata.lng[i];
		step++;
	}

	gps_array_data[step] =' ';
	step++;

//	for (i = 0; i < 10 ; i++) {
//		gps_array_data[step] = gpsdata.time[i];
//		step++;
//	}
//
//	gps_array_data[step] =',';
//	step++;

	for (i = 0; i < 2 ; i++) {
		gps_array_data[step] = gpsdata.sats[i];
		step++;
	}

	gps_array_data[step] =' ';
	step++;

	for (i = 0; i < 8 ; i++) {
		gps_array_data[step] = gpsdata.alti[i];
		step++;
	}

	gps_array_data[step] =' ';
	step++;

	gps_array_data[step] = gpsdata.ns;
	step++;
	gps_array_data[step] = gpsdata.ew;
	step++;
	gps_array_data[step] = gpsdata.valid;

}


uint8_t gps_update_data(uint8_t data)
{
	if(data == '$') //$ = Start of NMEA Sentence
	{
		gpsindex = 0;
		gpsdone = 0;
	}
	else if(data == '*') //<CR> = End of Transmission
	{
		gpsdone = 1;
		if(rawgps[4] == 'G') //Make sure this is a GGA sentence
		{
			gps_parse_nmea();
			return 1;
		}
	}
	if(gpsdone != 1)
	{
		rawgps[gpsindex] = data;
		gpsindex++;
	}
	if(gpsindex > 81)
	{
		gpsindex = 0;
	}
	return 0;
}

uint8_t* gps_get_stream(void)
{
	return gps_array_data;
}
