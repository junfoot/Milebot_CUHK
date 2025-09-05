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



#ifndef _CO_OD_H_
#define _CO_OD_H_   
#pragma anon_unions

/*******************************************************************************
   CANopen DATA TYPES
*******************************************************************************/
   typedef bool_t       BOOLEAN;
   typedef uint8_t      UNSIGNED8;
   typedef uint16_t     UNSIGNED16;
   typedef uint32_t     UNSIGNED32;
   typedef uint64_t     UNSIGNED64;
   typedef int8_t       INTEGER8;
   typedef int16_t      INTEGER16;
   typedef int32_t      INTEGER32;
   typedef int64_t      INTEGER64;
   typedef float32_t    REAL32; 
   typedef float64_t    REAL64; 
   typedef char_t       VISIBLE_STRING;
   typedef oChar_t      OCTET_STRING;

   #ifdef DOMAIN
   #undef DOMAIN
   #endif

   typedef domain_t     DOMAIN;

#ifndef timeOfDay_t
    typedef union {
        unsigned long long ullValue;
        struct {
            unsigned long ms:28;
            unsigned reserved:4;
            unsigned days:16;
            unsigned reserved2:16;
        };
    }timeOfDay_t;
#endif

    typedef timeOfDay_t TIME_OF_DAY;
    typedef timeOfDay_t TIME_DIFFERENCE;


/*******************************************************************************
   FILE INFO:
      FileName:     _project_rt_exchange_0801.xml
      FileVersion:  0
      CreationTime: 
      CreationDate: 
      CreatedBy:    WB
******************************************************************************/


/*******************************************************************************
   DEVICE INFO:
      VendorName:     MileBot
      VendorNumber    0
      ProductName:    axo_can
      ProductNumber:  0
******************************************************************************/


/*******************************************************************************
   FEATURES
*******************************************************************************/
  #define CO_NO_SYNC                     1   //Associated objects: 1005-1007
  #define CO_NO_EMERGENCY                1   //Associated objects: 1014, 1015
  #define CO_NO_SDO_SERVER               1   //Associated objects: 1200-127F
  #define CO_NO_SDO_CLIENT               1   //Associated objects: 1280-12FF
  #define CO_NO_LSS_SERVER               0   //LSS Slave
  #define CO_NO_LSS_CLIENT               0   //LSS Master
  #define CO_NO_RPDO                     1   //Associated objects: 14xx, 16xx
  #define CO_NO_TPDO                     1   //Associated objects: 18xx, 1Axx
  #define CO_NO_NMT_MASTER               1


/*******************************************************************************
   OBJECT DICTIONARY
*******************************************************************************/
   #define CO_OD_NoOfElements             70


/*******************************************************************************
   TYPE DEFINITIONS FOR RECORDS
*******************************************************************************/
/*1018    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     vendorID;
               UNSIGNED32     productCode;
               UNSIGNED32     revisionNumber;
               UNSIGNED32     serialNumber;
               }              OD_identity_t;
/*1200    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDClientToServer;
               UNSIGNED32     COB_IDServerToClient;
               }              OD_SDOServerParameter_t;
/*1280    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDClientToServer;
               UNSIGNED32     COB_IDServerToClient;
               UNSIGNED8      nodeIDOfTheSDOServer;
               }              OD_SDOClientParameter_t;
/*1400    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDUsedByRPDO;
               UNSIGNED8      transmissionType;
               }              OD_RPDOCommunicationParameter_t;
/*1600    */ typedef struct {
               UNSIGNED8      numberOfMappedObjects;
               UNSIGNED32     mappedObject1;
               UNSIGNED32     mappedObject2;
               UNSIGNED32     mappedObject3;
               UNSIGNED32     mappedObject4;
               UNSIGNED32     mappedObject5;
               UNSIGNED32     mappedObject6;
               UNSIGNED32     mappedObject7;
               UNSIGNED32     mappedObject8;
               }              OD_RPDOMappingParameter_t;
/*1800    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDUsedByTPDO;
               UNSIGNED8      transmissionType;
               UNSIGNED16     inhibitTime;
               UNSIGNED8      comparabilityEntry;
               UNSIGNED16     eventTimer;
               UNSIGNED8      SYNCStartValue;
               }              OD_TPDOCommunicationParameter_t;
/*1a00    */ typedef struct {
               UNSIGNED8      numberOfMappedObjects;
               UNSIGNED32     mappedObject1;
               UNSIGNED32     mappedObject2;
               UNSIGNED32     mappedObject3;
               UNSIGNED32     mappedObject4;
               UNSIGNED32     mappedObject5;
               UNSIGNED32     mappedObject6;
               UNSIGNED32     mappedObject7;
               UNSIGNED32     mappedObject8;
               }              OD_TPDOMappingParameter_t;
/*2120    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               INTEGER64      I64;
               UNSIGNED64     U64;
               REAL32         R32;
               REAL64         R64;
               DOMAIN         domain;
               }              OD_testVar_t;
/*2130    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               VISIBLE_STRING string[30];
               UNSIGNED64     epochTimeBaseMs;
               UNSIGNED32     epochTimeOffsetMs;
               }              OD_time_t;
/*2301    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     size;
               UNSIGNED8      axisNo;
               VISIBLE_STRING name[30];
               VISIBLE_STRING color[20];
               UNSIGNED32     map;
               UNSIGNED8      format;
               UNSIGNED8      trigger;
               INTEGER32      threshold;
               }              OD_traceConfig_t;
/*2401    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     size;
               INTEGER32      value;
               INTEGER32      min;
               INTEGER32      max;
               DOMAIN         plot;
               UNSIGNED32     triggerTime;
               }              OD_trace_t;
/*60c2    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               }              OD_interpolation_time_period_t;

/*******************************************************************************
   TYPE DEFINITIONS FOR OBJECT DICTIONARY INDEXES

   some of those are redundant with CO_SDO.h CO_ObjDicId_t <Common CiA301 object 
   dictionary entries>
*******************************************************************************/
/*1000 */
        #define OD_1000_deviceType                                  0x1000

