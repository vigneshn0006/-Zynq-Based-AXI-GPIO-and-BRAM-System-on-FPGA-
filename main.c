/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "sleep.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "xstatus.h"
#include "xbram.h"

#define GPIO_BASEADDR   XPAR_AXI_GPIO_0_BASEADDR
#ifdef XPAR_AXI_BRAM_CTRL_0_BASEADDR
  #define BRAM_BASE_ADDR XPAR_AXI_BRAM_CTRL_0_BASEADDR
#else
  #define BRAM_BASE_ADDR XPAR_XBRAM_0_BASEADDR
#endif

#define LED_CHANNEL 1
#define SWITCH_CHANNEL 2
#define LED_MASK 0x0F
#define SWITCH_MASK 0x0F
int main()
{
    // XGpio Gpio;
    // int Status;
    // u32 switch_status=0;
    // init_platform();
   
	// Status = XGpio_Initialize(&Gpio, GPIO_BASEADDR);
	// if (Status != XST_SUCCESS) {
	// 	xil_printf("Gpio Initialization Failed\r\n");
	// 	return XST_FAILURE;
	// }
   
    // print("Hello World\n\r");
    // print("Successfully ran Hello World application");
    // XGpio_SetDataDirection(&Gpio, LED_CHANNEL, ~LED_MASK);
    // XGpio_SetDataDirection(&Gpio, SWITCH_CHANNEL, SWITCH_MASK);
    // switch_status=XGpio_DiscreteRead(&Gpio, SWITCH_CHANNEL);
    // XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, switch_status);
    // xil_printf("Switch State: 0x%02X \n", switch_status);
   XBram Bram;
    XBram_Config *ConfigPtr;
    int Status;

    const char write_str[] = "Vignesh N";
    char read_buffer[32] = {0};
    u32 len = strlen(write_str);

    init_platform();

    xil_printf("\r\n==========================================\r\n");
    xil_printf("     XBRAM String Write/Read Demonstration \r\n");
    xil_printf("==========================================\r\n");

    // 1. Lookup Configuration matching xbram.h prototype
#ifndef SDT
    ConfigPtr = XBram_LookupConfig(XPAR_AXI_BRAM_CTRL_0_DEVICE_ID);
#else
    ConfigPtr = XBram_LookupConfig(BRAM_BASE_ADDR);
#endif

    if (ConfigPtr == NULL) {
        xil_printf("XBram_LookupConfig failed!\r\n");
        cleanup_platform();
        return XST_FAILURE;
    }

    // 2. Initialize XBram instance using XBram_CfgInitialize
    Status = XBram_CfgInitialize(&Bram, ConfigPtr, ConfigPtr->CtrlBaseAddress);
    if (Status != XST_SUCCESS) {
        xil_printf("XBram_CfgInitialize failed!\r\n");
        cleanup_platform();
        return XST_FAILURE;
    }

    xil_printf("Target BRAM Base Address: 0x%08X\r\n", (UINTPTR)BRAM_BASE_ADDR);
    xil_printf("Writing string: \"%s\"...\r\n", write_str);

    // 3. Byte-level memory write into BRAM address space
    for (u32 i = 0; i <= len; i++) {
        XBram_Out8(BRAM_BASE_ADDR + i, (u8)write_str[i]);
    }

    xil_printf("Write finished. Reading back from BRAM...\r\n");

    // 4. Byte-level memory read back from BRAM address space
    for (u32 i = 0; i < len; i++) {
        read_buffer[i] = (char)XBram_In8(BRAM_BASE_ADDR + i);
    }
    read_buffer[len] = '\0'; // Ensure string is null-terminated

    xil_printf("\r\nResult string read from BRAM: \"%s\"\r\n", read_buffer);

    // 5. Memory Offset Map (Hex & ASCII breakdown)
    xil_printf("\r\n--- BRAM Memory Dump ---\r\n");
    for (u32 i = 0; i < len; i++) {
        xil_printf("Address 0x%08X : 0x%02X ('%c')\r\n", 
                   (UINTPTR)(BRAM_BASE_ADDR + i), 
                   (u8)read_buffer[i], 
                   read_buffer[i]);
    }
    
    cleanup_platform();
    return 0;
}
