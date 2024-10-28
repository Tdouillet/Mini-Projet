/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      main.c
 *
 * @author    Jean DEBAINS
 * @date      14/06/2023
 *
 * @version   0.0.0
 *
 * @brief     Main program entry point
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
#include <stdio.h>
#include "Common.h"


/*********************************/
/* HARDWARE INCLUDES             */
/*********************************/
#include "ISR.h"
#include "CLOCK.h"
#include "ADC.h"
#include "TIMER.h"
#include "EUSART.h"
#include "I2CM.h"
#include "GPIO.h"

// Add the required includes for the hardware modules here...

/*********************************/
/* DRIVER INCLUDES               */
/*********************************/
#include "LCD.h"

// Add the required includes for the driver modules here...

/*********************************/
/* APPLICATION INCLUDES          */
/*********************************/
// Add the required includes for the application modules here...


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/

#define ADC_TIMEOUT 1000

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                                                                       */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* START FUNCTION                                                                                                     */
/**********************************************************************************************************************/
void main(void)
{
  // WDT operating mode ? WDT Disabled:
  #pragma config WDTE = OFF

  // Low-voltage programming enabled:
  #pragma config LVP = ON

  //Variable declaration
  uint16_t adc_value;
  uint16_t * adc_value_ptr = &adc_value;
  ADC_tenuStatus estatus;
    
  // Enable the Global Interrupts
  ISR_GlobalInterruptEnable();

  // Enable the Peripheral Interrupts:
  ISR_PeripheralInterruptEnable();

  // Begin of the Init:
  CMN_vidInitialize();

  /*********************************/
  /* HARDWARE INITIALIZATIONS      */
  /*********************************/
  CLOCK_vidInitialize();
  EUSART_vidInitialize();
  ADC_vidInitialize();
  TIM0_vidInitialize();
  I2CM_vidInitalize();
  GPIO_Init();

  // Add your initialization function here for the hardware modules...

  /*********************************/
  /* DRIVER INITIALIZATIONS        */
  /*********************************/
  LCD_vidInitialize();

  // Add your initialization function here for the driver modules...

  /*********************************/
  /* APPLICATION INITIALIZATIONS   */
  /*********************************/
  // Add your initialization function here for the application modules...

  // End of the Init:
  CMN_vidEndInit();

  // Main program loop:
  while(true)
  {
    
      estatus = ADC_enuGetRawValue(adc_value_ptr,ADC_TIMEOUT);
      CMN_systemPrintf("%d",adc_value);
      Nop();
      
      
  }

  // We should never reach this code part:
  CMN_abortAll();
}


/*--------------------------------------------------------------------------------------------------------------------*/
