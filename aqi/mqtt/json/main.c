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
#include <stdint.h>

#include "cJSON.h"
#include "datatypes.h"


LARGE_INTEGER epochTime = {
	.QuadPart = 1722356996379
};

device_shadow_t aprs_data = {
	.latitude = 51.08,
	.longitude = -114.10,
	.altitude = 1000,
	.speed = 0.0,
	.hdg = 264,
	.batt_voltage = 95,
	.temperature = 25,
	.pressure = 1012,
	.gas_res = 1000
};

char *deviceID = "MyDeviceID";

// faked sender
void MQTT_Send(char *JSONString)
{
	fprintf(stderr, "%s\n", JSONString);
}

// this calls the formatter; returns null if anything fails
char *Create_MQTTString(char *deviceID, device_shadow_t device_data, uint64_t timestamp)
{
	cJSON *aprs;
	char *MQTTString;

	if ((aprs = cJSON_CreateObject()) == NULL)
		return NULL;

	if (!Format_APRS(deviceID, aprs, &aprs_data, &epochTime))
		return NULL;

	if ((MQTTString = cJSON_Print(aprs)) = NULL)
		return NULL;

	return MQTTString;

}

// test mainline
int main()
{
	char  *JSONString;
	
	if((JSONString=CreateMQTTString(deviceID, device_data, timestamp)) != NULL)	{
		MQTT_Send(JSONString);			// here is where we transmit the string
	}
	else {
		fprintf(stderr, "JSON Create Failed\n");
	}

}
