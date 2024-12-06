/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      SERP.h
 *
 * @author    <yourName>
 * @date      <date>
 *
 * @version   0.0.0
 *
 * @brief     <you can add a description of your module here, otherwise please delete this line>
 * @details   <you can add more details of your module here, otherwise please delete this line>
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */
#ifndef SERP_H_
#define SERP_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "Common.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

//Type used to send data
typedef enum SERP_tenuStatus
{
  SERP_eSTATUS_OK                                         = 0,
  SERP_eSTATUS_NO_OK,
  SERP_eSTATUS_NULL_BUFFER
}SERP_tenuStatus;

//Type used for the received data
typedef enum SERP_tenuEvent
{
  SERP_eEVENT_NONE                                         = 0,
  SERP_eEVENT_START_MEASURE,
  SERP_eEVENT_STOP_MEASURE,
  SERP_eEVENT_PACKET_ERROR
}SERP_tenuEvent;

typedef void (*Receive_Callback)(SERP_tenuEvent event);

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/

bool SERP_bSetInterruptHandler(const Receive_Callback Callback_function);

void SERP_Init(void);

SERP_tenuStatus SERP_SendTemperatureValue(int8_t temperature);

SERP_tenuStatus SERP_SendLiveSign(void);

SERP_tenuStatus SERP_SendCustomMessage(char * msg, uint16_t msg_size);

/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* TEMPLATE_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/

