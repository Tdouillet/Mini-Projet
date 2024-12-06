/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      AppManager.c
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
#include "AppManager.h"
#include "GPIO.h"
#include "TIMER.h"
#include "LCD.h"
#include "MCP9700.h"
#include "SERP.h"

/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/

#define COMPARE_VALUE_500MS 250
#define IDLE_MSG_SIZE 13
#define START_ROW 0
#define START_COLUMN 0

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/

APPM_enuEvent event_flag = eEVENT_NONE;
SERP_tenuEvent SERP_event = SERP_eEVENT_NONE;
const char IDLE_mode_buffer[IDLE_MSG_SIZE] = "IDLE mode\n";

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                       */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

void LED_Callback_Function(void){
    event_flag = eEVENT_BUTTON;
}

void TIMER_Callback_Function(void){
    event_flag = eEVENT_TIMER;
}

void EUSART_Callback_Function(SERP_tenuEvent event){
    event_flag = eEVENT_USART;
    SERP_event = event;
    
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

void App_Init(void) {
    
    //Initialize the callback functions
    (void)LED_SetInterruptHandler(LED_Callback_Function);
    (void)TIM0_bRegisterTimerCbk(TIMER_Callback_Function);
    (void)SERP_bSetInterruptHandler(EUSART_Callback_Function);
    
}

void App_vidStart(void){
           
    APPM_enuStatus app_status = sAPP_SUSPENDED;
    int8_t temperature = 0;
    
    //The timer is started and never stopped
    TIM0_bStart(TIM0_eLFINTOSC,TIM0_ePRESCALER_64,TIM0_ePOSTSCALER_1,COMPARE_VALUE_500MS);
    
    while(1)
    {
        event_flag = eEVENT_NONE;
        
        while(event_flag == eEVENT_NONE)
            
        switch (app_status){
            
            //Stop measure mode
            case sAPP_SUSPENDED:
                
                switch (event_flag){
                    
                    case eEVENT_BUTTON:
                        app_status = sAPP_RUNNING;
                        break;
                        
                    case eEVENT_TIMER:
                        //Send the live sign and a custom message to the HMI
                        SERP_SendLiveSign();
                        char * buf_pointer = IDLE_mode_buffer;
                        SERP_SendCustomMessage(buf_pointer, IDLE_MSG_SIZE);
                        break;
                        
                    //Change the app's state based on the HMI's message
                    case eEVENT_USART:
                        if (SERP_event == SERP_eEVENT_START_MEASURE){
                            app_status = sAPP_RUNNING;
                        }else if (SERP_event == SERP_eEVENT_STOP_MEASURE){
                            app_status = sAPP_SUSPENDED;
                        }
                        break;
                }
                
                break;
            
            //Start measure mode
            case sAPP_RUNNING:
                
                switch (event_flag){
                    
                    case eEVENT_BUTTON:
                     
                        app_status = sAPP_SUSPENDED;
                        
                        //Force the LED's state to 0
                        LED_SetLow();
                        
                        //Write IDLE mode text on the LCD
                        LCD_enuClearAll(LCD_eDEVICE_ID_DISPLAY);
                        LCD_enuSetCursor(LCD_eDEVICE_ID_DISPLAY,START_COLUMN,START_ROW);
                        LCD_enuWriteText(LCD_eDEVICE_ID_DISPLAY,IDLE_mode_buffer);

                        break;
                        
                    case eEVENT_TIMER:
                      
                        //Toggle LED and calculate the temperature
                        LED_Toggle();
                        temperature = MCP9700_GetTemperature();
                        
                        //Send the live sign and the temperature to the HMI
                        SERP_SendLiveSign();
                        SERP_SendTemperatureValue(temperature);
                        
                        //Send the temperature to the LCD
                        LCD_enuClearAll(LCD_eDEVICE_ID_DISPLAY);
                        LCD_enuSetCursor(LCD_eDEVICE_ID_DISPLAY,START_COLUMN,START_ROW);
                        LCD_enuPrintf(LCD_eDEVICE_ID_DISPLAY,"Temperature : %d",temperature);
                        
                        break;
                        
                    //Change the app's state based on the HMI's message
                    case eEVENT_USART:
                        if (SERP_event == SERP_eEVENT_START_MEASURE){
                            app_status = sAPP_RUNNING;
                        }else if (SERP_event == SERP_eEVENT_STOP_MEASURE){
                            app_status = sAPP_SUSPENDED;
                        }
                        break;
                    
                }
                
                break;
        }          
    
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/








