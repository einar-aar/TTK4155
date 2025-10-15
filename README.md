# Distributed Embedded Control System – Ping Pong Game

## Project Overview

This project implements a **distributed, computer-controlled electromechanical ping pong game**. 

The system is divided into **two embedded nodes** that communicate via a **CAN bus**:
- **Node 1**: User interface, input, display, and basic control.
- **Node 2**: Motor control, servo control, IR sensing, and game mechanics.

The objective is to assemble the hardware and develop software for the microcontrollers to create a **fully functional embedded control system**.

## System Architecture

### Node 1 Components
- Breadboard with AVR microcontroller (ATmega162)
- User I/O board with OLED display, joystick, buttons, and touch inputs
- SPI communication interface
- RS-232 for serial communication

### Node 2 Components
- Arduino Due (ATSAM3X8E, ARM Cortex-M3)
- Custom shield for motor and servo control
- CAN bus controller
- IR sensors and solenoid

## Technologies and Tools

- **Languages**: C  
- **Microcontrollers**: ATmega162 (Node 1), ATSAM3X8E (Node 2)  
- **Communication**: RS-232, SPI, CAN bus  
- **Development environment**:
  - AVR-GCC & OpenOCD (Linux)
- **Debugger**: Atmel-ICE (JTAG/ISP)
- **Terminal software** for serial communication
- **Version control**: Git, Github

## Features

- Real-time joystick input and feedback via OLED display  
- Distributed architecture using CAN bus  
- Modular C drivers for hardware abstraction  
- Servo-controlled paddle and IR ball detection  
- Motorized mechanisms for ball return  
- Optional multiplayer mode with two full systems

