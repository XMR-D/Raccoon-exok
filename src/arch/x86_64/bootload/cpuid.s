[BITS 32]

DetectCpuid:
    pushfd
    pop ax
    mov cx, ax
    xor ax, 1 << 21

    push ax
    popfd

    pushfd
    pop ax

    push cx

    xor ax, cx
    jz NoCPUID
    ret

DetectLongmode:
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz NoLongmode
    ret

NoLongmode:
    hlt ;No long Mode Support

NoCPUID:
    hlt ;CPUID not supported
