#include "system.h"

/* Special pointer which includes the limit:*/
struct gdt_ptr
{
    uint16_t limit : 16;
    uint32_t base : 32;
}__attribute__((packed));

/* Our GDT, with 5 entries, and finally our special GDT pointer */
struct gdt_entry gdt[6];
struct gdt_ptr gp;

struct gdt_entry_64 gdt_64[3];
struct gdt_ptr gp_64;

/* function to reload the GDT our Gdt*/
extern void gdt_flush();
extern void gdt_flush_64();

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

void gdt64_set_gate(int num, uint16_t lowlimit, uint8_t highlimit, uint16_t lowbase, uint8_t middlebase, uint8_t middlebase2, uint32_t highbase, uint8_t a, uint8_t rw, uint8_t dc, uint8_t e, uint8_t s, uint8_t dpl, uint8_t p, uint8_t g, uint8_t db, uint8_t l)
{
    gdt_64[num].llimit = lowlimit;
    gdt_64[num].lbase = lowbase;
    gdt_64[num].mbase = middlebase;

    //access gate bits setting
    gdt_64[num].A = a;
    gdt_64[num].RW = rw;
    gdt_64[num].DC = dc;
    gdt_64[num].E = e;
    gdt_64[num].S = s;
    gdt_64[num].DPL = dpl;
    gdt_64[num].P = p;

    gdt[num].hlimit = highlimit;

    //flag byte setting
    gdt_64[num].reservedf = 0;
    gdt_64[num].L = l;
    gdt_64[num].DB = db;
    gdt_64[num].G = g;

    gdt_64[num].mbase2 = middlebase2;
    gdt_64[num].hbase = highbase;
    gdt_64[num].reserved = 0;
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

void set_64_gdt()
{
    gp_64.limit = (sizeof(struct gdt_entry_64) * 2) - 1;
    gp_64.base = (uint32_t)  &gdt_64;

    gdt64_set_gate(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    gdt64_set_gate(1, 0xFFFF, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0b00, 1, 1, 0, 1);
    gdt64_set_gate(2, 0xFFFF, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0b00, 1, 1, 0, 1);

    gdt_flush_64();
}
