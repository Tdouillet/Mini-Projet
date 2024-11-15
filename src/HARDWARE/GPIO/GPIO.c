/*
 * File:   GPIO.c
 * Author: thiba
 *
 * Created on 25 octobre 2024, 15:49
 */


#include <xc.h>
#include "GPIO.h"
#include "ISR.h"

//void Init_GPIO(uint16_t *gpio_address,signal signal_mode,IO io_mode){
//    
//    *gpio_address = 1;
//    TRISBbits.TRISB3 = 1;
//    
//}

static LED_Callback LED_Callback_Function = NULL;

void GPIO_ClearIntFlags(void){
    
    IOCBFbits.IOCBF4 = 0;    
    
}

static bool GPIO_Callback(void){
    
    Nop();
    if (PIR0bits.IOCIF == 1){
        if (LED_Callback_Function != NULL){
            LED_Callback_Function();
            GPIO_ClearIntFlags();
        }   
    }
    
    return true;
}

void LED_SetLow(void){
    
    LATAbits.LATA4 = 0; 
};

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
    // On vérifie toujours les paramètres en entrée d'une fonction publique:
    if(Callback_function != NULL)
    {
        LED_Callback_Function = (LED_Callback)Callback_function;
        handlerSet = true;
    }
    return handlerSet;
    
}


