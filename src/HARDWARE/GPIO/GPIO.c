/*
 * File:   GPIO.c
 * Author: thiba
 *
 * Created on 25 octobre 2024, 15:49
 */


#include <xc.h>
#include "GPIO.h"

//void Init_GPIO(uint16_t *gpio_address,signal signal_mode,IO io_mode){
//    
//    *gpio_address = 1;
//    TRISBbits.TRISB3 = 1;
//    
//}

void GPIO_Init(void){
    
    ANSELAbits.ANSELA4 = 0;
    TRISAbits.TRISA4 = 0;
    
}

uint8_t GPIO_SetHigh(void){
    
    LATAbits.LATA4 = 1;
    if (PORTAbits.RA4 != 1){
        return 1;
    }
    return 0;
};

uint8_t GPIO_SetLow(void){
    
    LATAbits.LATA4 = 0;
    if (PORTAbits.RA4 != 0){
        return 1;
    }
    
    return 0;
    
};
