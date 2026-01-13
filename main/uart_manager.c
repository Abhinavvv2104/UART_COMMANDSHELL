#include "uart_manager.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// --- HARDWARE SETTINGS ---
// We use UART0 (Connected to the USB Serial on ESP32)
#define UART_NUM UART_NUM_0
#define TXD_PIN (GPIO_NUM_1)
#define RXD_PIN (GPIO_NUM_3)

// The "Warehouse" size for the Driver/ISR (internal buffer)
#define RX_BUF_SIZE 1024

// --- GLOBAL VARIABLES ---
// The "Pager" (Queue Handle) that the driver uses to wake us up
static QueueHandle_t uart_queue;

// Pointer to OUR Ring Buffer (The "Shop Shelf")
static RingBuffer *rb_ptr; 

// --- 1. CONFIGURATION (The Setup) ---
void uart_manager_init(RingBuffer *rb) {
    rb_ptr = rb; // Save the Ring Buffer address

    // A. Define the communication rules
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    
    // B. Apply rules to UART0
    uart_param_config(UART_NUM, &uart_config);
    
    // C. Connect the physical pins
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    
    // D. Install the Driver (Hire the ISR Staff & Build the Queue)
    // 20 = Queue size (Mailbox holds 20 notes)
    // &uart_queue = Give us the key to the mailbox
    uart_driver_install(UART_NUM, RX_BUF_SIZE * 2, 0, 20, &uart_queue, 0);
}

// --- 2. THE TASK (The Worker) ---
static void uart_event_task(void *pvParameters) {
    uart_event_t event;
    uint8_t temp_buffer[128]; // Temporary bucket for the forklift
    
    while(1) {
        // SLEEP HERE until the Queue Pager beeps.
        // portMAX_DELAY = Wait forever, do not burn CPU.
        if(xQueueReceive(uart_queue, (void * )&event, (TickType_t)portMAX_DELAY)) {
            
            // We woke up! Check the notification type.
            if(event.type == UART_DATA) {
                // The ISR says: "I have data for you."
                
                // 1. Fetch data from the Driver's Warehouse
                int len = uart_read_bytes(UART_NUM, temp_buffer, event.size, portMAX_DELAY);
                
                // 2. Stock it onto our Ring Buffer Shelf
                for(int i=0; i<len; i++) {
                    ring_buffer_write(rb_ptr, temp_buffer[i]);
                }
            }
            // (We ignore other events like BUFFER_FULL for now)
        }
    }
}

// --- 3. START ENGINE ---
void uart_manager_start(void) {
    // Create the FreeRTOS task that runs the function above
    xTaskCreate(uart_event_task, "uart_event_task", 4096, NULL, 12, NULL);
}