#include <stdio.h>


int uart0_init (unsigned int baud);

int uart0_rx (FILE* filestream);

int uart0_tx ( char byte, FILE* filestream);

/*
int uart0_tx_cast(char c, FILE* filestream);

int uart0_rx_cast(FILE* filestream);*/
