#ifndef UART_H
#define UART_H

void uart2_init(void);
void uart2_send_char(char c);
void uart2_send_string(const char* str);

#endif
