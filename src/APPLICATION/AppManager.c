/*
 * File:   app.c
 * Author: thiba
 *
 * Created on October 18, 2024, 4:52 PM
 */


#include <xc.h>
#include "GPIO.h"

void LED_Callback_Function(void){
    
    LED_Toggle();
}

void App_Init(void) {
    
    (void)LED_SetInterruptHandler(LED_Callback_Function);
    GPIO_Init();
    
}


