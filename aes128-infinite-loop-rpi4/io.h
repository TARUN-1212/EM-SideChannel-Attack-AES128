/*
 * io.h - Header file for GPIO and UART interface on Raspberry Pi 4 (bare-metal)
 * Provides function declarations for UART initialization, text output,
 * and memory-mapped I/O helpers
 * Part of AES128 Side-Channel Attack Project
 */

 #ifndef _IO_H_
 #define _IO_H_
 
 // Initialize UART1 (Mini UART) with 115200 baud, 8-bit, no parity
 void uart_init(void);
 
 // Transmit a null-terminated string over UART
 void uart_writeText(char *buffer);
 
 // Write a single byte to UART (blocking until ready)
 void uart_write(unsigned char ch);
 
 // Memory-mapped I/O functions to write/read directly to/from registers
 void mmio_write(long reg, unsigned int val);
 unsigned int mmio_read(long reg);
 
 // (Optional - remove or implement these if unused in your project)
 void uart_loadOutputFifo(void);        // Not defined in io.c
 unsigned char uart_readByte(void);     // Not defined in io.c
 unsigned int uart_isReadByteReady(void); // Not defined in io.c
 void uart_writeByteBlocking(unsigned char ch); // Duplicate of uart_write?
 void uart_update(void);                // Not defined in io.c
 void uart_hex(unsigned int d);        // Not defined in io.c
 
 #endif // _IO_H_
 