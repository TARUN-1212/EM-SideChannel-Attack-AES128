/*
 * link.ld - Linker script for bare-metal Raspberry Pi 4 (AArch64)
 * Places code and data in specific memory regions for proper boot execution
 * Part of AES128 Side-Channel Attack Project by Sai Tarun Baratam
 */

SECTIONS
{
    /* Start address for kernel in RAM (0x80000 = 512 KB) */
    . = 0x80000;

    /* Place the startup code first (boot.S) */
    .text : {
        KEEP(*(.text.boot))                  /* Ensure boot code is kept */
        *(.text .text.* .gnu.linkonce.t*)    /* All other text/code sections */
    }

    /* Read-only data (e.g., S-boxes) */
    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r*)
    }

    /* Mark the start of writable data section */
    PROVIDE(_data = .);

    /* Initialized data section */
    .data : {
        *(.data .data.* .gnu.linkonce.d*)
    }

    /* Uninitialized data (bss) */
    .bss (NOLOAD) : {
        . = ALIGN(16);         /* Align to 16 bytes for safety */
        __bss_start = .;       /* Start symbol for zeroing in boot.S */
        *(.bss .bss.*)
        *(COMMON)              /* For uninitialized global variables */
        __bss_end = .;
    }

    /* Define end of program image */
    _end = .;

    /* Remove useless sections from final binary */
    /DISCARD/ : {
        *(.comment)
        *(.gnu*)
        *(.note*)
        *(.eh_frame*)
    }
}

/* Export bss size (in 8-byte words) to be used in boot.S for clearing */
__bss_size = (__bss_end - __bss_start) >> 3;
