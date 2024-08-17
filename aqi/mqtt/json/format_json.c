/*---------------------------------------------------------------------------
	Project:	      APRS over MQTT

	Module:		      JSON formatter

	File Name:	      format_json.c

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

					  Copyright � 2023, Alberta Digital Radio Communications Society


	Revision History:

---------------------------------------------------------------------------*/

#include <stdbool.h>

#include "cJSON.h"
#include "datatypes.h"

bool Format_JSON(cJSON *root_object, char *deviceID, device_shadow_t *device_data, LARGE_INTEGER *timestamp)
{

	cJSON *IDString, *TimeStamp;
	cJSON *PositionData, *PositionItem;
	cJSON *JSONLat, *JSONLong, *JSONAlt, *JSONSpeed, *JSONHdg, *JSONFix;
	cJSON *JSONBatt;
	cJSON *AQIData, *AQIItem;
	cJSON *JSONTemp, *JSONPressure, *JSONHumidity, *JSONIAQ;

	/*
	 *	Start with the device ID
	 */
	if ((IDString = cJSON_CreateString(deviceID)) == NULL)
		return false;
	cJSON_AddItemToObject(root_object, "Device ID", IDString);

	/*
	 *	Add the position array
	 */
	if((PositionData = cJSON_CreateArray()) == NULL)
		return false;
	cJSON_AddItemToObject(root_object, "Position", PositionData);
	if ((PositionItem = cJSON_CreateObject()) == NULL)
		return false;
	cJSON_AddItemToArray(PositionData, PositionItem);

	// latitude
	if ((JSONLat = cJSON_CreateNumber(device_data->latitude)) == NULL)
		return false;
	cJSON_AddItemToObject(PositionItem, "Latitude", JSONLat);
	// longitude
	if ((JSONLong = cJSON_CreateNumber(device_data->longitude)) == NULL)
		return false;
	cJSON_AddItemToObject(PositionItem, "Longitude", JSONLong);
	// altitude
	if ((JSONAlt = cJSON_CreateNumber(device_data->altitude)) == NULL)
		return false;
	cJSON_AddItemToObject(PositionItem, "Altitude", JSONAlt);
	// speed
	if ((JSONSpeed = cJSON_CreateNumber(device_data->speed)) == NULL)
		return false;
	cJSON_AddItemToObject(PositionItem, "Speed", JSONSpeed);
	// heading
	if ((JSONHdg = cJSON_CreateNumber(device_data->hdg)) == NULL)
		return false;
	cJSON_AddItemToObject(PositionItem, "Heading", JSONHdg);
	// and finally, fix validity
	if ((JSONFix = cJSON_CreateBool(device_data->fixvalid)) == NULL)
		return false;
	cJSON_AddItemToObject(PositionItem, "Valid", JSONFix);

	/*
	 *	Add the BME680 data
	 */
	if ((AQIData = cJSON_CreateArray()) == NULL)
		return false;
	cJSON_AddItemToObject(root_object, "BME630", AQIData);
	if ((AQIItem = cJSON_CreateObject()) == NULL)
		return false;
	cJSON_AddItemToArray(AQIData, AQIItem);
	// temperature
	if ((JSONTemp = cJSON_CreateNumber(device_data->temperature)) == NULL)
		return false;
	cJSON_AddItemToObject(AQIItem, "Temperature", JSONTemp);
	// pressure
	if ((JSONPressure = cJSON_CreateNumber(device_data->pressure)) == NULL)
		return false;
	cJSON_AddItemToObject(AQIItem, "Pressure", JSONPressure);
	// humidity
	if ((JSONHumidity = cJSON_CreateNumber(device_data->relative_humidity)) == NULL)
		return false;
	cJSON_AddItemToObject(AQIItem, "Humidity", JSONHumidity);
	// Gas Resistance
	if ((JSONIAQ = cJSON_CreateNumber(device_data->air_qual)) == NULL)
		return false;
	cJSON_AddItemToObject(AQIItem, "IAQ", JSONIAQ);

	/*
	 *	Add the battery voltage
	 */
	if ((JSONBatt = cJSON_CreateNumber(device_data->batt_voltage)) == NULL)
		return false;
	cJSON_AddItemToObject(root_object, "Battery Level", JSONBatt);

	/*
	 *	Finally add the timestamp
	 */
	if ((TimeStamp = cJSON_CreateNumber((double)timestamp->QuadPart)) == NULL)
		return false;
	cJSON_AddItemToObject(root_object, "Epoch Time", TimeStamp);

	// completed
	return true;
}