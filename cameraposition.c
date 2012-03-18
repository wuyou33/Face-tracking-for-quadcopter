/**
 ******************************************************************************
 * @addtogroup UAVObjects OpenPilot UAVObjects
 * @{ 
 * @addtogroup CameraPosition CameraPosition
 * @brief Coordinates to move the camera
 *
 * Autogenerated files and functions for CameraPosition Object
 * @{ 
 *
 * @file       cameraposition.c
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      Implementation of the CameraPosition object. This file has been 
 *             automatically generated by the UAVObjectGenerator.
 * 
 * @note       Object definition file: cameraposition.xml. 
 *             This is an automatically generated file.
 *             DO NOT modify manually.
 *
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "openpilot.h"
#include "cameraposition.h"

// Private variables
static UAVObjHandle handle = NULL;

/**
 * Initialize object.
 * \return 0 Success
 * \return -1 Failure to initialize or -2 for already initialized
 */
int32_t CameraPositionInitialize(void)
{
	// Don't set the handle to null if already registered
	if(UAVObjGetByID(CAMERAPOSITION_OBJID) != NULL)
		return -2;
	
	// Register object with the object manager
	handle = UAVObjRegister(CAMERAPOSITION_OBJID, CAMERAPOSITION_NAME, CAMERAPOSITION_METANAME, 0,
			CAMERAPOSITION_ISSINGLEINST, CAMERAPOSITION_ISSETTINGS, CAMERAPOSITION_NUMBYTES, &CameraPositionSetDefaults);

	// Done
	if (handle != 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

/**
 * Initialize object fields and metadata with the default values.
 * If a default value is not specified the object fields
 * will be initialized to zero.
 */
void CameraPositionSetDefaults(UAVObjHandle obj, uint16_t instId)
{
	CameraPositionData data;
	UAVObjMetadata metadata;

	// Initialize object fields to their default values
	UAVObjGetInstanceData(obj, instId, &data);
	memset(&data, 0, sizeof(CameraPositionData));

	UAVObjSetInstanceData(obj, instId, &data);

	// Initialize object metadata to their default values
	metadata.access = ACCESS_READWRITE;
	metadata.gcsAccess = ACCESS_READWRITE;
	metadata.telemetryAcked = 0;
	metadata.telemetryUpdateMode = UPDATEMODE_PERIODIC;
	metadata.telemetryUpdatePeriod = 1000;
	metadata.gcsTelemetryAcked = 0;
	metadata.gcsTelemetryUpdateMode = UPDATEMODE_MANUAL;
	metadata.gcsTelemetryUpdatePeriod = 0;
	metadata.loggingUpdateMode = UPDATEMODE_NEVER;
	metadata.loggingUpdatePeriod = 0;
	UAVObjSetMetadata(obj, &metadata);
}

/**
 * Get object handle
 */
UAVObjHandle CameraPositionHandle()
{
	return handle;
}

/**
 * Get/Set object Functions
 */
void CameraPositionxSet( float *Newx )
{
	UAVObjSetDataField(CameraPositionHandle(), (void*)Newx, offsetof( CameraPositionData, x), sizeof(float));
}
void CameraPositionxGet( float *Newx )
{
	UAVObjGetDataField(CameraPositionHandle(), (void*)Newx, offsetof( CameraPositionData, x), sizeof(float));
}
void CameraPositionySet( float *Newy )
{
	UAVObjSetDataField(CameraPositionHandle(), (void*)Newy, offsetof( CameraPositionData, y), sizeof(float));
}
void CameraPositionyGet( float *Newy )
{
	UAVObjGetDataField(CameraPositionHandle(), (void*)Newy, offsetof( CameraPositionData, y), sizeof(float));
}


/**
 * @}
 */

