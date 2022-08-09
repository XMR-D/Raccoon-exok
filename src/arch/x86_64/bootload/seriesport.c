#include "system.h"

int init_serial()
{
   outb(0x3f8 + 1, 0x00); //shut off the interrupts

   outb(0x3f8 + 3, 0x80); //Connection speed (baud rate = 3);
   outb(0x3f8 + 0, 0x03);
   outb(0x3f8 + 1, 0x00);

   outb(0x3f8 + 3, 0x03); // others init (parity, stop bit nb ...);
   outb(0x3f8 + 2, 0xC7);
   outb(0x3f8 + 4, 0x0B);
   outb(0x3f8 + 4, 0x1E);
   outb(0x3f8 + 0, 0xAE);

   if(inb(0x3f8 + 0) != 0xAE)
      return 1;

   outb(0x3f8 + 4, 0x0F);
   return 0;
}

int is_buffer_empty()
{
    return inb(0x3f8 + 5) & 0x20;
}

void Send_char(char c)
{
    init_serial();
    while(is_buffer_empty() == 0);
    outb(0x3f8, c);
}

void Send_log(unsigned char * log)
{
    init_serial();
    for(int i = 0; log[i] != '\0'; i++)
        Send_char(log[i]);
}

