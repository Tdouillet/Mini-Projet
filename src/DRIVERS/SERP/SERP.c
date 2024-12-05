/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      SERP.c
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



/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "SERP.h"
#include "EUSART.h"

/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
#define _XTAL_FREQ                                          4000000UL

//Byte values
#define PACKET_START_BYTE      0x6f
#define PACKET_STOP_BYTE       0x65
#define ESCAPE_BYTE            0x64

//TX Id values
#define TEMPERATURE_ID         18
#define LIVE_SIGN_ID           19
#define CUSTOM_MSG_ID          20

//RX Id values
#define START_MEASURE_ID       17
#define STOP_MEASURE_ID        18

#define LIVE_SIGN_SIZE         0
#define TEMPERATURE_SIZE       1

#define EUSART_TIMEOUT_MS      1  //It takes 86us to send one byte so 1ms should be enough

#define START_BYTE_INDEX       0
#define MSG_ID_INDEX           1
#define LSB_SIZE_INDEX         2
#define MSB_SIZE_INDEX         3
#define STOP_BYTE_INDEX        4

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/

static Receive_Callback Receive_Callback_Function = NULL;

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                       */
/**********************************************************************************************************************/

SERP_tenuStatus SERP_SendFrame(const char * data, char msg_id, uint16_t data_size);

/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

SERP_tenuStatus SERP_SendFrame(const char * data, char msg_id, uint16_t data_size){
    
    SERP_tenuStatus enuStatus = SERP_eSTATUS_NO_OK;
    
    //The data can be empty only if the size entered is 0
    if (data == NULL && data_size != 0){
        enuStatus = SERP_eSTATUS_NULL_BUFFER;
        return enuStatus;
    }
    
    EUSART_vidSendChar(PACKET_START_BYTE,EUSART_TIMEOUT_MS);
    
    if (msg_id == PACKET_START_BYTE || msg_id == PACKET_STOP_BYTE || msg_id == ESCAPE_BYTE){
            EUSART_vidSendChar(ESCAPE_BYTE,EUSART_TIMEOUT_MS);
        }
    EUSART_vidSendChar(msg_id,EUSART_TIMEOUT_MS);
    
    if ((char)data_size == PACKET_START_BYTE || (char)data_size == PACKET_STOP_BYTE || (char)data_size == ESCAPE_BYTE){
            EUSART_vidSendChar(ESCAPE_BYTE,EUSART_TIMEOUT_MS);
        }
    EUSART_vidSendChar((char) data_size,EUSART_TIMEOUT_MS);
    
    if ((char)(data_size>>8) == PACKET_START_BYTE || (char)(data_size>>8) == PACKET_STOP_BYTE || (char)(data_size>>8) == ESCAPE_BYTE){
            EUSART_vidSendChar(ESCAPE_BYTE,EUSART_TIMEOUT_MS);
        }
    EUSART_vidSendChar((char) (data_size>>8),EUSART_TIMEOUT_MS);
    
    //Send every byte of the data buffer (including the escape byte)
    for (uint8_t buf_index = 0; buf_index < data_size; buf_index++){
        if (*(data + buf_index) == PACKET_START_BYTE || *(data + buf_index) == PACKET_STOP_BYTE || *(data + buf_index) == ESCAPE_BYTE){
            EUSART_vidSendChar(ESCAPE_BYTE,EUSART_TIMEOUT_MS);
        }
        EUSART_vidSendChar(*(data + buf_index),EUSART_TIMEOUT_MS);
    }
    
    EUSART_vidSendChar(PACKET_STOP_BYTE,EUSART_TIMEOUT_MS);
            
    enuStatus = SERP_eSTATUS_OK;
    return enuStatus;
}

void SERP_Callback(char const * const rx_data, const uint8_t rx_size, const EUSART_tenuStatus enuStatus){
    
    SERP_tenuEvent rx_msg = SERP_eEVENT_NONE;
    static uint8_t byte_count = START_BYTE_INDEX;
    
    //Check every byte to ensure the integrity of the received packet
    switch (byte_count){
        
        case START_BYTE_INDEX:
            if (*rx_data == PACKET_START_BYTE){
                byte_count++;
            } else {
                rx_msg = SERP_eEVENT_FRAME_ERROR;
            }
            break;
        
        case MSG_ID_INDEX:
            if (*rx_data == START_MEASURE_ID){
                rx_msg = SERP_eEVENT_START_MEASURE;
                byte_count++;
            } else if (*rx_data == STOP_MEASURE_ID){
                rx_msg = SERP_eEVENT_STOP_MEASURE;
                byte_count++;
            } else {
                rx_msg = SERP_eEVENT_FRAME_ERROR;
                byte_count = START_BYTE_INDEX;
            }
            break;
            
        case LSB_SIZE_INDEX:
            if (*rx_data == (char)LIVE_SIGN_SIZE){
                byte_count++;
            } else {
                rx_msg = SERP_eEVENT_FRAME_ERROR;
                byte_count = START_BYTE_INDEX;
            }
            break;
            
        case MSB_SIZE_INDEX:
            if (*rx_data == (char)LIVE_SIGN_SIZE >> 8){
                byte_count++;
            } else {
                rx_msg = SERP_eEVENT_FRAME_ERROR;
                byte_count = START_BYTE_INDEX;
            }
            break;
            
        case STOP_BYTE_INDEX:
            if (*rx_data == PACKET_STOP_BYTE){
                byte_count++;
            } else {
                rx_msg = SERP_eEVENT_FRAME_ERROR;
                byte_count = START_BYTE_INDEX;
            }
            break;
    }    
    
    //Send the corresponding message to the upper layer
    if (rx_msg == SERP_eEVENT_NONE || rx_msg == SERP_eEVENT_FRAME_ERROR){
        Receive_Callback_Function(rx_msg);
        byte_count = START_BYTE_INDEX;
        rx_msg = SERP_eEVENT_NONE;
    }
    
}


/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

bool SERP_bSetInterruptHandler(const Receive_Callback Callback_function){
    
    bool handlerSet = false;
    // On vérifie toujours les paramètres en entrée d'une fonction publique:
    if(Callback_function != NULL)
    {
        Receive_Callback_Function = (Receive_Callback)Callback_function;
        handlerSet = true;
    }
    return handlerSet;
    
}

void SERP_Init(void) {
    
    (void)EUSART_enuRegisterRxCbk(SERP_Callback);
    
}



SERP_tenuStatus SERP_SendTemperatureValue(int8_t temperature){
    
    SERP_tenuStatus enuStatus = SERP_eSTATUS_NO_OK;     
    char char_temperature = (char)temperature;
    char * data = &char_temperature;
    enuStatus = SERP_SendFrame(data, TEMPERATURE_ID, TEMPERATURE_SIZE);
    return enuStatus;
    
}

SERP_tenuStatus SERP_SendLiveSign(void){
    
    SERP_tenuStatus enuStatus = SERP_eSTATUS_NO_OK; 
    char * data = NULL;
    enuStatus = SERP_SendFrame(data, LIVE_SIGN_ID, LIVE_SIGN_SIZE);
    return enuStatus;
}

SERP_tenuStatus SERP_SendCustomMessage(char * msg, uint16_t msg_size){
    
    SERP_tenuStatus enuStatus = SERP_eSTATUS_NO_OK; 
    enuStatus = SERP_SendFrame(msg, CUSTOM_MSG_ID, msg_size);
    return enuStatus;
}



/*--------------------------------------------------------------------------------------------------------------------*/