/*1001 */
        #define OD_1001_errorRegister                               0x1001

/*1002 */
        #define OD_1002_manufacturerStatusRegister                  0x1002

/*1003 */
        #define OD_1003_preDefinedErrorField                        0x1003

        #define OD_1003_0_preDefinedErrorField_maxSubIndex          0
        #define OD_1003_1_preDefinedErrorField_standardErrorField   1
        #define OD_1003_2_preDefinedErrorField_standardErrorField   2
        #define OD_1003_3_preDefinedErrorField_standardErrorField   3
        #define OD_1003_4_preDefinedErrorField_standardErrorField   4
        #define OD_1003_5_preDefinedErrorField_standardErrorField   5
        #define OD_1003_6_preDefinedErrorField_standardErrorField   6
        #define OD_1003_7_preDefinedErrorField_standardErrorField   7
        #define OD_1003_8_preDefinedErrorField_standardErrorField   8

/*1005 */
        #define OD_1005_COB_ID_SYNCMessage                          0x1005

/*1006 */
        #define OD_1006_communicationCyclePeriod                    0x1006

/*1007 */
        #define OD_1007_synchronousWindowLength                     0x1007

/*1008 */
        #define OD_1008_manufacturerDeviceName                      0x1008

/*1009 */
        #define OD_1009_manufacturerHardwareVersion                 0x1009

/*100a */
        #define OD_100a_manufacturerSoftwareVersion                 0x100a

/*100c */
        #define OD_100c_guardTime                                   0x100c

/*100d */
        #define OD_100d_lifeTimeFactor                              0x100d

/*1010 */
        #define OD_1010_storeParameters                             0x1010

        #define OD_1010_0_storeParameters_maxSubIndex               0
        #define OD_1010_1_storeParameters_saveAllParameters         1

/*1011 */
        #define OD_1011_restoreDefaultParameters                    0x1011

        #define OD_1011_0_restoreDefaultParameters_maxSubIndex      0
        #define OD_1011_1_restoreDefaultParameters_restoreAllDefaultParameters 1

/*1012 */
        #define OD_1012_COB_ID_TIME                                 0x1012

/*1013 */
        #define OD_1013_highResolutionTimeStamp                     0x1013

/*1014 */
        #define OD_1014_COB_ID_EMCY                                 0x1014

/*1015 */
        #define OD_1015_inhibitTimeEMCY                             0x1015

/*1016 */
        #define OD_1016_consumerHeartbeatTime                       0x1016

        #define OD_1016_0_consumerHeartbeatTime_maxSubIndex         0
        #define OD_1016_1_consumerHeartbeatTime_consumerHeartbeatTime 1
        #define OD_1016_2_consumerHeartbeatTime_consumerHeartbeatTime 2
        #define OD_1016_3_consumerHeartbeatTime_consumerHeartbeatTime 3
        #define OD_1016_4_consumerHeartbeatTime_consumerHeartbeatTime 4

/*1017 */
        #define OD_1017_producerHeartbeatTime                       0x1017

/*1018 */
        #define OD_1018_identity                                    0x1018

        #define OD_1018_0_identity_maxSubIndex                      0
        #define OD_1018_1_identity_vendorID                         1
        #define OD_1018_2_identity_productCode                      2
        #define OD_1018_3_identity_revisionNumber                   3
        #define OD_1018_4_identity_serialNumber                     4

/*1019 */
        #define OD_1019_synchronousCounterOverflowValue             0x1019

/*1029 */
        #define OD_1029_errorBehavior                               0x1029

        #define OD_1029_0_errorBehavior_maxSubIndex                 0
        #define OD_1029_1_errorBehavior_communication               1
        #define OD_1029_2_errorBehavior_communicationOther          2
        #define OD_1029_3_errorBehavior_communicationPassive        3
        #define OD_1029_4_errorBehavior_generic                     4
        #define OD_1029_5_errorBehavior_deviceProfile               5
        #define OD_1029_6_errorBehavior_manufacturerSpecific        6

/*1200 */
        #define OD_1200_SDOServerParameter                          0x1200

        #define OD_1200_0_SDOServerParameter_maxSubIndex            0
        #define OD_1200_1_SDOServerParameter_COB_IDClientToServer   1
        #define OD_1200_2_SDOServerParameter_COB_IDServerToClient   2

/*1280 */
        #define OD_1280_SDOClientParameter                          0x1280

        #define OD_1280_0_SDOClientParameter_maxSubIndex            0
        #define OD_1280_1_SDOClientParameter_COB_IDClientToServer   1
        #define OD_1280_2_SDOClientParameter_COB_IDServerToClient   2
        #define OD_1280_3_SDOClientParameter_nodeIDOfTheSDOServer   3

