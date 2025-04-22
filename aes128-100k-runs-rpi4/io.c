/*
 * io.c - GPIO and UART driver for Raspberry Pi 4 (bare-metal)
 * Provides functions to configure GPIOs and send data over UART1
 * Used to transmit intermediate values, ciphertext, or debug data
 * Part of DRDO AES side-channel project by Sai Tarun Baratam
 */

// ======================================================================
// GPIO Register Definitions (based on BCM2711 datasheet)
// ======================================================================

enum {
    PERIPHERAL_BASE = 0xFE000000,
    GPFSEL0         = PERIPHERAL_BASE + 0x200000,
    GPFSEL1         = PERIPHERAL_BASE + 0x200004,
    GPFSEL2         = PERIPHERAL_BASE + 0x200008,
    GPSET0          = PERIPHERAL_BASE + 0x20001C,
    GPCLR0          = PERIPHERAL_BASE + 0x200028,
    GPPUPPDN0       = PERIPHERAL_BASE + 0x2000E4
};

enum {
    GPIO_MAX_PIN       = 53,
    GPIO_FUNCTION_ALT5 = 2,   // Used for UART1
};

enum {
    Pull_None = 0,
};

// ======================================================================
// MMIO (Memory-Mapped I/O) Access Helpers
// ======================================================================

void mmio_write(long reg, unsigned int val) {
    *(volatile unsigned int *)reg = val;
}

unsigned int mmio_read(long reg) {
    return *(volatile unsigned int *)reg;
}

// ======================================================================
// GPIO Configuration Functions
// ======================================================================

unsigned int gpio_call(unsigned int pin_number, unsigned int value,
                       unsigned int base, unsigned int field_size,
                       unsigned int field_max) {

    unsigned int field_mask = (1 << field_size) - 1;
    if (pin_number > field_max || value > field_mask) return 0;

    unsigned int num_fields = 32 / field_size;
    unsigned int reg = base + ((pin_number / num_fields) * 4);
    unsigned int shift = (pin_number % num_fields) * field_size;

    unsigned int curval = mmio_read(reg);
    curval &= ~(field_mask << shift);         // Clear bits
    curval |= (value << shift);               // Set new value
    mmio_write(reg, curval);

    return 1;
}

unsigned int gpio_set     (unsigned int pin_number, unsigned int value) {
    return gpio_call(pin_number, value, GPSET0, 1, GPIO_MAX_PIN);
}

unsigned int gpio_clear   (unsigned int pin_number, unsigned int value) {
    return gpio_call(pin_number, value, GPCLR0, 1, GPIO_MAX_PIN);
}

unsigned int gpio_pull    (unsigned int pin_number, unsigned int value) {
    return gpio_call(pin_number, value, GPPUPPDN0, 2, GPIO_MAX_PIN);
}

unsigned int gpio_function(unsigned int pin_number, unsigned int value) {
    return gpio_call(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN);
}

// Configure GPIO pin for alternate function ALT5 (used for UART1 TX/RX)
void gpio_useAsAlt5(unsigned int pin_number) {
    gpio_pull(pin_number, Pull_None);
    gpio_function(pin_number, GPIO_FUNCTION_ALT5);
}

// ======================================================================
// UART1 (Mini UART) Configuration and Transmission
// ======================================================================

enum {
    AUX_BASE        = PERIPHERAL_BASE + 0x215000,
    AUX_ENABLES     = AUX_BASE + 4,
    AUX_MU_IO_REG   = AUX_BASE + 64,
    AUX_MU_IER_REG  = AUX_BASE + 68,
    AUX_MU_IIR_REG  = AUX_BASE + 72,
    AUX_MU_LCR_REG  = AUX_BASE + 76,
    AUX_MU_MCR_REG  = AUX_BASE + 80,
    AUX_MU_LSR_REG  = AUX_BASE + 84,
    AUX_MU_CNTL_REG = AUX_BASE + 96,
    AUX_MU_BAUD_REG = AUX_BASE + 104,

    AUX_UART_CLOCK  = 500000000,    // Clock for AUX peripheral
    UART_MAX_QUEUE  = 16 * 1024
};

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK/(baud*8))-1)

// Initialize Mini UART1 for 115200 baud, 8-bit, no parity
void uart_init() {
    mmio_write(AUX_ENABLES, 1);          // Enable UART1
    mmio_write(AUX_MU_IER_REG, 0);       // Disable interrupts
    mmio_write(AUX_MU_CNTL_REG, 0);      // Disable TX/RX
    mmio_write(AUX_MU_LCR_REG, 3);       // 8-bit mode
    mmio_write(AUX_MU_MCR_REG, 0);       // No modem control
    mmio_write(AUX_MU_IIR_REG, 0xC6);    // Disable interrupts
    mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));  // Set baud rate

    gpio_useAsAlt5(14);  // TXD1
    gpio_useAsAlt5(15);  // RXD1

    mmio_write(AUX_MU_CNTL_REG, 3);      // Enable TX/RX
}

// Check if UART is ready to transmit a byte
unsigned int uart_isWriteByteReady() {
    return mmio_read(AUX_MU_LSR_REG) & 0x20;
}

// Send a single byte over UART
void uart_write(unsigned char ch) {
    while (!uart_isWriteByteReady());
    mmio_write(AUX_MU_IO_REG, (unsigned int)ch);
}

// Send a null-terminated string over UART
void uart_writeText(char *buffer) {
    while (*buffer) {
        if (*buffer == '\n') uart_write('\r');  // Add carriage return
        uart_write(*buffer++);
    }
}
