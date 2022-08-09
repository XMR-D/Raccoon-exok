#include "system.h"

struct cpu{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t eflags;
    uint32_t nberr, err_code;
    uint32_t gs, fs, es, ds, ss;
};

extern void irq_handler();

void irqe0()
{
    //sprintf((unsigned char *) "tick !\n");
}

void irqe1()
{
    keyboard_handling();
}

void irqe2()
{
    printf((unsigned char *) "IRQ %u triggered\n", 2);
}

void irqe3()
{
    printf((unsigned char *) "IRQ %u triggered\n", 3);
}

void irqe4()
{
    printf((unsigned char *) "IRQ %u triggered\n", 4);
}

void irqe5()
{
    printf((unsigned char *) "IRQ %u triggered\n", 5);
}

void irqe6()
{
    printf((unsigned char *) "IRQ %u triggered\n", 6);
}

void irqe7()
{
    printf((unsigned char *) "IRQ %u triggered\n", 7);
}

void irqe8()
{
    printf((unsigned char *) "IRQ %u triggered\n", 8);
}

void irqe9()
{
    printf((unsigned char *) "IRQ %u triggered\n", 9);
}

void irqe10()
{
    printf((unsigned char *) "IRQ %u triggered\n", 10);
}

void irqe11()
{
    printf((unsigned char *) "IRQ %u triggered\n", 11);
}

void irqe12()
{
    printf((unsigned char *) "IRQ %u triggered\n", 12);
}

void irqe13()
{
    printf((unsigned char *) "IRQ %u triggered\n", 13);
}

void irqe14()
{
    printf((unsigned char *) "IRQ %u triggered\n", 14);
}

void irqe15()
{
    printf((unsigned char *) "IRQ %u triggered\n", 15);
}

typedef void (*Handler)(void);

Handler jumptableirq[16] = {irqe0, irqe1, irqe2, irqe3, irqe4, irqe5, irqe6, irqe7, irqe8, irqe9, irqe10, irqe11, irqe12, irqe13, irqe14, irqe15};

unsigned int tab[2];

void Basic_irq_handling()
{
    jumptableirq[tab[1]-32]();
    outb(0x20, 0x20);
}

void Irq_remap()
{
    //ICW1 -- initialisation begin
    outb(0x20, 0x11);
    outb(0xA0, 0x11);


    //ICW2 -- remap interrupt numbers
    outb(0x21, 0x20);
    outb(0xA1 , 0x28);

    //ICW3 -- make the two pic communicate
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    //ICW4
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    //Finished
    outb(0x21, 0x00);
    outb(0xA1, 0x00);

}



