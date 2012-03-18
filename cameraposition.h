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
 * @file       cameraposition.h
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

#ifndef CAMERAPOSITION_H
#define CAMERAPOSITION_H

// Object constants
#define CAMERAPOSITION_OBJID 0xF1F9A6E
#define CAMERAPOSITION_NAME "CameraPosition"
#define CAMERAPOSITION_METANAME "CameraPositionMeta"
#define CAMERAPOSITION_ISSINGLEINST 1
#define CAMERAPOSITION_ISSETTINGS 0
#define CAMERAPOSITION_NUMBYTES sizeof(CameraPositionData)

// Object access macros
/**
 * @function CameraPositionGet(dataOut)
 * @brief Populate a CameraPositionData object
 * @param[out] dataOut 
 */
#define CameraPositionGet(dataOut) UAVObjGetData(CameraPositionHandle(), dataOut)
#define CameraPositionSet(dataIn) UAVObjSetData(CameraPositionHandle(), dataIn)
#define CameraPositionInstGet(instId, dataOut) UAVObjGetInstanceData(CameraPositionHandle(), instId, dataOut)
#define CameraPositionInstSet(instId, dataIn) UAVObjSetInstanceData(CameraPositionHandle(), instId, dataIn)
#define CameraPositionConnectQueue(queue) UAVObjConnectQueue(CameraPositionHandle(), queue, EV_MASK_ALL_UPDATES)
#define CameraPositionConnectCallback(cb) UAVObjConnectCallback(CameraPositionHandle(), cb, EV_MASK_ALL_UPDATES)
#define CameraPositionCreateInstance() UAVObjCreateInstance(CameraPositionHandle(),&CameraPositionSetDefaults)
#define CameraPositionRequestUpdate() UAVObjRequestUpdate(CameraPositionHandle())
#define CameraPositionRequestInstUpdate(instId) UAVObjRequestInstanceUpdate(CameraPositionHandle(), instId)
#define CameraPositionUpdated() UAVObjUpdated(CameraPositionHandle())
#define CameraPositionInstUpdated(instId) UAVObjUpdated(CameraPositionHandle(), instId)
#define CameraPositionGetMetadata(dataOut) UAVObjGetMetadata(CameraPositionHandle(), dataOut)
#define CameraPositionSetMetadata(dataIn) UAVObjSetMetadata(CameraPositionHandle(), dataIn)
#define CameraPositionReadOnly() UAVObjReadOnly(CameraPositionHandle())

// Object data
typedef struct {
    float x;
    float y;

} __attribute__((packed)) CameraPositionData;

// Field information
// Field x information
// Field y information


// Generic interface functions
int32_t CameraPositionInitialize();
UAVObjHandle CameraPositionHandle();
void CameraPositionSetDefaults(UAVObjHandle obj, uint16_t instId);

// set/Get functions
extern void CameraPositionxSet( float *Newx );
extern void CameraPositionxGet( float *Newx );
extern void CameraPositionySet( float *Newy );
extern void CameraPositionyGet( float *Newy );


#endif // CAMERAPOSITION_H

/**
 * @}
 * @}
 */
