/*---------------------------------------------------------------------------
	Project:	      APRS over MQTT

	Module:		      formatter header file

	File Name:	      datatypes.h

	Revision:	      1.00

	Author:			  Martin Alcock, VE6VH

	Description:      This file contains the formatter for the APRS over MQTT project.

					  This software module and all others are the exclusive property of the Alberta Digital
					  Radio Communications Society and others (?the owners?), all rights are reserved.

					  The owners grant licence to any Amateur for personal or club use, and only on hardware
					  designed by them expressly for this purpose, use on any other hardware is prohibited.

					  This file, and others may be copied or distributed only with the inclusion of this
					  notice.

					  No warranty, either express or implied or transfer of rights is granted in this
					  licence and the owner is not liable for any outcome whatsoever arising from such usage.

					  Copyright © 2023, Alberta Digital Radio Communications Society


	Revision History:

---------------------------------------------------------------------------*/
#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef BOOL
typedef uint8_t BOOL;
#define FALSE 0
#define TRUE  1
#endif

#define DEVICE_MSG_LEN 240 // some placeholder value for now.

#ifndef LARGE_INTEGER
typedef union _LARGE_INTEGER {
	struct {
		uint32_t LowPart;
		uint32_t  HighPart;
	} DUMMYSTRUCTNAME;
	struct {
		uint32_t LowPart;
		uint32_t  HighPart;
	} u;
	uint64_t QuadPart;
} LARGE_INTEGER;
#endif

typedef struct device_shadow
{
	// location
	double latitude;
	double longitude;
	float altitude; 	// skeptical
	float speed;		// speed
	float hdg;			// heading
	bool   fixvalid;	// fix is valid
	// batttery
	int    batt_voltage;
	// BME630 output
	int    temperature; 	// you can do the arithmetic to convert these to float in bme680.c if you wish. the log shows how.
	int 	pressure;
	int 	relative_humidity;
	int 	gas_res;

} device_shadow_t;

/* @brief Just an snprintf wrapper.
    Returns the number of characters that would have been written if n had been sufficiently large, not counting the terminating null character.
    If an encoding error occurs, a negative number is returned.
    Notice that only when this returned value is non-negative and less than n, the string has been completely written.
*/
BOOL Format_APRS(char *deviceID, cJSON *aprs, device_shadow_t *device_data, LARGE_INTEGER *timestamp);


#endif /* _DATATYPES_H_ */
