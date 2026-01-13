# UART_COMMANDSHELL

![Language](https://img.shields.io/badge/Language-C-blue) ![Platform](https://img.shields.io/badge/Platform-ESP--IDF-red) ![OS](https://img.shields.io/badge/OS-FreeRTOS-green)

A professional, non-blocking Command Line Interface (CLI) for the ESP32. This project utilizes a **Producer-Consumer architecture** to process serial data efficiently without freezing the CPU, making it suitable for real-time embedded applications.

## 🚀 Project Overview

Standard UART polling blocks the CPU and wastes resources. This project solves that by decoupling hardware interrupts from application logic:
1.  **ISR (Interrupt Service Routine):** Instantly captures incoming bytes.
2.  **Ring Buffer:** Safely stores data in a circular queue (O(1) complexity).
3.  **FreeRTOS Task:** Processes data asynchronously to control hardware.

## 🛠️ Tech Stack & Features

* **Microcontroller:** ESP32 (WROOM)
* **OS:** FreeRTOS (Tasks, Queues, Delays)
* **Data Structure:** Custom Circular (Ring) Buffer
* **Peripherals:** UART0 (Serial), GPIO (LED Control)
* **Performance:** Non-blocking I/O with Zero-Copy mechanisms

## 🔌 Hardware Setup

This project controls an external LED via UART commands.

| Component | Pin Connection |
| :--- | :--- |
| **LED Anode (+)** | **GPIO 2** |
| **LED Cathode (-)** | **GND** (via 220Ω Resistor) |
| **Console** | USB-to-UART (Micro-USB Cable) |

## ⚡ How to Build & Run

1.  **Clone the Repository**
    ```bash
    git clone [https://github.com/Abhinavvv2104/UART_COMMANDSHELL.git](https://github.com/Abhinavvv2104/UART_COMMANDSHELL.git)
    ```

2.  **Build and Flash**
    ```bash
    idf.py build
    idf.py flash
    ```

3.  **Open Monitor**
    ```bash
    idf.py monitor
    ```

4.  **Usage**
    Type the following commands in the serial terminal:
    * `LED_ON`  → Turns the LED ON.
    * `LED_OFF` → Turns the LED OFF.

## 🧠 Software Architecture

```mermaid
graph LR
    PC[User Terminal] -- "LED_ON" --> UART[UART ISR]
    UART -- Event --> Task[Manager Task]
    Task -- Byte --> Buffer[Ring Buffer]
    Buffer -- String --> App


Developed by Abhinav | Powered by ESP-IDF & FreeRTOS