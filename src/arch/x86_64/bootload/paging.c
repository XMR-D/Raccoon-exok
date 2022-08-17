#include "system.h"

uint32_t * PML4T = (void *) 0x1000;
uint32_t * PDPT = (void *) 0x2000;
uint32_t * PDT = (void *) 0x3000;
uint32_t * PT = (void *) 0x4000;

void Init_Paging(void)
{
    PML4T[0] = (uint32_t) &PDPT;
    PDPT[0] = (uint32_t) &PDT;
    PDT[0] = (uint32_t) &PT;
    uint32_t adress = 0x0;

    for(uint32_t i = 0; i <= 511; i += 1)
    {
        PT[i] = 0x0 + adress;
        adress += 0x1000;
    }

}