/*1400 */
        #define OD_1400_RPDOCommunicationParameter                  0x1400

        #define OD_1400_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1400_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1400_2_RPDOCommunicationParameter_transmissionType 2

/*1600 */
        #define OD_1600_RPDOMappingParameter                        0x1600

        #define OD_1600_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1600_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1600_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1600_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1600_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1600_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1600_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1600_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1600_8_RPDOMappingParameter_mappedObject8        8

/*1800 */
        #define OD_1800_TPDOCommunicationParameter                  0x1800

        #define OD_1800_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1800_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1800_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1800_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1800_4_TPDOCommunicationParameter_comparabilityEntry 4
        #define OD_1800_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1800_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1a00 */
        #define OD_1a00_TPDOMappingParameter                        0x1a00

        #define OD_1a00_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a00_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a00_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a00_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a00_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a00_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a00_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a00_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a00_8_TPDOMappingParameter_mappedObject8        8

/*1f80 */
        #define OD_1f80_NMTStartup                                  0x1f80

/*2100 */
        #define OD_2100_errorStatusBits                             0x2100

/*2101 */
        #define OD_2101_CANNodeID                                   0x2101

/*2102 */
        #define OD_2102_CANBitRate                                  0x2102

/*2103 */
        #define OD_2103_SYNCCounter                                 0x2103

/*2104 */
        #define OD_2104_SYNCTime                                    0x2104

/*2106 */
        #define OD_2106_powerOnCounter                              0x2106

/*2107 */
        #define OD_2107_performance                                 0x2107

        #define OD_2107_0_performance_maxSubIndex                   0
        #define OD_2107_1_performance_cyclesPerSecond               1
        #define OD_2107_2_performance_timerCycleTime                2
        #define OD_2107_3_performance_timerCycleMaxTime             3
        #define OD_2107_4_performance_mainCycleTime                 4
        #define OD_2107_5_performance_mainCycleMaxTime              5

/*2108 */
        #define OD_2108_temperature                                 0x2108

        #define OD_2108_0_temperature_maxSubIndex                   0
        #define OD_2108_1_temperature_mainPCB                       1

/*2109 */
        #define OD_2109_voltage                                     0x2109

        #define OD_2109_0_voltage_maxSubIndex                       0
        #define OD_2109_1_voltage_mainPCBSupply                     1

/*2110 */
        #define OD_2110_variableInt32                               0x2110

        #define OD_2110_0_variableInt32_maxSubIndex                 0
        #define OD_2110_1_variableInt32_int32                       1
        #define OD_2110_2_variableInt32_int32                       2
        #define OD_2110_3_variableInt32_int32                       3
        #define OD_2110_4_variableInt32_int32                       4
        #define OD_2110_5_variableInt32_int32                       5
        #define OD_2110_6_variableInt32_int32                       6
        #define OD_2110_7_variableInt32_int32                       7
        #define OD_2110_8_variableInt32_int32                       8
        #define OD_2110_9_variableInt32_int32                       9
        #define OD_2110_10_variableInt32_int32                      10
        #define OD_2110_11_variableInt32_int32                      11
        #define OD_2110_12_variableInt32_int32                      12
        #define OD_2110_13_variableInt32_int32                      13
        #define OD_2110_14_variableInt32_int32                      14
        #define OD_2110_15_variableInt32_int32                      15
        #define OD_2110_16_variableInt32_int32                      16

/*2111 */
        #define OD_2111_variableROM_Int32                           0x2111

        #define OD_2111_0_variableROM_Int32_maxSubIndex             0
        #define OD_2111_1_variableROM_Int32_int32                   1
        #define OD_2111_2_variableROM_Int32_int32                   2
        #define OD_2111_3_variableROM_Int32_int32                   3
        #define OD_2111_4_variableROM_Int32_int32                   4
        #define OD_2111_5_variableROM_Int32_int32                   5
        #define OD_2111_6_variableROM_Int32_int32                   6
        #define OD_2111_7_variableROM_Int32_int32                   7
        #define OD_2111_8_variableROM_Int32_int32                   8
        #define OD_2111_9_variableROM_Int32_int32                   9
        #define OD_2111_10_variableROM_Int32_int32                  10
        #define OD_2111_11_variableROM_Int32_int32                  11
        #define OD_2111_12_variableROM_Int32_int32                  12
        #define OD_2111_13_variableROM_Int32_int32                  13
        #define OD_2111_14_variableROM_Int32_int32                  14
        #define OD_2111_15_variableROM_Int32_int32                  15
        #define OD_2111_16_variableROM_Int32_int32                  16

/*2112 */
        #define OD_2112_variableNV_Int32                            0x2112

        #define OD_2112_0_variableNV_Int32_maxSubIndex              0
        #define OD_2112_1_variableNV_Int32_int32                    1
        #define OD_2112_2_variableNV_Int32_int32                    2
        #define OD_2112_3_variableNV_Int32_int32                    3
        #define OD_2112_4_variableNV_Int32_int32                    4
        #define OD_2112_5_variableNV_Int32_int32                    5
        #define OD_2112_6_variableNV_Int32_int32                    6
        #define OD_2112_7_variableNV_Int32_int32                    7
        #define OD_2112_8_variableNV_Int32_int32                    8
        #define OD_2112_9_variableNV_Int32_int32                    9
        #define OD_2112_10_variableNV_Int32_int32                   10
        #define OD_2112_11_variableNV_Int32_int32                   11
        #define OD_2112_12_variableNV_Int32_int32                   12
        #define OD_2112_13_variableNV_Int32_int32                   13
        #define OD_2112_14_variableNV_Int32_int32                   14
        #define OD_2112_15_variableNV_Int32_int32                   15
        #define OD_2112_16_variableNV_Int32_int32                   16

