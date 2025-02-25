/*
 Author: Matt Bunting
 Copyright (c) 2020 Arizona Board of Regents
 All rights reserved.

 Permission is hereby granted, without written agreement and without
 license or royalty fees, to use, copy, modify, and distribute this
 software and its documentation for any purpose, provided that the
 above copyright notice and the following two paragraphs appear in
 all copies of this software.

 IN NO EVENT SHALL THE ARIZONA BOARD OF REGENTS BE LIABLE TO ANY PARTY
 FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

 THE ARIZONA BOARD OF REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER
 IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION
 TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

 */

#ifndef OBD_PID_DEFINITIONSS_H
#define OBD_PID_DEFINITIONSS_H



#define CAN_FRAME_TYPE_MASK (0xF0)

namespace Panda {


// TODO: Finish codifying some PID codes from here as enums/defines
// https://en.wikipedia.org/wiki/OBD-II_PIDs


enum CanFrameTypes {
	CAN_FRAME_SINGLE = 0x00,
	CAN_FRAME_FIRST = 0x01,
	CAN_FRAME_CONSECUTIVE = 0x02,
	CAN_FRAME_FLOW_CONTROL = 0x03
};


enum ObdPidModes {
	OBD_PID_SERVICE_CURRENT_DATA = 0x01,
	OBD_PID_SERVICE_FREEZE_FRAME_DATA = 0x02,
	OBD_PID_SERVICE_VEHICLE_INFO = 0x09
};

enum ObdPidVehicleInfo {
	OBD_PID_VEHICLE_INFO_VIN_COUNT = 0x01,
	OBD_PID_VEHICLE_INFO_VIN = 0x02
};

}

#endif
