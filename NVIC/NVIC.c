/*
 * NVIC.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Youssef Khaled
 */

#include "NVIC.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/*                              Pointers To Base                              */
#define NVIC_PRI_BASE_REG                   ((volatile uint32 *) 0xE000E400)
#define NVIC_EN_BASE_REG                    ((volatile uint32 *) 0xE000E100)
#define NVIC_DIS_BASE_REG                   ((volatile uint32 *) 0xE000E180)

/*********************************************************************
 * Service Name: NVIC_EnableIRQ
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): IRQ_Num - Number of the IRQ from the target vector table
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to enable Interrupt request for this specific IRQ.
 **********************************************************************/

void NVIC_EnableIRQ(NVIC_IRQType IRQ_Num)
{
    /* Each Register is 32 bit and the IRQ numbers are continuous like 33 , 34 and so on */
    uint8 Reg_Num = IRQ_Num / 32;
    uint8 Bit_Offset = IRQ_Num % 32;
    /* Access The current Register by use the base address + offset {0,1,2,3,4} so if Reg_Num = 0 -> Address = 0xE000E400
     * else if = 1 -> Address = 0xE000E404 incremented by (4 bytes because the pointer to this address is uint32) */
    NVIC_EN_BASE_REG[Reg_Num] |= (1 << Bit_Offset);
}

/*********************************************************************
 * Service Name: NVIC_DisableIRQ
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): IRQ_Num - Number of the IRQ from the target vector table
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to disable Interrupt request for this specific IRQ.
 **********************************************************************/

void NVIC_DisableIRQ(NVIC_IRQType IRQ_Num)
{
    /* Each Register is 32 bit and the IRQ numbers are continuous like 33 , 34 and so on */
    uint8 Reg_Num = IRQ_Num / 32;
    uint8 Bit_Offset = IRQ_Num % 32;
    /* Access The current Register by use the base address + offset {0,1,2,3,4} so if Reg_Num = 0 -> Address = 0xE000E100
     * else if = 1 -> Address = 0xE000E104 incremented by (4 bytes because the pointer to this address is uint32) */
    NVIC_DIS_BASE_REG[Reg_Num] |= (1 << Bit_Offset);
}

/*********************************************************************
 * Service Name: NVIC_SetPriorityIRQ
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in):
 *     IRQ_Num - Number of the IRQ from the target vector table
 *     IRQ_Priority - Priority value to be set for the specified IRQ
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to set the priority value for a specific IRQ.
 **********************************************************************/

void NVIC_SetPriorityIRQ(NVIC_IRQType IRQ_Num,
                         NVIC_IRQPriorityType IRQ_Priority)
{
    /* Priority Muse be only 3-bits */
    IRQ_Priority &= 0x07;
    uint8 Reg_Num = IRQ_Num / 4;
    /* Only use 3 bits and the registers contain only 4 Peripherals and bits offset is 5 , 13 , 21 , 29 */
    uint8 Bit_Offset = ((IRQ_Num % 4) * 8) + 5;
    /* Make sure there is no previous Priority */
    NVIC_PRI_BASE_REG[Reg_Num] &= ~(0x07 << Bit_Offset);
    /* Set the new priority */
    NVIC_PRI_BASE_REG[Reg_Num] |= (IRQ_Priority << Bit_Offset);
}

/*********************************************************************
 * Service Name: NVIC_EnableException
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): Exception_Num - Number of the exception to enable
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to enable specific ARM system or fault exceptions.
 **********************************************************************/

void NVIC_EnableException(NVIC_ExceptionType Exception_Num)
{
    switch (Exception_Num)
    {
    case EXCEPTION_MEM_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL |= MEM_FAULT_ENABLE_MASK;
        break;
    case EXCEPTION_BUS_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL |= BUS_FAULT_ENABLE_MASK;
        break;
    case EXCEPTION_USAGE_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL |= USAGE_FAULT_ENABLE_MASK;
        break;
    default:
        Enable_Faults();
    }

}

/*********************************************************************
 * Service Name: NVIC_DisableException
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): Exception_Num - Number of the exception to disable
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to disable specific ARM system or fault exceptions.
 **********************************************************************/

void NVIC_DisableException(NVIC_ExceptionType Exception_Num)
{
    switch (Exception_Num)
    {
    case EXCEPTION_MEM_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL &= ~MEM_FAULT_ENABLE_MASK;
        break;
    case EXCEPTION_BUS_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL &= ~BUS_FAULT_ENABLE_MASK;
        break;
    case EXCEPTION_USAGE_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL &= ~USAGE_FAULT_ENABLE_MASK;
        break;
    default:
        Disable_Faults();
    }
}

/*********************************************************************
 * Service Name: NVIC_SetPriorityException
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in):
 *     Exception_Num - Number of the exception to set priority
 *     Exception_Priority - Priority value to be set for the specified exception
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to set the priority value for specific ARM system or fault
 *              exceptions.
 **********************************************************************/

void NVIC_SetPriorityException(NVIC_ExceptionType Exception_Num,
                               NVIC_ExceptionPriorityType Exception_Priority)
{
    switch (Exception_Num)
    {
    case EXCEPTION_MEM_FAULT_TYPE:
        NVIC_SYSTEM_PRI1_REG = (NVIC_SYSTEM_PRI1_REG
                & ~(MEM_FAULT_PRIORITY_MASK))
                | (Exception_Priority << MEM_FAULT_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_BUS_FAULT_TYPE:
        NVIC_SYSTEM_PRI1_REG = (NVIC_SYSTEM_PRI1_REG
                & ~(BUS_FAULT_PRIORITY_MASK))
                | (Exception_Priority << BUS_FAULT_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_USAGE_FAULT_TYPE:
        NVIC_SYSTEM_PRI1_REG = (NVIC_SYSTEM_PRI1_REG
                & ~(USAGE_FAULT_PRIORITY_MASK))
                | (Exception_Priority << USAGE_FAULT_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_SVC_TYPE:
        NVIC_SYSTEM_PRI2_REG = (NVIC_SYSTEM_PRI2_REG & ~(SVC_PRIORITY_MASK))
                | (Exception_Priority << SVC_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_PEND_SV_TYPE:
        NVIC_SYSTEM_PRI3_REG = (NVIC_SYSTEM_PRI3_REG & ~(PENDSV_PRIORITY_MASK))
                | (Exception_Priority << PENDSV_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_DEBUG_MONITOR_TYPE:
        NVIC_SYSTEM_PRI3_REG = (NVIC_SYSTEM_PRI3_REG
                & ~(DEBUG_MONITOR_PRIORITY_MASK))
                | (Exception_Priority << DEBUG_MONITOR_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_SYSTICK_TYPE:
        NVIC_SYSTEM_PRI3_REG = (NVIC_SYSTEM_PRI3_REG & ~(SYSTICK_PRIORITY_MASK))
                | (Exception_Priority << SYSTICK_PRIORITY_BITS_POS);
    }
}
