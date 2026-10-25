# Zynq-7000 SoC Design with AXI GPIO and BRAM (Vivado + Vitis)

A Zynq-7000 SoC system built in Vivado IP Integrator, with an embedded C application in Vitis that controls Programmable Logic (PL) peripherals from the Processing System (PS).

## Overview
This project connects the ZYNQ7 Processing System to PL peripherals over an AXI interconnect. The ARM processor uses memory-mapped AXI GPIO to drive 8 LEDs and read 8 switches, and accesses on-chip Block RAM through an AXI BRAM Controller. The bitstream was generated in Vivado and the application software was developed in Vitis.

## Features
- ZYNQ7 Processing System configured with the M_AXI_GP0 master port
- AXI GPIO with two channels: 8-bit LED output and 8-bit switch input
- AXI BRAM Controller with Block Memory Generator for on-chip memory
- AXI Interconnect for PS-to-PL communication
- Processor System Reset for synchronized resets
- Embedded C application in Vitis to control PL peripherals

## Block Design

![Block Design](images/block_design.png)

## Architecture

| IP Block | Description |
|---|---|
| `processing_system7_0` | ZYNQ7 Processing System (ARM Cortex-A9), provides the AXI master, clock (`FCLK_CLK0`) and reset |
| `axi_interconnect_0` | Routes AXI transactions from the PS to the PL peripherals |
| `axi_gpio_0` | Dual-channel GPIO: `leds_8bits` (output) and `sws_8bits` (input) |
| `axi_bram_ctrl_0` | AXI slave interface to the block RAM |
| `blk_mem_gen_0` | Block Memory Generator providing on-chip memory |
| `proc_sys_reset_0` | Generates synchronized reset signals for the interconnect and peripherals |

## Address Map
| Peripheral | Base Address | Range |
|---|---|---|
| `axi_gpio_0` | _<fill from Address Editor>_ | _<e.g., 64K>_ |
| `axi_bram_ctrl_0` | _<fill from Address Editor>_ | _<e.g., 8K>_ |

## Software (Vitis)
The embedded C application runs on the ARM processor and:
- Writes values to the LED channel of the AXI GPIO
- Reads the switch inputs from the second GPIO channel
- _<Add: reads/writes BRAM, prints over UART, etc., if your code does this>_

Example snippet:

```c
#include "xparameters.h"
#include "xgpio.h"

XGpio Gpio;

int main() {
    XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_0_BASEADDR);
    XGpio_SetDataDirection(&Gpio, 1, 0x00);  // Channel 1: LEDs (output)
    XGpio_SetDataDirection(&Gpio, 2, 0xFF);  // Channel 2: switches (input)

    while (1) {
        u32 sw = XGpio_DiscreteRead(&Gpio, 2);
        XGpio_DiscreteWrite(&Gpio, 1, sw);   // Mirror switches to LEDs
    }
    return 0;
}
```

## Tools Used
- Xilinx Vivado 2025.2 (IP Integrator, synthesis, implementation, bitstream)
- Xilinx Vitis (embedded C development)
- FPGA board: _<add your board here>_

## How to Run
1. Open Vivado and recreate the project using the provided Tcl script, or open the project directly.
2. Generate the bitstream and export the hardware platform (`.xsa`).
3. Create a platform and application project in Vitis using the `.xsa`.
4. Build the application and program the FPGA.
5. Run the application on the ARM processor and observe the LEDs responding to the switches.

## Project Structure
├── hardware/ # .xsa file, block design Tcl script, XDC constraints
├── software/ # Vitis source files (main.c, etc.)
├── images/ # Block design screenshot
└── README.md

## Future Improvements
- Add interrupt-driven GPIO handling
- Add custom AXI4-Lite IP for user-defined logic
- Add UART-based user interface for runtime control
- Use DMA for high-speed BRAM transfers
