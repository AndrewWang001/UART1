//#include <xc.h>
#include "config.h"

// Function to initialize a PPS input pin
w_status_t pps_input_init(pps_config_t pin_config) {
    if (pin_config.pps_value == 0) { //check if pps_value is valid
        return W_INVALID_PARAM;
    }

    ASSIGN_PPS(pin_config.port, pin_config.pin, pin_config.pps_value); //assign PPS fxn to pin
    SET_PIN_INPUT(pin_config.port, pin_config.pin); //set pin as input

    return W_SUCCESS;
}

// Function to initialize a PPS output pin
w_status_t pps_output_init(pps_config_t pin_config) {//check if pps_value is valid
    if (pin_config.pps_value == 0) {
        return W_INVALID_PARAM;
    }

    ASSIGN_PPS(pin_config.port, pin_config.pin, pin_config.pps_value);//assign PPS fxn to pin
    SET_PIN_OUTPUT(pin_config.port, pin_config.pin);//set pin as output

    return W_SUCCESS;
}

// UART Initialization Function
w_status_t uart_init(pps_config_t rx_config, pps_config_t tx_config, uint32_t baud, uint32_t osc_freq) {
    // Configure RX pin
    w_status_t rx_status = pps_input_init(rx_config);
    if (rx_status != W_SUCCESS) { //if failed, return error code
        return rx_status;
    }

    // Configure TX pin
    w_status_t tx_status = pps_output_init(tx_config);
    if (tx_status != W_SUCCESS) { //if failed, return error code
        return tx_status;
    }

    // UART configuration
    U1CON0bits.RXEN = 1;    // Enable receiver
    U1CON0bits.TXEN = 1;    // Enable transmitter - idk if needed
    
    U1CON0bits.BRGS = 0; // set up normal speed for baud rate generator
    //U1CON0bits.ABDEN = 0; // disable Auto baud detect
    
    U1CON0bits.MODE = 0b0000; // Asynchronous 8-bit mode
    
    U1CON2bits.RUNOVF = 1; // keep running on overflow, never stop receiving
    //U1CON2bits.FLO = 0; // disable flow control.

    // Baud rate calculation
    uint16_t baud_reg = (uint16_t)((osc_freq / (16 * baud)) - 1);
    U1BRGH = (baud_reg >> 8) & 0xFF; // Set high byte of baud rate
    U1BRGL = baud_reg & 0xFF;        // Set low byte of baud rate

    // Enable UART
    U1CON1bits.ON = 1;
    
    PIE3bits.U1RXIE = 1; // enable interrupts
    U1ERRIEbits.FERIE = 1; // enable framing error interrupt
    U1ERRIEbits.RXFOIE = 1; // enable overflow error interrupt

    return W_SUCCESS;
}

w_status_t uart_send_byte(uint8_t byte) {
    // Wait for the Transmit Buffer to be empty
    while (!PIR3bits.U1TXIF) {} // Use the correct interrupt flag for UART1 Transmit
    U1TXB = byte;     // Write byte to the UART Transmit Buffer
    PIR3bits.U1TXIF = 0;  // Clear transmit flag after writing
    return W_SUCCESS;
}

// Function to receive a byte via UART
uint8_t uart_receive_byte(void) {
    // Wait for data to be available in the Receive Buffer
    while (!PIR3bits.U1RXIF) {} // Use the correct interrupt flag for UART1 Receive
    uint8_t data = U1RXB;  // Read received byte
    PIR3bits.U1RXIF = 0;  // Ensure the flag is cleared
    return data;  // Read the received byte from the UART Receive Buffer
}
