/*
 * main.c - Bare-metal AES-128 infinite encryption loop on Raspberry Pi 4
 * Continuously encrypts a 16-byte block using ECB mode with GPIO toggling
 * Used for collecting EM traces during DRDO internship project
 * Author: Sai Tarun Baratam
 */

 #include <stdio.h>
 #include <string.h>
 #include <stdint.h>
 
 #define ECB 1
 
 #include "aes.h"
 #include "io.h"
 
 typedef unsigned int uint;
 
 // GPIO register addresses (only used if needed for manual GPIO manipulation)
 enum {
     PERIPHERAL_BASE    = 0xFE000000,
     GPFSEL0            = PERIPHERAL_BASE + 0x200000,
     GPSET0             = PERIPHERAL_BASE + 0x20001C,
     GPCLR0             = PERIPHERAL_BASE + 0x200028,
     GPPUPPDN0          = PERIPHERAL_BASE + 0x2000E4
 };
 
 enum {
     GPIO_MAX_PIN       = 53,
     GPIO_FUNCTION_ALT5 = 2,
 };
 
 static void test_encrypt_ecb(void);
 
 int main(void) {   
     // Small warm-up delay loop before beginning encryption
     uint64_t i = 0;
     while(i < 0x9f00000) {
         i++;
     }
 
     test_encrypt_ecb();  // Start the encryption routine
 
     // Control should never return here in infinite loop version
     while(1);
 }
 
 /**
  * @brief Encrypts a fixed 16-byte buffer continuously in an infinite loop
  *        Toggles GPIO pin 2 before and after encryption for EM trace sync
  */
 static void test_encrypt_ecb(void)
 {
     uint8_t key[16] = {
         0x2b, 0x7e, 0x15, 0x16,
         0x28, 0xae, 0xd2, 0xa6,
         0xab, 0xf7, 0x15, 0x88,
         0x09, 0xcf, 0x4f, 0x3c
     };
 
     uint8_t buffer[16] = {
         0x3a, 0xd7, 0x7b, 0xb4,
         0x0d, 0x7a, 0x36, 0x60,
         0xa8, 0x9e, 0xca, 0xf3,
         0x24, 0x66, 0xef, 0x97
     };
 
     // Expand the key once before entering the loop
     Key_Expansion(key);
 
     // Enable GPIO 3 as output (optional setup)
     uint gpfsel0 = mmio_read(GPFSEL0);
     gpfsel0 |= (1 << 6);  // Set GPIO3 to output (FSEL3 = 001)
     mmio_write(GPFSEL0, gpfsel0);
 
     // Delay before starting loop (optional)
     uint64_t i = 0;
     while (i < 0x7e0) {
         i++;
     }
 
     // === Infinite AES Encryption Loop with GPIO Toggle ===
     while (1) {
         mmio_write(GPSET0, 1 << 2);           // Set GPIO2 high (start)
         AES128_ECB_encrypt(buffer);           // Perform AES encryption
         mmio_write(GPCLR0, 1 << 2);           // Set GPIO2 low (end)
 
         i = 0;
         while (i < 0x7e0) {
             i++;                              // Delay between encryptions
         }
     }
 }
 