/*2120 */
        #define OD_2120_testVar                                     0x2120

        #define OD_2120_0_testVar_maxSubIndex                       0
        #define OD_2120_1_testVar_I64                               1
        #define OD_2120_2_testVar_U64                               2
        #define OD_2120_3_testVar_R32                               3
        #define OD_2120_4_testVar_R64                               4
        #define OD_2120_5_testVar_domain                            5

/*2130 */
        #define OD_2130_time                                        0x2130

        #define OD_2130_0_time_maxSubIndex                          0
        #define OD_2130_1_time_string                               1
        #define OD_2130_2_time_epochTimeBaseMs                      2
        #define OD_2130_3_time_epochTimeOffsetMs                    3

/*6000 */
        #define OD_6000_readInput8Bit                               0x6000

        #define OD_6000_0_readInput8Bit_maxSubIndex                 0
        #define OD_6000_1_readInput8Bit_input                       1
        #define OD_6000_2_readInput8Bit_input                       2
        #define OD_6000_3_readInput8Bit_input                       3
        #define OD_6000_4_readInput8Bit_input                       4
        #define OD_6000_5_readInput8Bit_input                       5
        #define OD_6000_6_readInput8Bit_input                       6
        #define OD_6000_7_readInput8Bit_input                       7
        #define OD_6000_8_readInput8Bit_input                       8

/*603f */
        #define OD_603f_error_code                                  0x603f

/*6040 */
        #define OD_6040_control_word                                0x6040

/*6041 */
        #define OD_6041_status_word                                 0x6041

/*6060 */
        #define OD_6060_mode_of_operation                           0x6060

/*6061 */
        #define OD_6061_mode_of_operation_display                   0x6061

/*6062 */
        #define OD_6062_custom_demand_pos                           0x6062

/*6063 */
        #define OD_6063_motor_actual_pos                            0x6063

/*6064 */
        #define OD_6064_custom_actual_pos                           0x6064

/*6065 */
        #define OD_6065_following_error_window                      0x6065

/*606b */
        #define OD_606b_custom_actual_velocity_command              0x606b

/*606c */
        #define OD_606c_custom_actual_velocity                      0x606c

/*607f */
        #define OD_607f_max_profile_velocity                        0x607f

/*6085 */
        #define OD_6085_quick_stop_deceleration                     0x6085

/*60c1 */
        #define OD_60c1_interpolate_data_record                     0x60c1

        #define OD_60c1_0_interpolate_data_record_maxSubIndex       0
        #define OD_60c1_1_interpolate_data_record_                  1
        #define OD_60c1_2_interpolate_data_record_                  2

/*60c2 */
        #define OD_60c2_interpolation_time_period                   0x60c2

        #define OD_60c2_0_interpolation_time_period_maxSubIndex     0

/*60c5 */
        #define OD_60c5_max_acceleration                            0x60c5

/*60c6 */
        #define OD_60c6_max_deceleration                            0x60c6

/*60fc */
        #define OD_60fc_motor_demand_value                          0x60fc

/*6200 */
        #define OD_6200_writeOutput8Bit                             0x6200

        #define OD_6200_0_writeOutput8Bit_maxSubIndex               0
        #define OD_6200_1_writeOutput8Bit_output                    1
        #define OD_6200_2_writeOutput8Bit_output                    2
        #define OD_6200_3_writeOutput8Bit_output                    3
        #define OD_6200_4_writeOutput8Bit_output                    4
        #define OD_6200_5_writeOutput8Bit_output                    5
        #define OD_6200_6_writeOutput8Bit_output                    6
        #define OD_6200_7_writeOutput8Bit_output                    7
        #define OD_6200_8_writeOutput8Bit_output                    8

/*6401 */
        #define OD_6401_readAnalogueInput16Bit                      0x6401

        #define OD_6401_0_readAnalogueInput16Bit_maxSubIndex        0
        #define OD_6401_1_readAnalogueInput16Bit_input              1
        #define OD_6401_2_readAnalogueInput16Bit_input              2
        #define OD_6401_3_readAnalogueInput16Bit_input              3
        #define OD_6401_4_readAnalogueInput16Bit_input              4
        #define OD_6401_5_readAnalogueInput16Bit_input              5
        #define OD_6401_6_readAnalogueInput16Bit_input              6
        #define OD_6401_7_readAnalogueInput16Bit_input              7
        #define OD_6401_8_readAnalogueInput16Bit_input              8
        #define OD_6401_9_readAnalogueInput16Bit_input              9
        #define OD_6401_10_readAnalogueInput16Bit_input             10
        #define OD_6401_11_readAnalogueInput16Bit_input             11
        #define OD_6401_12_readAnalogueInput16Bit_input             12

/*6411 */
        #define OD_6411_writeAnalogueOutput16Bit                    0x6411

        #define OD_6411_0_writeAnalogueOutput16Bit_maxSubIndex      0
        #define OD_6411_1_writeAnalogueOutput16Bit_output           1
        #define OD_6411_2_writeAnalogueOutput16Bit_output           2
        #define OD_6411_3_writeAnalogueOutput16Bit_output           3
        #define OD_6411_4_writeAnalogueOutput16Bit_output           4
        #define OD_6411_5_writeAnalogueOutput16Bit_output           5
        #define OD_6411_6_writeAnalogueOutput16Bit_output           6
        #define OD_6411_7_writeAnalogueOutput16Bit_output           7
        #define OD_6411_8_writeAnalogueOutput16Bit_output           8

