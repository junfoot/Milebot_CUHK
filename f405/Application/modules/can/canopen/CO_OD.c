/*******************************************************************************

   File - CO_OD.c/CO_OD.h
   CANopen Object Dictionary.

   Copyright (C) 2004-2008 Janez Paternoster

   License: GNU Lesser General Public License (LGPL).

   <http://canopennode.sourceforge.net>

   (For more information see <CO_SDO.h>.)

   This file is part of CANopenNode, an open source CANopen Stack.
   Project home page is <https://github.com/CANopenNode/CANopenNode>.
   For more information on CANopen see <http://www.can-cia.org/>.
 
   CANopenNode is free and open source software: you can redistribute
   it and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation, either version 2 of the
   License, or (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
  
   Following clarification and special exception to the GNU General Public
   License is included to the distribution terms of CANopenNode:
  
   Linking this library statically or dynamically with other modules is
   making a combined work based on this library. Thus, the terms and
   conditions of the GNU General Public License cover the whole combination.
  
   As a special exception, the copyright holders of this library give
   you permission to link this library with independent modules to
   produce an executable, regardless of the license terms of these
   independent modules, and to copy and distribute the resulting
   executable under terms of your choice, provided that you also meet,
   for each linked independent module, the terms and conditions of the
   license of that module. An independent module is a module which is
   not derived from or based on this library. If you modify this
   library, you may extend this exception to your version of the
   library, but you are not obliged to do so. If you do not wish
   to do so, delete this exception statement from your version.
 
   This file was automatically generated with libedssharp Object
   Dictionary Editor v0.7-9-g821eedb   DON'T EDIT THIS FILE MANUALLY !!!!
*******************************************************************************/


#include "CO_driver.h"
#include "CO_OD.h"
#include "CO_SDO.h"


/*******************************************************************************
   DEFINITION AND INITIALIZATION OF OBJECT DICTIONARY VARIABLES
*******************************************************************************/


