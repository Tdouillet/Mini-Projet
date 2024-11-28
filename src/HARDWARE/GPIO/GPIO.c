/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      GPIO.c
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
#include <xc.h>
#include "ISR.h"
#include "GPIO.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/

static LED_Callback LED_Callback_Function = NULL;

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                       */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

static bool GPIO_Callback(void){
    
    if (PIR0bits.IOCIF == 1){
        if (LED_Callback_Function != NULL){
            LED_Callback_Function();
            GPIO_ClearIntFlags();
        }   
    }
    
    return true;
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

void GPIO_ClearIntFlags(void){
    
    IOCBFbits.IOCBF4 = 0;    
    
}

void LED_SetLow(void){
    
    LATAbits.LATA4 = 0; 
};



void LED_SetHigh(void){
    
    LATAbits.LATA4 = 1;
};



void LED_Toggle(){
    
    if (PORTAbits.RA4 == 0){
        LED_SetHigh();
    } else {
        LED_SetLow();
    }
    
}

bool LED_SetInterruptHandler(const LED_Callback Callback_function){
    
    bool handlerSet = false;
    // On v�rifie toujours les param�tres en entr�e d'une fonction publique:
    if(Callback_function != NULL)
    {
        LED_Callback_Function = (LED_Callback)Callback_function;
        handlerSet = true;
    }
    return handlerSet;
    
}

void GPIO_Init(void){
    
    (void)ISR_bRegisterIsrCbk(ISR_ePERIPHERAL_INPUT_GPIO, GPIO_Callback);
    
    //LED configuration
    ANSELAbits.ANSELA4 = 0;
    TRISAbits.TRISA4 = 0;
    
    //Button configuration
    ANSELBbits.ANSELB4 = 0;
    TRISBbits.TRISB4 = 1;
    
    //Interruption configuration
    PIE0bits.IOCIE = 1; 
    
    //Set interrupt on falling and rising edge
    IOCBPbits.IOCBP4 = 1;
    IOCBNbits.IOCBN4 = 1;
    
    IOCBFbits.IOCBF4 = 0;
    
    LED_SetLow();  
    
}

GPIO_state GPIO_GetState(void){
    
    if (LATAbits.LATA4 == 0){
        
        return GPIO_IDLE_state;
        
    } else {
        
        return GPIO_MEASURE_state;
        
    }
    
}

/*--------------------------------------------------------------------------------------------------------------------*/




