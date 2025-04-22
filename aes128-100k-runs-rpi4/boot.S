/*
 * boot.S - ARMv8-A (AArch64) startup code for bare-metal Raspberry Pi 4
 * Initializes the system and jumps to the C `main()` function
 * Used in DRDO AES Side-Channel Attack Project
 */

.section ".text.boot"         // Ensure this code is placed at the beginning of the kernel image
.global _start                // Entry point symbol for the linker

_start:
    // ============================================================
    // STEP 1: Core Check - Run only on primary core (core 0)
    // ============================================================
    mrs     x1, mpidr_el1     // Read Multiprocessor Affinity Register
    and     x1, x1, #3        // Extract core ID (lowest 2 bits)
    cbz     x1, 2f            // If core 0, jump to next step

    // Non-core-0 cores hang in a loop (to avoid interference)
1:  wfe                      // Wait For Event (low-power idle)
    b       1b               // Infinite loop for other cores

2:
    // ============================================================
    // STEP 2: Stack Initialization
    // ============================================================
    ldr     x1, =_start       // Load address of _start
    mov     sp, x1            // Set stack pointer below current code

    // ============================================================
    // STEP 3: Zero out the BSS section
    // ============================================================
    ldr     x1, =__bss_start  // Start of .bss (uninitialized data)
    ldr     w2, =__bss_size   // Size of .bss (in words)

3:  cbz     w2, 4f            // If size is zero, skip clearing
    str     xzr, [x1], #8     // Store zero at current address and increment pointer
    sub     w2, w2, #1        // Decrement counter
    cbnz    w2, 3b            // Loop until zero

    // ============================================================
    // STEP 4: Jump to main C function (never returns)
    // ============================================================
4:  bl      main              // Call main()
    b       1b                // Just in case main returns, halt here
