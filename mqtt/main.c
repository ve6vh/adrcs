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
#include <string.h>
#include <stdlib.h>

#include "cJSON.h"
#include "datatypes.h"

// fields supplied by application
LARGE_INTEGER epochTime = {
	.QuadPart = 1722356996379
};

char *deviceID = "MyDeviceID";

// header struct: common to all types
MQTT_HDR mqtt_hdr  =
{
	.latitude = 51.08,
	.longitude = -114.10,
	.altitude = 1000,
	.speed = 0.0,
	.hdg = 264,
	.fixvalid = false,

	.batt_level = 95
};

// faked sender: just outputs on stderr
void MQTT_Send(char *topic, char *payload)
{
	fprintf(stderr, ">>>> Topic %s\n", topic);
	fprintf(stderr, "%s\n", payload);
}

/*
 *	Sample code to generate payload data
 *	Add the payload and hardware types, Device ID and epoch time
 *	then submit the header and payload structs to the apporriate
 *  Formatter. Payload structs are defined here so that the 
 *	configuration vars will work correctly
 */
#if USE_AQI
 //AQI appication struct
MQTT_AQI aqi_data = {

	.temperature = 25,
	.pressure = 1012,
	.relative_humidity = 10,
	.air_qual = 50
};
//  format an AQI sample payload
bool CreateAQIPayload(MQTT_HDR *hdr, MQTT_AQI *sensor, uint8_t *payload)
{
	cJSON *aprs;

	if ((aprs = cJSON_CreateObject()) == NULL)
		return false;

	// fill in the required fields
	hdr->PayloadType = MQTT_AQI_PAYLOAD;
	hdr->HwType = "AQI_Sample";
	hdr->DeviceID = deviceID;
	hdr->epoch.QuadPart = epochTime.QuadPart;

	if (!Format_AQI_MQTT(aprs, hdr, sensor))
		return false;
	
	if (!cJSON_PrintPreallocated(aprs, payload, SHORT_MSG_LEN, true))
		return false;
	
	// delete the object before exiting
	cJSON_Delete(aprs);

	return true;

}
#endif


#if USE_UIFRAME
// UIFrame Struct
MQTT_UIFRAME uiframe_data = {
	.Source = "ve6nhm-10",
	.Dest = "ve6nhm-2",
	.Repeaters = "Wide-1, Wide-2",
	.Frame = "542356120013275ADE6F"
};
// format a UI frame payload
bool CreateUIFramePayload(MQTT_HDR *hdr, MQTT_UIFRAME *frame, uint8_t *payload)
{
	cJSON *aprs;

	if ((aprs = cJSON_CreateObject()) == NULL)
		return false;

	// fill in the missing fields
	hdr->PayloadType = MQTT_UIFRAM_PAYLOAD;
	hdr->HwType = "UIFrame_Sample";
	hdr->DeviceID = deviceID;
	hdr->epoch.QuadPart = epochTime.QuadPart;

	if (!Format_UIFrame_MQTT(aprs, hdr, frame))
		return false;

	if (!cJSON_PrintPreallocated(aprs, payload, SHORT_MSG_LEN, true))
		return false;

	// delete the object before exiting
	cJSON_Delete(aprs);

	return true;

}
#endif


#if USE_TELEM
// telemetry struct
MQTT_TELEM telemetry_data = {
	.RadioID = 302092,
	.nADC = 3,
	.ADCData = { 813, 0, 23 },
	.nINP = 2,
	.INPData = { false, true },
	.nOUT = 2,
	.OUTData = { false, false }
};
// format a telemetry frame payload
bool CreateTelemetryPayload(MQTT_HDR *hdr, MQTT_TELEM *frame, uint8_t *payload)
{
	cJSON *aprs;

	if ((aprs = cJSON_CreateObject()) == NULL)
		return false;

	// fill in the missing fields
	hdr->PayloadType = MQTT_TELEM_PAYLOAD;
	hdr->HwType = "Telemetry_Sample";
	hdr->DeviceID = deviceID;
	hdr->epoch.QuadPart = epochTime.QuadPart;

	if (!Format_Telemetry_MQTT(aprs, hdr, frame))
		return false;

	if (!cJSON_PrintPreallocated(aprs, payload, LONG_MSG_LEN, true))
		return false;

	// delete the object before exiting
	cJSON_Delete(aprs);

	return true;

}
#endif

// test mainline
int main()
{
	char  JSONString[SHORT_MSG_LEN];
	char LongJSONString[LONG_MSG_LEN];

	// do the AQI record first
#if USE_AQI
	memset(JSONString, 0, SHORT_MSG_LEN);
	if(CreateAQIPayload(&mqtt_hdr, &aqi_data, JSONString))	{
		MQTT_Send(Get_MQTT_PubTopic(&mqtt_hdr),JSONString);													// here is where we transmit the string
	}
	else {
		fprintf(stderr, "JSON Create for AQI data Failed\n");
	}
#endif

	// now the UI frame
#if USE_UIFRAME
	memset(JSONString, 0, SHORT_MSG_LEN);
	if (CreateUIFramePayload(&mqtt_hdr, &uiframe_data, JSONString)) {
		MQTT_Send(Get_MQTT_PubTopic(&mqtt_hdr), JSONString);														// here is where we transmit the string
	}
	else {
		fprintf(stderr, "JSON Create for UI Frame Failed\n");
	}
#endif

	// and finally a telemtry frame
#if USE_TELEM
	memset(LongJSONString, 0, LONG_MSG_LEN);
	if (CreateTelemetryPayload(&mqtt_hdr, &telemetry_data, LongJSONString)) {
		MQTT_Send(Get_MQTT_PubTopic(&mqtt_hdr), LongJSONString);															// here is where we transmit the string
	}
	else {
		fprintf(stderr, "JSON Create for Telemetry Frame Failed\n");
	}
#endif

	exit(0);

}
 