/*6500 */
        #define OD_6500_imu_adc                                     0x6500

        #define OD_6500_0_imu_adc_maxSubIndex                       0
        #define OD_6500_1_imu_adc_output                            1
        #define OD_6500_2_imu_adc_output                            2
        #define OD_6500_3_imu_adc_output                            3
        #define OD_6500_4_imu_adc_output                            4
        #define OD_6500_5_imu_adc_output                            5
        #define OD_6500_6_imu_adc_output                            6
        #define OD_6500_7_imu_adc_output                            7
        #define OD_6500_8_imu_adc_output                            8

/*6501 */
        #define OD_6501_enc_inc                                     0x6501

        #define OD_6501_0_enc_inc_maxSubIndex                       0
        #define OD_6501_1_enc_inc_output                            1
        #define OD_6501_2_enc_inc_output                            2
        #define OD_6501_3_enc_inc_output                            3
        #define OD_6501_4_enc_inc_output                            4
        #define OD_6501_5_enc_inc_output                            5
        #define OD_6501_6_enc_inc_output                            6
        #define OD_6501_7_enc_inc_output                            7
        #define OD_6501_8_enc_inc_output                            8

/*6502 */
        #define OD_6502_enc_abs1                                    0x6502

        #define OD_6502_0_enc_abs1_maxSubIndex                      0
        #define OD_6502_1_enc_abs1_output                           1
        #define OD_6502_2_enc_abs1_output                           2
        #define OD_6502_3_enc_abs1_output                           3
        #define OD_6502_4_enc_abs1_output                           4
        #define OD_6502_5_enc_abs1_output                           5
        #define OD_6502_6_enc_abs1_output                           6
        #define OD_6502_7_enc_abs1_output                           7
        #define OD_6502_8_enc_abs1_output                           8

/*6503 */
        #define OD_6503_enc_abs2                                    0x6503

        #define OD_6503_0_enc_abs2_maxSubIndex                      0
        #define OD_6503_1_enc_abs2_output                           1
        #define OD_6503_2_enc_abs2_output                           2
        #define OD_6503_3_enc_abs2_output                           3
        #define OD_6503_4_enc_abs2_output                           4
        #define OD_6503_5_enc_abs2_output                           5
        #define OD_6503_6_enc_abs2_output                           6
        #define OD_6503_7_enc_abs2_output                           7
        #define OD_6503_8_enc_abs2_output                           8

/*******************************************************************************
   STRUCTURES FOR VARIABLES IN DIFFERENT MEMORY LOCATIONS
*******************************************************************************/
#define  CO_OD_FIRST_LAST_WORD     0x55 //Any value from 0x01 to 0xFE. If changed, EEPROM will be reinitialized.

/***** Structure for ROM variables ********************************************/
struct sCO_OD_ROM{
               UNSIGNED32     FirstWord;

/*1000      */ UNSIGNED32      deviceType;
/*1005      */ UNSIGNED32      COB_ID_SYNCMessage;
/*1006      */ UNSIGNED32      communicationCyclePeriod;
/*1007      */ UNSIGNED32      synchronousWindowLength;
/*1008      */ VISIBLE_STRING  manufacturerDeviceName[6];
/*1009      */ VISIBLE_STRING  manufacturerHardwareVersion[4];
/*100a      */ VISIBLE_STRING  manufacturerSoftwareVersion[4];
/*100c      */ UNSIGNED16      guardTime;
/*1012      */ UNSIGNED32      COB_ID_TIME;
/*1014      */ UNSIGNED32      COB_ID_EMCY;
/*1015      */ UNSIGNED16      inhibitTimeEMCY;
/*1016      */ UNSIGNED32      consumerHeartbeatTime[4];
/*1017      */ UNSIGNED16      producerHeartbeatTime;
/*1018      */ OD_identity_t   identity;
/*1019      */ UNSIGNED8       synchronousCounterOverflowValue;
/*1029      */ UNSIGNED8       errorBehavior[6];
/*1200      */ OD_SDOServerParameter_t SDOServerParameter[1];
/*1400      */ OD_RPDOCommunicationParameter_t RPDOCommunicationParameter[1];
/*1600      */ OD_RPDOMappingParameter_t RPDOMappingParameter[1];
/*1800      */ OD_TPDOCommunicationParameter_t TPDOCommunicationParameter[1];
/*1a00      */ OD_TPDOMappingParameter_t TPDOMappingParameter[1];
/*1f80      */ UNSIGNED32      NMTStartup;
/*2101      */ UNSIGNED8       CANNodeID;
/*2102      */ UNSIGNED16      CANBitRate;
/*2111      */ INTEGER32       variableROM_Int32[16];
/*6061      */ INTEGER8        mode_of_operation_display;
/*6062      */ INTEGER32       custom_demand_pos;
/*6065      */ UNSIGNED32      following_error_window;
/*607f      */ UNSIGNED32      max_profile_velocity;
/*6085      */ UNSIGNED32      quick_stop_deceleration;
/*60c2      */ OD_interpolation_time_period_t interpolation_time_period;
/*60c5      */ INTEGER32       max_acceleration;
/*60c6      */ INTEGER32       max_deceleration;

