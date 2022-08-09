OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
    . = 0x10000;
    .multiboot :
    {
        *(.multiboot)
    }
    .text :
    {
        *(.text)
    }
    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
    }

    end_of_kernel = .;
}
