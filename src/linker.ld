ENTRY (_START)
OUTPUTFORMAT(binary)
SECTOINS {
    . = 1M;
    .text: {
        *(.text)
    }

    .rodata: {
        *(.rodata)
    }

    .data: {
        *(.data)
    }

    .bss: {
        *(COMMON)
        *(.bss)
    }
}

