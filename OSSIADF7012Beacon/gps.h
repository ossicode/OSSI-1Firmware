/*
 * gps.h
 *
 *  Created on: 2012. 12. 21.
 *      Author: OSSI
 */

#ifndef GPS_H_
#define GPS_H_

#include "ossibeacon.h"

#define MAXCOMMAS 14   //Max commas to store positions of (14 for GGA sentence)
#define MAXGPS 82              //Max NMEA string size
#define MAXLAT 9               //Max latitude size in bytes
#define MAXLONG 10             //Max longitude size in bytes
#define MAXTIME 10             //Max time size in bytes
#define MAXSATS 2              //Max sats size in bytes
#define MAXALTI 8              //Max altitude size in bytes

typedef struct
{
	uint8_t lat[MAXLAT];
	uint8_t lng[MAXLONG];
	uint8_t time[MAXTIME];
	uint8_t sats[MAXSATS];
	uint8_t alti[MAXALTI];
	uint8_t ns;
	uint8_t ew;
	uint8_t valid;
} Gps;

void gps_set_readyFlag(void);
void gps_clear_readyFlag(void);
uint8_t gps_is_ready(void);

uint8_t gps_update_data(uint8_t data);
void gps_make_packet(void);
uint8_t* gps_get_stream(void);

#endif /* GPS_H_ */
