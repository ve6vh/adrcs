/*---------------------------------------------------------------------------
	Project:	      APRS over MQTT

	Module:		      JSON formatter

	File Name:	      aprs_json.c

	Revision:	      1.00

	Author:			  Martin Alcock, VE6VH

	Description:      This code formats the JSON string using the cJSON library.

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

#include "cJSON.h"
#include "datatypes.h"

BOOL Format_APRS(char *deviceID, cJSON *aprs, device_shadow_t *device_data, LARGE_INTEGER *timestamp)
{
	cJSON *IDString, *TimeStamp;
	cJSON *PositionData, *PositionItem;
	cJSON *JSONLat, *JSONLong, *JSONAlt, *JSONSpeed, *JSONHdg, *JSONFix;
	cJSON *JSONBatt;
	cJSON *AQIData, *AQIItem;
	cJSON *JSONTemp, *JSONPressure, *JSONHumidity, *JSONGasRes;
	
	// add the name
	if ((IDString = cJSON_CreateString(deviceID)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(aprs, "Device ID", IDString);

	// add the system time
	if ((TimeStamp = cJSON_CreateNumber((double)timestamp->QuadPart)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(aprs, "Epoch Time", TimeStamp);

	// now the location data array
	if((PositionData = cJSON_CreateArray()) == NULL)
		return FALSE;
	cJSON_AddItemToObject(aprs, "Position", PositionData);
	if ((PositionItem = cJSON_CreateObject()) == NULL)
		return FALSE;
	cJSON_AddItemToArray(PositionData, PositionItem);

	// latitude
	if ((JSONLat = cJSON_CreateNumber(device_data->latitude)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(PositionItem, "Latitude", JSONLat);
	// longitude
	if ((JSONLong = cJSON_CreateNumber(device_data->longitude)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(PositionItem, "Longitude", JSONLong);
	// altitude
	if ((JSONAlt = cJSON_CreateNumber(device_data->altitude)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(PositionItem, "Altitude", JSONAlt);
	// speed
	if ((JSONSpeed = cJSON_CreateNumber(device_data->speed)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(PositionItem, "Speed", JSONSpeed);
	// heading
	if ((JSONHdg = cJSON_CreateNumber(device_data->hdg)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(PositionItem, "Heading", JSONHdg);
	// and finally, fix validity
	if ((JSONFix = cJSON_CreateBool(device_data->fixvalid)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(PositionItem, "Valid", JSONFix);

	// now the battery voltage
	if ((JSONBatt = cJSON_CreateNumber(device_data->batt_voltage)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(aprs, "Battery Level", JSONBatt);

	// now the BME sensor array
	if ((AQIData = cJSON_CreateArray()) == NULL)
		return FALSE;
	cJSON_AddItemToObject(aprs, "BME630", AQIData);
	if ((AQIItem = cJSON_CreateObject()) == NULL)
		return FALSE;
	cJSON_AddItemToArray(AQIData, AQIItem);

	// temperature
	if ((JSONTemp = cJSON_CreateNumber(device_data->temperature)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(AQIItem, "Temperature", JSONTemp);
	// pressure
	if ((JSONPressure = cJSON_CreateNumber(device_data->pressure)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(AQIItem, "Pressure", JSONPressure);
	// humidity
	if ((JSONHumidity = cJSON_CreateNumber(device_data->relative_humidity)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(AQIItem, "Altitude", JSONHumidity);
	// Gas Resistance
	if ((JSONGasRes = cJSON_CreateNumber(device_data->gas_res)) == NULL)
		return FALSE;
	cJSON_AddItemToObject(AQIItem, "Gas Res", JSONGasRes);

	return TRUE;
}