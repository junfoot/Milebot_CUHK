/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : fram_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/7/26
* Description : extern header file of fram.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/7/26  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FRAM_IF_H_
#define _FRAM_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>
#include "..\..\system\std.h"
#include "fm25v_if.h"
     
/**** Definition of constants ****/


/**** Definition of types ****/ 

struct FRAM_ERROR_INFO
{
    bool_t read_data_fail;
    bool_t write_data_fail;
    bool_t data_wrong;
};
extern struct FRAM_ERROR_INFO fram_error;

enum FRAM_DATA_INIT_SECTION_TYPE
{
    FRAM_DATA_INIT_SECTION_ONE,

    /*Add some section here*/
    FRAM_DATA_INIT_SECTION_MAX
};


/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/
extern const uint8_t fram_init_flags[4];

/**** Declaration of functions ****/
extern void fram_init(void);
extern uint8_t fram_id_write(uint8_t id, const uint8_t *write_buffer);
extern uint8_t fram_write(uint8_t id, uint16_t item, const uint8_t *write_buffer);
extern uint8_t fram_read (uint8_t id, uint16_t item, uint8_t *read_buffer);
extern uint8_t fram_id_read (uint8_t id, uint8_t *read_buffer);
extern uint8_t fram_sn_data_write(uint8_t *buffer);
extern uint8_t fram_sn_data_read(uint8_t *buffer);
extern uint8_t fram_write_mcu_fault(uint8_t *buffer);
extern uint8_t fram_read_mcu_fault(uint8_t *buffer);
extern uint8_t fram_write_system_setting(uint8_t *buffer);
extern uint8_t fram_read_system_setting(uint8_t *buffer);
extern uint8_t fram_write_enc_bourns(uint8_t *buffer);
extern uint8_t fram_read_enc_bourns(uint8_t *buffer);
extern uint8_t fram_write_enc_tawa(uint8_t *buffer);
extern uint8_t fram_read_enc_tawa(uint8_t *buffer);

extern uint8_t fram_write_imu_cali(uint8_t *buffer);
extern uint8_t fram_read_imu_cali(uint8_t *buffer);


#ifdef UPGRADE_OPTION
extern uint8_t fram_write_swdl_mask(void);
extern uint8_t fram_erase_swdl_mask (void);
extern uint8_t fram_read_swdl_mask (uint8_t* pBlockData);
extern bool_t fram_update_is_available(void);
#endif    /* UPGRADE_OPTION */
extern void fram_init_all_data(void);
extern void fram_polling(void);


#ifdef __cplusplus
}
#endif

#endif /*_FRAM_IF_H_*/

/****************************** END OF FILE ***********************************/
