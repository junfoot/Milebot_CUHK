/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : abi_common.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : Main include for ABI (AirBorneInterface).
                @todo explain how to use ABI
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ABI_COMMON_H_
#define _ABI_COMMON_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>
#include "..\std.h"
#include "..\..\power\power_mana_if.h"  
#include "..\..\mcu\uart_if.h"
#include "common.h"


/* Include here headers with structure definition you may want to use with ABI
 * Ex: '#include "..\modules\sensor\imu_if.h"' in order to use the imu structure
 */
#include "abi_sender_ids.h"

/**** Definition of constants ****/


/**** Definition of types ****/ 
/** Generic callback definition */
typedef void (*abi_callback)(void);

/** Event structure to store callbacks in a linked list */
struct abi_struct 
{
  uint8_t id;
  abi_callback cb;
  struct abi_struct *next;
};
typedef struct abi_struct abi_event;

/**** Definition of macros ****/
/* Some magic to avoid to compile C code, only headers */
#ifdef ABI_C
#define ABI_EXTERN
#else
#define ABI_EXTERN extern
#endif

/** Broadcast address.
 * When binding to an ABI message with broadcast address as a sender id,
 * messages from all senders are received.
 */
#define ABI_BROADCAST 255

/** Reserved ABI ID to disable callback.
 * When binding to an ABI message with 0 as sender id,
 * the callback is disabled.
 */
#define ABI_DISABLE 0

/** Macros for linked list */
#define ABI_FOREACH(head,el) for(el=head; el; el=el->next)
#define ABI_PREPEND(head,add) { (add)->next = head; head = add; }
/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


#ifdef __cplusplus
}
#endif

#endif /*_ABI_COMMON_H_*/

/****************************** END OF FILE ***********************************/