/***** Definition for ROM variables ********************************************/
struct sCO_OD_ROM CO_OD_ROM = {
           CO_OD_FIRST_LAST_WORD,

/*1000*/ 0x0000L,
/*1005*/ 0x0080L,
/*1006*/ 0x0000L,
/*1007*/ 0x0000L,
/*1008*/ {'E', 'X', 'O', '0', '1', 'C'},
/*1009*/ {'0', '.', '0', '1'},
/*100a*/ {'0', '.', '0', '1'},
/*100c*/ 0x00,
/*1012*/ 0x0000L,
/*1014*/ 0x0080L,
/*1015*/ 0x64,
/*1016*/ {0x00000000, 0x00000000, 0x00000000, 0x00000000},
/*1017*/ 0x190,
/*1018*/ {0x4L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1019*/ 0x0L,
/*1029*/ {0x00, 0x00, 0x01, 0x00, 0x00, 0x00},
/*1200*/ {{0x2L, 0x0600L, 0x0580L}},
/*1400*/ {{0x2L, 0x0186L, 0xfeL}},
/*1600*/ {{0x2L, 0x60640020L, 0x60410010L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L}},
/*1800*/ {{0x6L, 0x0206L, 0xfeL, 0x64, 0x0L, 0x00, 0x0L}},
/*1a00*/ {{0x3L, 0x60c10110L, 0x60c10210L, 0x60400010L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L}},
/*1f80*/ 0x0001L,
/*2101*/ 0x9L,
/*2102*/ 0x36b,
/*2111*/ {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*6061*/ 0x0,
/*6062*/ 0x0000L,
/*6065*/ 0x0000L,
/*607f*/ 0x0000L,
/*6085*/ 0x0000L,
/*60c2*/ {0x0L},
/*60c5*/ 0x0000L,
/*60c6*/ 0x0000L,

           CO_OD_FIRST_LAST_WORD,
};


/***** Definition for RAM variables ********************************************/
struct sCO_OD_RAM CO_OD_RAM = {
           CO_OD_FIRST_LAST_WORD,

/*1001*/ 0x0L,
/*1002*/ 0x0000L,
/*1003*/ {0, 0, 0, 0, 0, 0, 0, 0},
/*100d*/ 0x0L,
/*1010*/ {0x00000003},
/*1011*/ {0x00000001},
/*1013*/ 0x0000L,
/*1280*/ {{0x3L, 0x0600L, 0x0580L, 0x6L}},
/*2100*/ {0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L},
/*2103*/ 0x00,
/*2104*/ 0x00,
/*2107*/ {1000, 0, 0, 0, 0},
/*2108*/ {0},
/*2109*/ {0},
/*2110*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*2120*/ {0x5L, 0x1234567890abcdefL, 0x234567890abcdef1L, 12.345, 456.789, 0},
/*2130*/ {0x3L, {'-', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, 0x00000000L, 0x0000L},
/*6000*/ {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
/*603f*/ 0x00,
/*6040*/ 0x00,
/*6041*/ 0x00,
/*6060*/ 0x0,
/*6063*/ 0x0000L,
/*6064*/ 0x0000L,
/*606b*/ 0x0000L,
/*606c*/ 0x0000L,
/*60c1*/ {0x00, 0x00},
/*60fc*/ 0x0000L,
/*6200*/ {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
/*6401*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*6411*/ {0, 0, 0, 0, 0, 0, 0, 0},
/*6500*/ {0, 0, 0, 0, 0, 0, 0, 0},
/*6501*/ {0, 0, 0, 0, 0, 0, 0, 0},
/*6502*/ {0, 0, 0, 0, 0, 0, 0, 0},
/*6503*/ {0, 0, 0, 0, 0, 0, 0, 0},

           CO_OD_FIRST_LAST_WORD,
};


/***** Definition for EEPROM variables ********************************************/
struct sCO_OD_EEPROM CO_OD_EEPROM = {
           CO_OD_FIRST_LAST_WORD,

/*2106*/ 0x0000L,
/*2112*/ {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

           CO_OD_FIRST_LAST_WORD,
};




/*******************************************************************************
   STRUCTURES FOR RECORD TYPE OBJECTS
*******************************************************************************/


/*0x1018*/ const CO_OD_entryRecord_t OD_record1018[5] = {
           {(void*)&CO_OD_ROM.identity.maxSubIndex, 0x05, 0x1 },
           {(void*)&CO_OD_ROM.identity.vendorID, 0x85, 0x4 },
           {(void*)&CO_OD_ROM.identity.productCode, 0x85, 0x4 },
           {(void*)&CO_OD_ROM.identity.revisionNumber, 0x85, 0x4 },
           {(void*)&CO_OD_ROM.identity.serialNumber, 0x85, 0x4 },
};

/*0x1200*/ const CO_OD_entryRecord_t OD_record1200[3] = {
           {(void*)&CO_OD_ROM.SDOServerParameter[0].maxSubIndex, 0x05, 0x1 },
           {(void*)&CO_OD_ROM.SDOServerParameter[0].COB_IDClientToServer, 0x85, 0x4 },
           {(void*)&CO_OD_ROM.SDOServerParameter[0].COB_IDServerToClient, 0x85, 0x4 },
};

/*0x1280*/ const CO_OD_entryRecord_t OD_record1280[4] = {
           {(void*)&CO_OD_RAM.SDOClientParameter[0].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.SDOClientParameter[0].COB_IDClientToServer, 0xbe, 0x4 },
           {(void*)&CO_OD_RAM.SDOClientParameter[0].COB_IDServerToClient, 0xbe, 0x4 },
           {(void*)&CO_OD_RAM.SDOClientParameter[0].nodeIDOfTheSDOServer, 0x0e, 0x1 },
};

/*0x1400*/ const CO_OD_entryRecord_t OD_record1400[3] = {
           {(void*)&CO_OD_ROM.RPDOCommunicationParameter[0].maxSubIndex, 0x05, 0x1 },
           {(void*)&CO_OD_ROM.RPDOCommunicationParameter[0].COB_IDUsedByRPDO, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.RPDOCommunicationParameter[0].transmissionType, 0x0d, 0x1 },
};

/*0x1600*/ const CO_OD_entryRecord_t OD_record1600[9] = {
           {(void*)&CO_OD_ROM.RPDOMappingParameter[0].numberOfMappedObjects, 0x0d, 0x1 },
           {(void*)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject1, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject2, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject3, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject4, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject5, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject6, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject7, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject8, 0x8d, 0x4 },
};

/*0x1800*/ const CO_OD_entryRecord_t OD_record1800[7] = {
           {(void*)&CO_OD_ROM.TPDOCommunicationParameter[0].maxSubIndex, 0x05, 0x1 },
           {(void*)&CO_OD_ROM.TPDOCommunicationParameter[0].COB_IDUsedByTPDO, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.TPDOCommunicationParameter[0].transmissionType, 0x0d, 0x1 },
           {(void*)&CO_OD_ROM.TPDOCommunicationParameter[0].inhibitTime, 0x8d, 0x2 },
           {(void*)&CO_OD_ROM.TPDOCommunicationParameter[0].comparabilityEntry, 0x05, 0x1 },
           {(void*)&CO_OD_ROM.TPDOCommunicationParameter[0].eventTimer, 0x8d, 0x2 },
           {(void*)&CO_OD_ROM.TPDOCommunicationParameter[0].SYNCStartValue, 0x0d, 0x1 },
};

/*0x1a00*/ const CO_OD_entryRecord_t OD_record1a00[9] = {
           {(void*)&CO_OD_ROM.TPDOMappingParameter[0].numberOfMappedObjects, 0x0d, 0x1 },
           {(void*)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject1, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject2, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject3, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject4, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject5, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject6, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject7, 0x8d, 0x4 },
           {(void*)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject8, 0x8d, 0x4 },
};

/*0x2120*/ const CO_OD_entryRecord_t OD_record2120[6] = {
           {(void*)&CO_OD_RAM.testVar.maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.testVar.I64, 0xbe, 0x8 },
           {(void*)&CO_OD_RAM.testVar.U64, 0xbe, 0x8 },
           {(void*)&CO_OD_RAM.testVar.R32, 0xbe, 0x4 },
           {(void*)&CO_OD_RAM.testVar.R64, 0xbe, 0x8 },
           {(void*)0, 0x0e, 0x0 },
};

/*0x2130*/ const CO_OD_entryRecord_t OD_record2130[4] = {
           {(void*)&CO_OD_RAM.time.maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.time.string, 0x06, 0x1e },
           {(void*)&CO_OD_RAM.time.epochTimeBaseMs, 0x8e, 0x8 },
           {(void*)&CO_OD_RAM.time.epochTimeOffsetMs, 0xbe, 0x4 },
};

/*0x60c2*/ const CO_OD_entryRecord_t OD_record60c2[1] = {
           {(void*)&CO_OD_ROM.interpolation_time_period.maxSubIndex, 0x05, 0x1 },
};

/*******************************************************************************
   OBJECT DICTIONARY
*******************************************************************************/
const CO_OD_entry_t CO_OD[70] = {

{0x1000, 0x00, 0x85, 4, (void*)&CO_OD_ROM.deviceType},
{0x1001, 0x00, 0x26, 1, (void*)&CO_OD_RAM.errorRegister},
{0x1002, 0x00, 0xa6, 4, (void*)&CO_OD_RAM.manufacturerStatusRegister},
{0x1003, 0x08, 0x86, 4, (void*)&CO_OD_RAM.preDefinedErrorField[0]},
{0x1005, 0x00, 0x8d, 4, (void*)&CO_OD_ROM.COB_ID_SYNCMessage},
{0x1006, 0x00, 0x8d, 4, (void*)&CO_OD_ROM.communicationCyclePeriod},
{0x1007, 0x00, 0x8d, 4, (void*)&CO_OD_ROM.synchronousWindowLength},
{0x1008, 0x00, 0x05, 6, (void*)&CO_OD_ROM.manufacturerDeviceName},
{0x1009, 0x00, 0x05, 4, (void*)&CO_OD_ROM.manufacturerHardwareVersion},
{0x100a, 0x00, 0x05, 4, (void*)&CO_OD_ROM.manufacturerSoftwareVersion},
{0x100c, 0x00, 0x85, 2, (void*)&CO_OD_ROM.guardTime},
{0x100d, 0x00, 0x06, 1, (void*)&CO_OD_RAM.lifeTimeFactor},
{0x1010, 0x01, 0x8e, 4, (void*)&CO_OD_RAM.storeParameters[0]},
{0x1011, 0x01, 0x8e, 4, (void*)&CO_OD_RAM.restoreDefaultParameters[0]},
{0x1012, 0x00, 0x85, 4, (void*)&CO_OD_ROM.COB_ID_TIME},
{0x1013, 0x00, 0x8e, 4, (void*)&CO_OD_RAM.highResolutionTimeStamp},
{0x1014, 0x00, 0x85, 4, (void*)&CO_OD_ROM.COB_ID_EMCY},
{0x1015, 0x00, 0x8d, 2, (void*)&CO_OD_ROM.inhibitTimeEMCY},
{0x1016, 0x04, 0x8d, 4, (void*)&CO_OD_ROM.consumerHeartbeatTime[0]},
{0x1017, 0x00, 0x8d, 2, (void*)&CO_OD_ROM.producerHeartbeatTime},
{0x1018, 0x04, 0x00, 0, (void*)&OD_record1018},
{0x1019, 0x00, 0x0d, 1, (void*)&CO_OD_ROM.synchronousCounterOverflowValue},
{0x1029, 0x06, 0x0d, 1, (void*)&CO_OD_ROM.errorBehavior[0]},
{0x1200, 0x02, 0x00, 0, (void*)&OD_record1200},
{0x1280, 0x03, 0x00, 0, (void*)&OD_record1280},
{0x1400, 0x02, 0x00, 0, (void*)&OD_record1400},
{0x1600, 0x08, 0x00, 0, (void*)&OD_record1600},
{0x1800, 0x06, 0x00, 0, (void*)&OD_record1800},
{0x1a00, 0x08, 0x00, 0, (void*)&OD_record1a00},
{0x1f80, 0x00, 0x8d, 4, (void*)&CO_OD_ROM.NMTStartup},
{0x2100, 0x00, 0x26, 10, (void*)&CO_OD_RAM.errorStatusBits},
{0x2101, 0x00, 0x0d, 1, (void*)&CO_OD_ROM.CANNodeID},
{0x2102, 0x00, 0x8d, 2, (void*)&CO_OD_ROM.CANBitRate},
{0x2103, 0x00, 0x8e, 2, (void*)&CO_OD_RAM.SYNCCounter},
{0x2104, 0x00, 0x86, 2, (void*)&CO_OD_RAM.SYNCTime},
{0x2106, 0x00, 0x87, 4, (void*)&CO_OD_EEPROM.powerOnCounter},
{0x2107, 0x05, 0xbe, 2, (void*)&CO_OD_RAM.performance[0]},
{0x2108, 0x01, 0xa6, 2, (void*)&CO_OD_RAM.temperature[0]},
{0x2109, 0x01, 0xa6, 2, (void*)&CO_OD_RAM.voltage[0]},
{0x2110, 0x10, 0xfe, 4, (void*)&CO_OD_RAM.variableInt32[0]},
{0x2111, 0x10, 0xfd, 4, (void*)&CO_OD_ROM.variableROM_Int32[0]},
{0x2112, 0x10, 0xff, 4, (void*)&CO_OD_EEPROM.variableNV_Int32[0]},
{0x2120, 0x05, 0x00, 0, (void*)&OD_record2120},
{0x2130, 0x03, 0x00, 0, (void*)&OD_record2130},
{0x6000, 0x08, 0x66, 1, (void*)&CO_OD_RAM.readInput8Bit[0]},
{0x603f, 0x00, 0xbe, 2, (void*)&CO_OD_RAM.error_code},
{0x6040, 0x00, 0xbe, 2, (void*)&CO_OD_RAM.control_word},
{0x6041, 0x00, 0xbe, 2, (void*)&CO_OD_RAM.status_word},
{0x6060, 0x00, 0x0e, 1, (void*)&CO_OD_RAM.mode_of_operation},
{0x6061, 0x00, 0x25, 1, (void*)&CO_OD_ROM.mode_of_operation_display},
{0x6062, 0x00, 0xa5, 4, (void*)&CO_OD_ROM.custom_demand_pos},
{0x6063, 0x00, 0xa6, 4, (void*)&CO_OD_RAM.motor_actual_pos},
{0x6064, 0x00, 0xa6, 4, (void*)&CO_OD_RAM.custom_actual_pos},
{0x6065, 0x00, 0x8d, 4, (void*)&CO_OD_ROM.following_error_window},
{0x606b, 0x00, 0xa6, 4, (void*)&CO_OD_RAM.custom_actual_velocity_command},
{0x606c, 0x00, 0xa6, 4, (void*)&CO_OD_RAM.custom_actual_velocity},
{0x607f, 0x00, 0x8d, 4, (void*)&CO_OD_ROM.max_profile_velocity},
{0x6085, 0x00, 0x8d, 4, (void*)&CO_OD_ROM.quick_stop_deceleration},
{0x60c1, 0x02, 0xbe, 2, (void*)&CO_OD_RAM.interpolate_data_record[0]},
{0x60c2, 0x00, 0x00, 1, (void*)&OD_record60c2},
{0x60c5, 0x00, 0x8d, 4, (void*)&CO_OD_ROM.max_acceleration},
{0x60c6, 0x00, 0x8d, 4, (void*)&CO_OD_ROM.max_deceleration},
{0x60fc, 0x00, 0xa6, 4, (void*)&CO_OD_RAM.motor_demand_value},
{0x6200, 0x08, 0x3e, 1, (void*)&CO_OD_RAM.writeOutput8Bit[0]},
{0x6401, 0x0c, 0xa6, 2, (void*)&CO_OD_RAM.readAnalogueInput16Bit[0]},
{0x6411, 0x08, 0xbe, 2, (void*)&CO_OD_RAM.writeAnalogueOutput16Bit[0]},
{0x6500, 0x04, 0xbe, 2, (void*)&CO_OD_RAM.imu_adc[0]},
{0x6501, 0x04, 0xbe, 2, (void*)&CO_OD_RAM.enc_inc[0]},
{0x6502, 0x04, 0xbe, 2, (void*)&CO_OD_RAM.enc_abs1[0]},
{0x6503, 0x04, 0xbe, 2, (void*)&CO_OD_RAM.enc_abs2[0]},
};