               UNSIGNED32     LastWord;
};

/***** Structure for RAM variables ********************************************/
struct sCO_OD_RAM{
               UNSIGNED32     FirstWord;

/*1001      */ UNSIGNED8       errorRegister;
/*1002      */ UNSIGNED32      manufacturerStatusRegister;
/*1003      */ UNSIGNED32      preDefinedErrorField[8];
/*100d      */ UNSIGNED8       lifeTimeFactor;
/*1010      */ UNSIGNED32      storeParameters[1];
/*1011      */ UNSIGNED32      restoreDefaultParameters[1];
/*1013      */ UNSIGNED32      highResolutionTimeStamp;
/*1280      */ OD_SDOClientParameter_t SDOClientParameter[1];
/*2100      */ OCTET_STRING    errorStatusBits[10];
/*2103      */ UNSIGNED16      SYNCCounter;
/*2104      */ UNSIGNED16      SYNCTime;
/*2107      */ UNSIGNED16      performance[5];
/*2108      */ INTEGER16       temperature[1];
/*2109      */ INTEGER16       voltage[1];
/*2110      */ INTEGER32       variableInt32[16];
/*2120      */ OD_testVar_t    testVar;
/*2130      */ OD_time_t       time;
/*6000      */ UNSIGNED8       readInput8Bit[8];
/*603f      */ UNSIGNED16      error_code;
/*6040      */ UNSIGNED16      control_word;
/*6041      */ UNSIGNED16      status_word;
/*6060      */ INTEGER8        mode_of_operation;
/*6063      */ INTEGER32       motor_actual_pos;
/*6064      */ INTEGER32       custom_actual_pos;
/*606b      */ INTEGER32       custom_actual_velocity_command;
/*606c      */ INTEGER32       custom_actual_velocity;
/*60c1      */ INTEGER16       interpolate_data_record[2];
/*60fc      */ INTEGER32       motor_demand_value;
/*6200      */ UNSIGNED8       writeOutput8Bit[8];
/*6401      */ INTEGER16       readAnalogueInput16Bit[12];
/*6411      */ INTEGER16       writeAnalogueOutput16Bit[8];
/*6500      */ UNSIGNED16      imu_adc[8];
/*6501      */ UNSIGNED16      enc_inc[8];
/*6502      */ UNSIGNED16      enc_abs1[8];
/*6503      */ UNSIGNED16      enc_abs2[8];

               UNSIGNED32     LastWord;
};

/***** Structure for EEPROM variables ********************************************/
struct sCO_OD_EEPROM{
               UNSIGNED32     FirstWord;

/*2106      */ UNSIGNED32      powerOnCounter;
/*2112      */ INTEGER32       variableNV_Int32[16];

               UNSIGNED32     LastWord;
};

/***** Declaration of Object Dictionary variables *****************************/
extern struct sCO_OD_ROM CO_OD_ROM;

extern struct sCO_OD_RAM CO_OD_RAM;

extern struct sCO_OD_EEPROM CO_OD_EEPROM;

/*******************************************************************************
   ALIASES FOR OBJECT DICTIONARY VARIABLES
*******************************************************************************/
/*1000, Data Type: UNSIGNED32 */
        #define OD_deviceType                                       CO_OD_ROM.deviceType

/*1001, Data Type: UNSIGNED8 */
        #define OD_errorRegister                                    CO_OD_RAM.errorRegister

/*1002, Data Type: UNSIGNED32 */
        #define OD_manufacturerStatusRegister                       CO_OD_RAM.manufacturerStatusRegister

/*1003, Data Type: UNSIGNED32, Array[8] */
        #define OD_preDefinedErrorField                             CO_OD_RAM.preDefinedErrorField
        #define ODL_preDefinedErrorField_arrayLength                8
        #define ODA_preDefinedErrorField_standardErrorField         0

/*1005, Data Type: UNSIGNED32 */
        #define OD_COB_ID_SYNCMessage                               CO_OD_ROM.COB_ID_SYNCMessage

/*1006, Data Type: UNSIGNED32 */
        #define OD_communicationCyclePeriod                         CO_OD_ROM.communicationCyclePeriod

/*1007, Data Type: UNSIGNED32 */
        #define OD_synchronousWindowLength                          CO_OD_ROM.synchronousWindowLength

/*1008, Data Type: VISIBLE_STRING */
        #define OD_manufacturerDeviceName                           CO_OD_ROM.manufacturerDeviceName
        #define ODL_manufacturerDeviceName_stringLength             6

/*1009, Data Type: VISIBLE_STRING */
        #define OD_manufacturerHardwareVersion                      CO_OD_ROM.manufacturerHardwareVersion
        #define ODL_manufacturerHardwareVersion_stringLength        4

/*100a, Data Type: VISIBLE_STRING */
        #define OD_manufacturerSoftwareVersion                      CO_OD_ROM.manufacturerSoftwareVersion
        #define ODL_manufacturerSoftwareVersion_stringLength        4

/*100c, Data Type: UNSIGNED16 */
        #define OD_guardTime                                        CO_OD_ROM.guardTime

/*100d, Data Type: UNSIGNED8 */
        #define OD_lifeTimeFactor                                   CO_OD_RAM.lifeTimeFactor

/*1010, Data Type: UNSIGNED32, Array[1] */
        #define OD_storeParameters                                  CO_OD_RAM.storeParameters
        #define ODL_storeParameters_arrayLength                     1
        #define ODA_storeParameters_saveAllParameters               0

