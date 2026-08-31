/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSOC Control C1 MCU: UART Transmit Example
*              for ModusToolbox. This code example demonstrates simple UART
*              communication by printing a "Hello World" message on a terminal
*              and blinking an LED. 
*
* Related Document: See README.md
*
*******************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/

#include "cybsp.h"
#include "cy_utils.h"
#include "cy_gpio.h"
#include "cy_uart.h"

/*******************************************************************************
* Defines
*******************************************************************************/

/* Declarations for LED toggle and UART transmission timing */
#define TICKS_PER_SECOND        1000
#define TICKS_WAIT              1000

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of array to store the message to be transmitted */
const uint8_t message[] = "Hello world!!\r\n";
const uint8_t clear_screen[] = "\x1b[2J\x1b[;H";

/*******************************************************************************
* Function Name: SysTick_Handler
********************************************************************************
* Summary:
* Function called by system timer every millisecond. It toggles an LED and sends
* the message via UART.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void SysTick_Handler(void)
{
    static uint32_t ticks = 0;
    uint32_t index;

    ticks++;
    if (ticks == TICKS_WAIT)
    { 
        /* Toggle the LED */
        Cy_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);

        for (index = 0; index < sizeof(message) - 1; index++)
        {
            /* Send a message via UART periodically */
            Cy_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, message[index]);
        }
        ticks = 0;
    }
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It performs the following tasks:
*  1. Initial setup of device.
*  2. Starts the UART peripheral
*  3. Enables the system timer
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{    
    cy_rslt_t result;
    uint32_t index;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Start the UART peripheral*/ 
    Cy_UART_CH_Start(CYBSP_DEBUG_UART_HW);

    /* Send a message to clear the screen */
    for (index = 0; index < sizeof(clear_screen) - 1; index++)
    {
        Cy_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, clear_screen[index]);
    }

    /* Enable system timer */
    SysTick_Config(SystemCoreClock / TICKS_PER_SECOND);

    while(1)
    {
        /* Infinite loop */
    }
}

/* [] END OF FILE */
