## 📝 Project Overview
Complete implementation of SysTick timer and NVIC (Nested Vectored Interrupt Controller) drivers for Texas Instruments' TM4C123GH6PM microcontroller (ARM Cortex-M4F core), developed as the final project for Advanced Embedded Diploma.

## 🚀 Features

### SysTick Driver
| Feature                          | Function                      | Description |
|----------------------------------|-------------------------------|-------------|
| Initialization                   | `SysTick_Init()`             | Initialize with millisecond precision |
| Busy-wait Timing                 | `SysTick_StartBusyWait()`    | Polling-based delay |
| Interrupt Handling               | `SysTick_Handler()`          | ISR with callback support |
| Callback Management              | `SysTick_SetCallback()`      | Set user-defined handler |
| Timer Control                    | `SysTick_Start()`/`Stop()`   | Runtime control |
| Deinitialization                 | `SysTick_Deinit()`           | Clean shutdown |

### NVIC Driver
| Category         | Functions                      | Description |
|------------------|--------------------------------|-------------|
| IRQ Management   | `NVIC_EnableIRQ()`/`DisableIRQ()` | Control peripheral interrupts |
| IRQ Priority     | `NVIC_SetPriorityIRQ()`       | Configure interrupt priorities |
| Exception Control| `NVIC_EnableException()`      | System/fault exception handling |
| Exception Priority| `NVIC_SetPriorityException()` | Configure exception priorities |