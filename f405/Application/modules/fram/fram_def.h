/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : fram_def.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/7/26
* Description : fram data define.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/7/26  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/

     
/**** Includes ****/

     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/
	FRAM_START
	    CLASS     (CL_SOFTWARE_UPDATE_FLAG,         1,                  16,            cl_software_update_flag_array)
	    CLASS     (CL_SOFTWARE_VERSION,                 1,             SW_VERSION_LONGTH,            cl_software_version_array) 
	    CLASS     (CL_PRODUCT_SERIES_NUMBER,         1,                  12,            cl_mb_serial_number_array)
	    CLASS     (CL_HARDWARE_VERSION,                1,                   1,            cl_mb_hw_version_array)
	    CLASS     (CL_FRAM_INIT_FLAG,                 1,                   4,            cl_fram_init_flag_array)
	    CLASS     (CL_MCU_FAULT_DATA,                    1,                   10,        cl_no_init_data_array)
	    CLASS     (CL_FRAM_RESERVE1,                        1,                   1,          cl_no_init_data_array)
	    CLASS     (CL_FRAM_RESERVE2,                        1,                   1,            cl_no_init_data_array)
	    CLASS     (CL_FRAM_SYSTEM_SETTING,                1,                   3,        cl_no_init_data_array)
	    CLASS     (CL_FRAM_ENC_BOURNS,                   1,                   12,        cl_no_init_data_array)
	    CLASS     (CL_FRAM_ENC_TAMAGAWA,                1,                   24,        cl_no_init_data_array)
	    CLASS     (CL_FRAM_IMU_CALIBRATION,           1,                    15,         cl_no_init_data_array)
FRAM_END

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/

/****************************** END OF FILE ***********************************/
