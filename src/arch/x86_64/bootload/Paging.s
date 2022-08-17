Disable_Paging:
    mov eax, cr0
    and eax, 01111111111111111111111111111111b
    mov cr0, eax


Set_Paging:
    ;enable the physical address extension paging (PAE)
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ;move the physical address of pml4 into cr3
    mov edi, 0x1000
    mov cr3, edi
    

    ;set the long mode bit in the efer register
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ;enable paging
    mov eax, cr0
    or eax, 1 << 31 | 1 << 0
    mov cr0, eax