/*1011, Data Type: UNSIGNED32, Array[1] */
        #define OD_restoreDefaultParameters                         CO_OD_RAM.restoreDefaultParameters
        #define ODL_restoreDefaultParameters_arrayLength            1
        #define ODA_restoreDefaultParameters_restoreAllDefaultParameters 0

/*1012, Data Type: UNSIGNED32 */
        #define OD_COB_ID_TIME                                      CO_OD_ROM.COB_ID_TIME

/*1013, Data Type: UNSIGNED32 */
        #define OD_highResolutionTimeStamp                          CO_OD_RAM.highResolutionTimeStamp

/*1014, Data Type: UNSIGNED32 */
        #define OD_COB_ID_EMCY                                      CO_OD_ROM.COB_ID_EMCY

/*1015, Data Type: UNSIGNED16 */
        #define OD_inhibitTimeEMCY                                  CO_OD_ROM.inhibitTimeEMCY

/*1016, Data Type: UNSIGNED32, Array[4] */
        #define OD_consumerHeartbeatTime                            CO_OD_ROM.consumerHeartbeatTime
        #define ODL_consumerHeartbeatTime_arrayLength               4
        #define ODA_consumerHeartbeatTime_consumerHeartbeatTime     0

/*1017, Data Type: UNSIGNED16 */
        #define OD_producerHeartbeatTime                            CO_OD_ROM.producerHeartbeatTime

/*1018, Data Type: identity_t */
        #define OD_identity                                         CO_OD_ROM.identity

/*1019, Data Type: UNSIGNED8 */
        #define OD_synchronousCounterOverflowValue                  CO_OD_ROM.synchronousCounterOverflowValue

/*1029, Data Type: UNSIGNED8, Array[6] */
        #define OD_errorBehavior                                    CO_OD_ROM.errorBehavior
        #define ODL_errorBehavior_arrayLength                       6
        #define ODA_errorBehavior_communication                     0
        #define ODA_errorBehavior_communicationOther                1
        #define ODA_errorBehavior_communicationPassive              2
        #define ODA_errorBehavior_generic                           3
        #define ODA_errorBehavior_deviceProfile                     4
        #define ODA_errorBehavior_manufacturerSpecific              5

/*1200, Data Type: SDOServerParameter_t */
        #define OD_SDOServerParameter                               CO_OD_ROM.SDOServerParameter

/*1280, Data Type: SDOClientParameter_t */
        #define OD_SDOClientParameter                               CO_OD_RAM.SDOClientParameter

/*1400, Data Type: RPDOCommunicationParameter_t */
        #define OD_RPDOCommunicationParameter                       CO_OD_ROM.RPDOCommunicationParameter

/*1600, Data Type: RPDOMappingParameter_t */
        #define OD_RPDOMappingParameter                             CO_OD_ROM.RPDOMappingParameter

/*1800, Data Type: TPDOCommunicationParameter_t */
        #define OD_TPDOCommunicationParameter                       CO_OD_ROM.TPDOCommunicationParameter

/*1a00, Data Type: TPDOMappingParameter_t */
        #define OD_TPDOMappingParameter                             CO_OD_ROM.TPDOMappingParameter

/*1f80, Data Type: UNSIGNED32 */
        #define OD_NMTStartup                                       CO_OD_ROM.NMTStartup

/*2100, Data Type: OCTET_STRING */
        #define OD_errorStatusBits                                  CO_OD_RAM.errorStatusBits
        #define ODL_errorStatusBits_stringLength                    10

/*2101, Data Type: UNSIGNED8 */
        #define OD_CANNodeID                                        CO_OD_ROM.CANNodeID

/*2102, Data Type: UNSIGNED16 */
        #define OD_CANBitRate                                       CO_OD_ROM.CANBitRate

/*2103, Data Type: UNSIGNED16 */
        #define OD_SYNCCounter                                      CO_OD_RAM.SYNCCounter

/*2104, Data Type: UNSIGNED16 */
        #define OD_SYNCTime                                         CO_OD_RAM.SYNCTime

/*2106, Data Type: UNSIGNED32 */
        #define OD_powerOnCounter                                   CO_OD_EEPROM.powerOnCounter

/*2107, Data Type: UNSIGNED16, Array[5] */
        #define OD_performance                                      CO_OD_RAM.performance
        #define ODL_performance_arrayLength                         5
        #define ODA_performance_cyclesPerSecond                     0
        #define ODA_performance_timerCycleTime                      1
        #define ODA_performance_timerCycleMaxTime                   2
        #define ODA_performance_mainCycleTime                       3
        #define ODA_performance_mainCycleMaxTime                    4

/*2108, Data Type: INTEGER16, Array[1] */
        #define OD_temperature                                      CO_OD_RAM.temperature
        #define ODL_temperature_arrayLength                         1
        #define ODA_temperature_mainPCB                             0

/*2109, Data Type: INTEGER16, Array[1] */
        #define OD_voltage                                          CO_OD_RAM.voltage
        #define ODL_voltage_arrayLength                             1
        #define ODA_voltage_mainPCBSupply                           0

/*2110, Data Type: INTEGER32, Array[16] */
        #define OD_variableInt32                                    CO_OD_RAM.variableInt32
        #define ODL_variableInt32_arrayLength                       16
        #define ODA_variableInt32_int32                             0

