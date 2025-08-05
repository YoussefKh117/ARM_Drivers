/*
 * SysTick.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Youssef Khaled
 */

#include "SysTick.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Handler Function                            *
 *******************************************************************************/
void SysTick_Handler(void)
{
    /* Just call the call back function when interrupt occurs
     * when timer count flag = 1 */
    if (g_callBackPtr != NULL_PTR)
    {
        (*g_callBackPtr)();
    }
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void SysTick_Init(uint16 a_TimeInMilliSeconds)
{
    /* Disable the SysTick Timer by Clear the ENABLE Bit */
    SYSTICK_CTRL_REG = 0;
    SYSTICK_RELOAD_REG = (ms_tick * a_TimeInMilliSeconds - 1); /* Set the Reload value with ms_tick to count 1 ms */
    SYSTICK_CURRENT_REG = 0; /* Clear the Current Register value */
    /* Configure the SysTick Control Register
     * Enable the SysTick Timer (ENABLE = 1)
     * Enable SysTick Interrupt (INTEN = 1)
     * Choose the clock source to be System Clock (CLK_SRC = 1) */
    SYSTICK_CTRL_REG |= 0x07;
}

void SysTick_StartBusyWait(uint16 a_TimeInMilliSeconds)
{
    SYSTICK_CTRL_REG = 0;
    /* Set the Reload value with ms_tick to count 1 ms */
    SYSTICK_RELOAD_REG = (ms_tick * a_TimeInMilliSeconds - 1);
    /* Clear the Current Register value */
    SYSTICK_CURRENT_REG = 0;

    /* Configure the SysTick Control Register
     * Enable the SysTick Timer (ENABLE = 1)
     * Enable SysTick Interrupt (INTEN = 1)
     * Choose the clock source to be System Clock (CLK_SRC = 1) */
    SYSTICK_CTRL_REG |= SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_CLK_SRC;

    /* Wait Until Timer To be 0 */
    while (!(SYSTICK_CTRL_REG & SYSTICk_CTRL_FLAG))
        ;
    SYSTICK_CTRL_REG &= ~SYSTICK_CTRL_ENABLE; /* Disable the SysTick Timer by Clear the ENABLE Bit (Bit0) */
    SYSTICK_CURRENT_REG = 0; /* Clear the Current Register */
}

void SysTick_SetCallBack(volatile void (*Ptr2Func)(void))
{
    /*Save Call Back Function In Global Variable */
    g_callBackPtr = Ptr2Func;
}

void SysTick_Stop(void)
{
    SYSTICK_CTRL_REG &= ~SYSTICK_CTRL_CLK_SRC;
}

void SysTick_Start(void)
{
    SYSTICK_CTRL_REG |= SYSTICK_CTRL_CLK_SRC;
}

void SysTick_DeInit(void)
{
    /* Disable the SysTick Timer by Clear the ENABLE Bit (Bit0) */
    SYSTICK_CTRL_REG = 0;
    /* Clear the Reload Register */
    SYSTICK_RELOAD_REG = 0;
    /* Clear the Current Register */
    SYSTICK_CURRENT_REG = 0;
    /* Restore the global pointer to Null */
    g_callBackPtr = NULL_PTR;
}
