/*
 * aes.h - Header file for AES-128 encryption and decryption
 * Provides function declarations and macro configuration for enabling modes (ECB/CBC).
 * Used with bare-metal AES project for side-channel attack experiments.
 */

 #ifndef _AES_H_
 #define _AES_H_
 
 // ======================================================================
 // Configuration Macros
 // ======================================================================
 // Uncomment or define these macros to enable specific AES modes.
 // CBC (Cipher Block Chaining) mode can be used for secure stream encryption.
 // ECB (Electronic Codebook) mode is used for basic block-wise encryption.
 // For the DRDO project, only ECB is used for simplicity in SCA testing.
 
 /*
 #ifndef CBC
     #define CBC 1
 #endif
 */
 
 #ifndef ECB
     #define ECB 1
 #endif
 
 // ======================================================================
 // Public APIs (Only available if ECB mode is enabled)
 // ======================================================================
 
 #if defined(ECB) && ECB
 
 // Expand the input key into round keys used by the AES algorithm
 void Key_Expansion(const uint8_t* key);
 
 // Perform AES-128 encryption in ECB mode
 // The function works in-place on the given 16-byte block
 void AES128_ECB_encrypt(uint8_t *output);
 
 // Perform AES-128 decryption in ECB mode
 // Input: 16-byte encrypted data and original 128-bit key
 // Output: Decrypted plaintext
 void AES128_ECB_decrypt(uint8_t* input, const uint8_t* key, uint8_t *output);
 
 #endif // #if defined(ECB) && ECB
 
 // ======================================================================
 // CBC mode (currently unused in this project, but available if needed)
 // ======================================================================
 
 /*
 #if defined(CBC) && CBC
 
 // AES-128 encryption using CBC mode for multiple blocks
 void AES128_CBC_encrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);
 
 // AES-128 decryption using CBC mode for multiple blocks
 void AES128_CBC_decrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);
 
 #endif // #if defined(CBC) && CBC
 */
 
 #endif //_AES_H_
 