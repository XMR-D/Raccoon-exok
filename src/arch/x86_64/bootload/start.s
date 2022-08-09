[BITS 32]

[section .multiboot]
align 4

dd 0x1badb002
dd 0x2
dd -(0x1badb002 + 0x2)


[section .text]
align 4

%macro IRQ_NOCODE 1
[global irq%1]
extern change_args
irq%1:
    push 0
    push %1
    call change_args
    jmp irq_handler
%endmacro

%macro ISR_NOCODE 1
[global isr%1]
extern change_args
isr%1:
    push 0
    push %1
    call change_args
    jmp isr_handler
%endmacro

%macro ISR_CODE 1
[global isr%1]
extern change_args
isr%1:
    push %1
    call change_args
    jmp isr_handler
%endmacro

global start
start:
    mov esp, _sys_stack
    jmp stublet


; endless loop where we call main and other functions.
stublet:
    push bx
    xchg bx, bx
    extern convert
    call convert
    extern lmain
    call lmain
    ;TODO : jmp to the main exok-kernel with the return value of lmain
    jmp $


global gdt_flush
extern gp
gdt_flush:
    lgdt [gp]
    mov ax, 0x10       ; 0x10 offset in GDT to kernel data segment (DS)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2    ; 0x08 offset in GDT to kernel code segment (CS)
flush2:
    ret

global idt_load
extern idtpointer
idt_load:
    lidt [idtpointer]
    ret

ISR_NOCODE 0
ISR_NOCODE 1
ISR_NOCODE 2
ISR_NOCODE 3
ISR_NOCODE 4
ISR_NOCODE 5
ISR_NOCODE 6
ISR_NOCODE 7
ISR_CODE 8
ISR_NOCODE 9
ISR_CODE 10
ISR_CODE 11
ISR_CODE 12
ISR_CODE 13
ISR_CODE 14
ISR_NOCODE 15
ISR_NOCODE 16
ISR_CODE 17
ISR_NOCODE 18
ISR_NOCODE 19

IRQ_NOCODE 32
IRQ_NOCODE 33
IRQ_NOCODE 34
IRQ_NOCODE 35
IRQ_NOCODE 36
IRQ_NOCODE 37
IRQ_NOCODE 38
IRQ_NOCODE 39
IRQ_NOCODE 40
IRQ_NOCODE 41
IRQ_NOCODE 42
IRQ_NOCODE 43
IRQ_NOCODE 44
IRQ_NOCODE 45
IRQ_NOCODE 46
IRQ_NOCODE 47

global isr_handler
extern Basic_isr_handling
isr_handler:
    pusha
    push gs
    push fs
    push ds
    push es

    mov ax, 0x10       ; 0x10 offset in GDT to kernel data segment (DS)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; call the C handling function
    call Basic_isr_handling

    ; Restore context
    pop es
    pop ds
    pop fs
    pop gs
    popa
    add esp, 8
    iret

global irq_handler
extern Basic_irq_handling
irq_handler:
    pusha
    push gs
    push fs
    push ds
    push es

    mov ax, 0x10       ; 0x10 offset in GDT to kernel data segment (DS)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    call Basic_irq_handling

    pop es
    pop ds
    pop fs
    pop gs
    popa
    add esp, 8
    iret

; allocate 8kB for .bss section used to stored the stack
[section .bss]
align 4

resb 8192
_sys_stack
