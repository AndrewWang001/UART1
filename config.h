
#ifndef CONFIG_H
#define	CONFIG_H


#include "common.h"
//#include <stdint.h>

// Structure to define PPS pin configuration
typedef struct {
    uint8_t port;        // Port (0 = A, 1 = B, 2 = C)
    uint8_t pin;         // Pin number (0-7)
    uint8_t pps_value;   // PPS value for this pin
} pps_config_t;


// Default pin configurations (can be overridden in MPLAB settings)
#ifndef RX_PORT
    #define RX_PORT 1  // Default to Port B
#endif

#ifndef RX_PIN
    #define RX_PIN 5   // Default to Pin 5
#endif

#ifndef RX_PPS_VALUE
    #define RX_PPS_VALUE 0x17  // Default PPS value for UART RX
#endif

#ifndef TX_PORT
    #define TX_PORT 1  // Default to Port B
#endif

#ifndef TX_PIN
    #define TX_PIN 7   // Default to Pin 7
#endif

#ifndef TX_PPS_VALUE
    #define TX_PPS_VALUE 0x16  // Default PPS value for UART TX
#endif


//macros

// Macro to concatenate tokens for register naming
// This macro concatenates three tokens together
//#define CONCAT(a, b, c) a##b##c 

// Macro to get the TRIS register for a constant port
//#define GET_TRIS_REG(port) CONCAT(TRIS, port, A)

// Automatically define RX and TX configurations using macros
#define RX_CONFIG ((pps_config_t){RX_PORT, RX_PIN, RX_PPS_VALUE})
#define TX_CONFIG ((pps_config_t){TX_PORT, TX_PIN, TX_PPS_VALUE})


//TRIS register for ports TRIS A B C -> when port is a runtime variable 
#define GET_TRIS_REG(port) ((port) == 0 ? (&TRISA) : ((port) == 1 ? (&TRISB) : (&TRISC)))

// Macro to set a pin as input
#define SET_PIN_INPUT(port, pin) (*GET_TRIS_REG(port) |= (1 << (pin)))
// Macro to set a pin as output
#define SET_PIN_OUTPUT(port, pin) (*GET_TRIS_REG(port) &= ~(1 << (pin)))

// Macro to get the PPS register address based on port and pin (constant)
//#define GET_PPS_REG(port, pin) CONCAT(R, port, pin##PPS) 

//runtime variable for PPS register address
#define GET_PPS_REG(port, pin) \
    ((port) == 0 ? (&RA0PPS + (pin)) : ((port) == 1 ? (&RB0PPS + (pin)) : (&RC0PPS + (pin))))

// Macro to assign the PPS register for a pin
#define ASSIGN_PPS(port, pin, pps_value) (*GET_PPS_REG(port, pin) = (pps_value))

//#define CONFIGURE_UART_PINS()    \
//    SET_PIN_INPUT(RX_PORT, RX_PIN); \
//    SET_PIN_OUTPUT(TX_PORT, TX_PIN); \
//    ASSIGN_PPS(RX_PORT, RX_PIN, RX_PPS_VALUE); \
//    ASSIGN_PPS(TX_PORT, TX_PIN, TX_PPS_VALUE);


// Function prototypes
w_status_t uart_init(pps_config_t rx_config, pps_config_t tx_config, uint32_t baud, uint32_t osc_freq);
w_status_t uart_send_byte(uint8_t byte);
uint8_t uart_receive_byte(void);



#endif	/* CONFIG_H */

