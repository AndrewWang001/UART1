#include "config.h"
#include <xc.h>

#include "config.h"

void main(void) {
    // Get TRIS register address and print to UART
    volatile uint8_t *tris_addr = GET_TRIS_REG(RX_PORT);
    uart_send_byte((uint8_t)((uint16_t)tris_addr & 0xFF));  // Print LSB of address
    uart_send_byte((uint8_t)(((uint16_t)tris_addr >> 8) & 0xFF));  // Print MSB of address
    uart_send_byte('H');
    uart_send_byte('i');

}
