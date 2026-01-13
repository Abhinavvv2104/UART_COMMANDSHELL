#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "uart_manager.h"
#include "ring_buffer.h"

// Note: Change this pin to match your external LED connection
#define LED_GPIO 2 

RingBuffer rx_buffer;
char cmd_string[64];
int cmd_idx = 0;

void process_command(char *cmd) {
    // Debug print to confirm command parsing
    printf("\nCMD RECEIVED: %s\n", cmd);

    if (strcmp(cmd, "LED_ON") == 0) {
        // Logic High (1) for Active High External LED
        gpio_set_level(LED_GPIO, 1); 
        printf(">> SUCCESS: LED Turned ON\n");
    } 
    else if (strcmp(cmd, "LED_OFF") == 0) {
        // Logic Low (0) for Active High External LED
        gpio_set_level(LED_GPIO, 0); 
        printf(">> SUCCESS: LED Turned OFF\n");
    }
    else {
        printf(">> ERROR: Unknown Command. Try 'LED_ON' or 'LED_OFF'\n");
    }
}

void app_main(void) {
    // 1. Hardware Setup
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0); // Start OFF

    // 2. System Setup
    ring_buffer_init(&rx_buffer);
    uart_manager_init(&rx_buffer);
    uart_manager_start();

    printf("\n\n=== UART CLI CONTROLLER READY ===\n");
    printf("commands: LED_ON, LED_OFF\n");

    // 3. Main Loop
    uint8_t byte;
    while (1) {
        if (ring_buffer_read(&rx_buffer, &byte)) {
            putchar(byte); // Echo to terminal

            if (byte == '\n' || byte == '\r') {
                cmd_string[cmd_idx] = '\0'; // Terminate string
                if(cmd_idx > 0) process_command(cmd_string);
                cmd_idx = 0; // Reset
            } else {
                if(cmd_idx < 63) cmd_string[cmd_idx++] = (char)byte;
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Yield to Scheduler
    }
}