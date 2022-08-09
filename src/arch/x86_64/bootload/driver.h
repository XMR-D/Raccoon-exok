#ifndef __DRIVER_H
#define __DRIVER_H

extern int init_serial();
extern int is_buffer_empty();
extern void Send_char(char c);
extern void Send_log(unsigned char * log);

#endif
