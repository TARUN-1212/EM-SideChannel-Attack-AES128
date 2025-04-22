/*
 * main.c - AES-128 encryption (100,000 times) on Raspberry Pi 4
 * Performs a fixed number of AES encryptions with GPIO toggling
 * Useful for side-channel trace collection with known sample count
 * Author: Sai Tarun Baratam (DRDO Internship Project)
 */

 #include <stdio.h>
 #include <string.h>
 #include <stdint.h>
 
 #define ECB 1
 
 #include "aes.h"
 #include "io.h"
 
 typedef unsigned int uint;
 
 // GPIO base and register offsets (BCM2711 peripheral base)
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
     // Initial startup delay (optional)
     uint64_t i = 0;
     while (i < 0x9f00000) {
         i++;
     }
 
     test_encrypt_ecb();  // Perform AES encryptions in loop
 
     while (1);  // Optional: Halt after completion
 }
 
 /**
  * @brief Performs exactly 100,000 AES-128 ECB encryptions with GPIO timing markers.
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
 
     // Pre-compute round keys once
     Key_Expansion(key);
 
     // Set GPIO3 as output
     uint gpfsel0 = mmio_read(GPFSEL0);
     gpfsel0 |= (1 << 6);  // GPIO3 → output
     mmio_write(GPFSEL0, gpfsel0);
 
     // Initial pre-loop delay
     uint64_t i = 0;
     while (i < 0x7e0) {
         i++;
     }
 
     // === Run AES Encryption 100,000 Times ===
     uint64_t encryptions = 0;
     while (encryptions < 100000) {
         mmio_write(GPSET0, 1 << 2);         // Toggle GPIO2 high (encryption start)
         AES128_ECB_encrypt(buffer);         // Perform encryption
         mmio_write(GPCLR0, 1 << 2);         // Toggle GPIO2 low (encryption end)
 
         // Inter-encryption delay (optional)
         i = 0;
         while (i < 0x7e0) {
             i++;
         }
 
         encryptions++;
     }
 }
 