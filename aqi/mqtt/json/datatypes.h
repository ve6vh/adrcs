#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "cJSON.h"

// struct to hold sensor data
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
	double  temperature; 	// you can do the arithmetic to convert these to float in bme680.c if you wish. the log shows how.
	double 	pressure;
	double 	relative_humidity;
	double 	air_qual;

} device_shadow_t;

// define a large integer
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


#define DEVICE_MSG_LEN 500	 				// allow a larger buffer
/* 	@brief A more detailed implementation that uses the cJSON library to format the message
	Creates an unformatted json string using the cJSON libaray. Buffer is preallocated
	so as to avoid a corrupt heap.
	@param deviceID Character string with a unique device ID
	@param data a cJSON object in which to store the data
	@param device_data Struct containing the device samples
	@param timestamp timestamp for data
*/
bool Format_JSON(cJSON *root_object, char *deviceID, device_shadow_t *device_data, LARGE_INTEGER *timestamp);

#endif /* _DATATYPES_H_ */
