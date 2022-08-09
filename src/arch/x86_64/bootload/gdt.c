#include "system.h"

/* Defines a GDT entry.*/

struct gdt_entry
{
    uint16_t llimit : 16;
    uint16_t lbase : 16;
    uint8_t mbase : 8;

    //access byte
    uint8_t A : 1;
    uint8_t RW : 1;
    uint8_t DC : 1;
    uint8_t E : 1;
    uint8_t S : 1;
    uint8_t DPL : 2;
    uint8_t P : 1;

    uint8_t hlimit : 4;

    //flag bits
    uint8_t Reserved : 1;
    uint8_t L : 1;
    uint8_t DB : 1;
    uint8_t G : 1;

    uint8_t hbase : 8;
}__attribute__((packed));


/* Special pointer which includes the limit:*/
struct gdt_ptr
{
    uint16_t limit : 16;
    uint32_t base : 32;
}__attribute__((packed));

/* Our GDT, with 5 entries, and finally our special GDT pointer */
struct gdt_entry gdt[6];
struct gdt_ptr gp;

/* function to reload the GDT our Gdt*/
extern void gdt_flush();

void gdt_set_gate(int num, uint16_t lowlimit, uint8_t highlimit, uint16_t lowbase, uint8_t middlebase, uint16_t highbase, uint8_t a, uint8_t rw, uint8_t dc, uint8_t e, uint8_t s, uint8_t dpl, uint8_t p, uint8_t g, uint8_t db, uint8_t l)
{
    gdt[num].llimit = lowlimit;
    gdt[num].lbase = lowbase;
    gdt[num].mbase = middlebase;

    //access gate bits setting
    gdt[num].A = a;
    gdt[num].RW = rw;
    gdt[num].DC = dc;
    gdt[num].E = e;
    gdt[num].S = s;
    gdt[num].DPL = dpl;
    gdt[num].P = p;

    gdt[num].hlimit = highlimit;

    //flag byte setting
    gdt[num].Reserved = 0;
    gdt[num].L = l;
    gdt[num].DB = db;
    gdt[num].G = g;

    gdt[num].hbase = highbase;
}

void gdt_install()
{
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gp.base = (unsigned int) &gdt;

    /* NULL descriptor */
    gdt_set_gate(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    /* Kernel Code Segment entry */
    gdt_set_gate(1, 0xFFFF, 0xF, 0, 0, 0, 0, 1, 0, 1, 1, 0b00, 1, 1, 1, 0);

    /*Kernel Data Segment entry.*/
    gdt_set_gate(2, 0xFFFF, 0xF, 0, 0, 0, 0, 1, 0, 0, 1, 0b00, 1, 1, 1, 0);

    /*Userland Code Segment entry.*/
    gdt_set_gate(3, 0xFFFF, 0xF, 0, 0, 0, 0, 1, 0, 1, 1, 0b11, 1, 1, 1, 0);

    /*Userland Data Segment entry*/
    gdt_set_gate(4, 0xFFFF, 0xF, 0, 0, 0, 0, 1, 0, 0, 1, 0b11, 1, 1, 1, 0);


    /* Flush out the old GDT*/
    gdt_flush();
}