/*2111, Data Type: INTEGER32, Array[16] */
        #define OD_variableROM_Int32                                CO_OD_ROM.variableROM_Int32
        #define ODL_variableROM_Int32_arrayLength                   16
        #define ODA_variableROM_Int32_int32                         0

/*2112, Data Type: INTEGER32, Array[16] */
        #define OD_variableNV_Int32                                 CO_OD_EEPROM.variableNV_Int32
        #define ODL_variableNV_Int32_arrayLength                    16
        #define ODA_variableNV_Int32_int32                          0

/*2120, Data Type: testVar_t */
        #define OD_testVar                                          CO_OD_RAM.testVar

/*2130, Data Type: time_t */
        #define OD_time                                             CO_OD_RAM.time

/*6000, Data Type: UNSIGNED8, Array[8] */
        #define OD_readInput8Bit                                    CO_OD_RAM.readInput8Bit
        #define ODL_readInput8Bit_arrayLength                       8
        #define ODA_readInput8Bit_input                             0

/*603f, Data Type: UNSIGNED16 */
        #define OD_error_code                                       CO_OD_RAM.error_code

/*6040, Data Type: UNSIGNED16 */
        #define OD_control_word                                     CO_OD_RAM.control_word

/*6041, Data Type: UNSIGNED16 */
        #define OD_status_word                                      CO_OD_RAM.status_word

/*6060, Data Type: INTEGER8 */
        #define OD_mode_of_operation                                CO_OD_RAM.mode_of_operation

/*6061, Data Type: INTEGER8 */
        #define OD_mode_of_operation_display                        CO_OD_ROM.mode_of_operation_display

/*6062, Data Type: INTEGER32 */
        #define OD_custom_demand_pos                                CO_OD_ROM.custom_demand_pos

/*6063, Data Type: INTEGER32 */
        #define OD_motor_actual_pos                                 CO_OD_RAM.motor_actual_pos

/*6064, Data Type: INTEGER32 */
        #define OD_custom_actual_pos                                CO_OD_RAM.custom_actual_pos

/*6065, Data Type: UNSIGNED32 */
        #define OD_following_error_window                           CO_OD_ROM.following_error_window

/*606b, Data Type: INTEGER32 */
        #define OD_custom_actual_velocity_command                   CO_OD_RAM.custom_actual_velocity_command

/*606c, Data Type: INTEGER32 */
        #define OD_custom_actual_velocity                           CO_OD_RAM.custom_actual_velocity

/*607f, Data Type: UNSIGNED32 */
        #define OD_max_profile_velocity                             CO_OD_ROM.max_profile_velocity

/*6085, Data Type: UNSIGNED32 */
        #define OD_quick_stop_deceleration                          CO_OD_ROM.quick_stop_deceleration

/*60c1, Data Type: INTEGER16, Array[2] */
        #define OD_interpolate_data_record                          CO_OD_RAM.interpolate_data_record
        #define ODL_interpolate_data_record_arrayLength             2
        #define ODA_interpolate_data_record_                        0

/*60c2, Data Type: interpolation_time_period_t */
        #define OD_interpolation_time_period                        CO_OD_ROM.interpolation_time_period

/*60c5, Data Type: INTEGER32 */
        #define OD_max_acceleration                                 CO_OD_ROM.max_acceleration

/*60c6, Data Type: INTEGER32 */
        #define OD_max_deceleration                                 CO_OD_ROM.max_deceleration

/*60fc, Data Type: INTEGER32 */
        #define OD_motor_demand_value                               CO_OD_RAM.motor_demand_value

/*6200, Data Type: UNSIGNED8, Array[8] */
        #define OD_writeOutput8Bit                                  CO_OD_RAM.writeOutput8Bit
        #define ODL_writeOutput8Bit_arrayLength                     8
        #define ODA_writeOutput8Bit_output                          0

/*6401, Data Type: INTEGER16, Array[12] */
        #define OD_readAnalogueInput16Bit                           CO_OD_RAM.readAnalogueInput16Bit
        #define ODL_readAnalogueInput16Bit_arrayLength              12
        #define ODA_readAnalogueInput16Bit_input                    0

/*6411, Data Type: INTEGER16, Array[8] */
        #define OD_writeAnalogueOutput16Bit                         CO_OD_RAM.writeAnalogueOutput16Bit
        #define ODL_writeAnalogueOutput16Bit_arrayLength            8
        #define ODA_writeAnalogueOutput16Bit_output                 0

/*6500, Data Type: UNSIGNED16, Array[8] */
        #define OD_imu_adc                                          CO_OD_RAM.imu_adc
        #define ODL_imu_adc_arrayLength                             8
        #define ODA_imu_adc_output                                  0

/*6501, Data Type: UNSIGNED16, Array[8] */
        #define OD_enc_inc                                          CO_OD_RAM.enc_inc
        #define ODL_enc_inc_arrayLength                             8
        #define ODA_enc_inc_output                                  0

/*6502, Data Type: UNSIGNED16, Array[8] */
        #define OD_enc_abs1                                         CO_OD_RAM.enc_abs1
        #define ODL_enc_abs1_arrayLength                            8
        #define ODA_enc_abs1_output                                 0

/*6503, Data Type: UNSIGNED16, Array[8] */
        #define OD_enc_abs2                                         CO_OD_RAM.enc_abs2
        #define ODL_enc_abs2_arrayLength                            8
        #define ODA_enc_abs2_output                                 0

#endif