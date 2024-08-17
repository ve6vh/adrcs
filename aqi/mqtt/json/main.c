/*---------------------------------------------------------------------------
	Project:	      APRS over MQTT

	Module:		      test mainline

	File Name:	      main.c

	Revision:	      1.00

	Author:			  Martin Alcock, VE6VH

	Description:      This is a test mainline to generate and print the JSON data for MQTT

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

#include <stdio.h>
#include <winternl.h>
#include <stdbool.h>
#include <stdint.h>

#include "cJSON.h"
#include "datatypes.h"

LARGE_INTEGER epochTime = {
	.QuadPart = 1722356996379
};

device_shadow_t aprs_data =
{
	.latitude = 51.08,
	.longitude = -114.10,
	.altitude = 1000,
	.speed = 0.0,
	.hdg = 264,
	.fixvalid = false,

	.batt_voltage = 0,

	.temperature = 25,
	.pressure = 1012,
	.relative_humidity = 10,
	.air_qual = 50
};

char *deviceID = "MyDeviceID";

// faked sender
void MQTT_Send(char *JSONString)
{
	fprintf(stderr, "%s\n", JSONString);
}

// this calls the formatter; returns null if anything fails
bool CreateMQTTString(char *deviceID, device_shadow_t *device_data, LARGE_INTEGER *timestamp, uint8_t *payload)
{
	cJSON *aprs;

	if ((aprs = cJSON_CreateObject()) == NULL)
		return false;

	if (!Format_JSON(aprs, deviceID, device_data, timestamp))
		return false;
	
	if (!cJSON_PrintPreallocated(aprs, payload, DEVICE_MSG_LEN, true))
		return false;
	
	// delete the object before exiting
	cJSON_Delete(aprs);

	return true;

}

// test mainline
int main()
{
	char  JSONString[DEVICE_MSG_LEN];

	if(CreateMQTTString(deviceID, &aprs_data, &epochTime, JSONString))	{
		MQTT_Send(JSONString);													// here is where we transmit the string
	}
	else {
		fprintf(stderr, "JSON Create Failed\n");
	}

}
