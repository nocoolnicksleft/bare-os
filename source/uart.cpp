
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "startup.h"
#include "bcm2835.h"

#include "uart.h"

char _uart_getc() {
    
    char c;
    
    while (1) {
        if (AUX_MU_IIR_REG & AUX_MU_IIR_RECEIVED) break;
    }
        
    c = AUX_MU_IO_REG;

    return c;
}

void _uart_putc(const char c) {
    while (1) {
        if (AUX_MU_LSR_REG & AUX_MU_LSR_REG_TRANSMITTER_IDLE) break;
    }
    AUX_MU_IO_REG = c;
}

//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        _uart_putc(rc);
        if(rb==0) break;
    }
    _uart_putc(0x20);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    _uart_putc(0x0D);
    _uart_putc(0x0A);
}


//------------------------------------------------------------------------
/**
 * 
 * @param text
 */
void _uart_writes(const char* text) {
    while (*text) {
        _uart_putc(*text);
        text++;
    }
}

void _uart_init(void) {
    uint32 config;

    AUX_ENABLES = AUX_ENABLES_UART;

    AUX_MU_IER_REG = 0;

    AUX_MU_CNTL_REG = 0x0;

    AUX_MU_LCR_REG = AUX_MU_LCR_REG_8BIT;

    AUX_MU_MCR_REG = 0;

    // AUX_MU_IER_REG = AUX_MU_IER_ENABLEINT_TRANSMIT | AUX_MU_IER_ENABLEINT_RECEIVE;
    AUX_MU_IER_REG = AUX_MU_IER_ENABLEINT_RECEIVE;

    AUX_MU_IIR_REG = AUX_MU_IIR_CLEAR_RECEIVER | AUX_MU_IIR_CLEAR_TRANSMITTER | AUX_MU_IIR_ENABLEFIFO_RECEIVE | AUX_MU_IIR_ENABLEFIFO_TRANSMIT;

    AUX_MU_BAUD_REG = 0x10E;

    config = GPFSEL1;
    config &= ~GPFSEL1_PIN14_ALL;
    config |= GPFSEL1_PIN14_TXD1;
    config &= ~GPFSEL1_PIN15_ALL;
    config |= GPFSEL1_PIN15_RXD1;
    GPFSEL1 = config;

    GPPUD = GPPUD_OFF;
    delay(150);
    GPPUDCLK0 = (1 << 14) | (1 << 15);
    delay(150);
    GPPUDCLK0 = 0;

    AUX_MU_CNTL_REG = AUX_MU_CNTL_ENABLE_RECEIVER | AUX_MU_CNTL_ENABLE_TRANSMITTER;



    INTR_ENABLE_IRQ_1 |= INTR_IRQ_PENDING1_AUX;

}





