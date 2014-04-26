
#ifndef _bcm2835_h_
#define _bcm2835_h_

typedef unsigned int uint32;
typedef unsigned short int u16;
typedef unsigned char u8;

typedef volatile unsigned int vuint32;
typedef volatile bool vbool;

struct fbInfo {
   uint32 physicalWidth; 
   uint32 physicalHeight; 
   uint32 virtualWidth; 
   uint32 virtualHeight; 
   uint32 gpuPitch;
   uint32 bitDepth;
   uint32 x;
   uint32 y;
   uint32 pointer;
   uint32 size;
};

#ifdef __cplusplus
extern "C" {
#endif

uint32_t GET32(uint32_t address);
void PUT32(uint32_t address, uint32_t value);
void dummy(int a);
void delay(int a);

void SetGpio(uint32_t pin, uint32_t value);
void SetGpioFunction(uint32_t, uint32_t value);

extern fbInfo * InitialiseFrameBuffer(uint32 width, uint32 height, uint32 colordepth);

uint32_t Random(uint32_t seed);
void SetGraphicsAddress(fbInfo * address);
void SetForeColour(uint32_t address);
void DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);
void DrawRect(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);

void enable_irq(void);
void enable_fiq(void);

#ifdef __cplusplus
}
#endif

#define GPFSEL0 (*(vuint32*) 0x20200000)
#define GPFSEL0_PIN9_OUTPUT (0b001 << 27)
#define GPFSEL0_PIN9_SPI0_MISO (0b100 << 27)
#define GPFSEL0_PIN9_SD1 (0b101 << 27)
#define GPFSEL0_PIN9_FUNC02 (0b110 << 27)
#define GPFSEL0_PIN9_FUNC03 (0b111 << 27)
#define GPFSEL0_PIN9_FUNC04 (0b011 << 27)
#define GPFSEL0_PIN9_FUNC05 (0b010 << 27)

#define GPFSEL1 (*(vuint32*) 0x20200004)

#define GPFSEL1_PIN14_OUTPUT (0b001 << 12)
#define GPFSEL1_PIN14_TXD0 (0b100 << 12)
#define GPFSEL1_PIN14_SD6 (0b101 << 12)
#define GPFSEL1_PIN14_TXD1 (0b010 << 12)
#define GPFSEL1_PIN14_ALL (0b111 << 12)

#define GPFSEL1_PIN15_OUTPUT (0b001 << 15)
#define GPFSEL1_PIN15_RXD0 (0b100 << 15)
#define GPFSEL1_PIN15_SD7 (0b101 << 15)
#define GPFSEL1_PIN15_RXD1 (0b010 << 15)
#define GPFSEL1_PIN15_ALL (0b111 << 15)



#define GPFSEL2 (*(vuint32*) 0x20200008)
#define GPFSEL3 (*(vuint32*) 0x2020000C)
#define GPFSEL4 (*(vuint32*) 0x20200010)
#define GPFSEL5 (*(vuint32*) 0x20200014)

#define GPSET0 (*(vuint32*) 0x2020001C)
#define GPSET1 (*(vuint32*) 0x20200020)

#define GPCLR0 (*(vuint32*) 0x20200028)
#define GPCLR1 (*(vuint32*) 0x2020002C)

#define GPLEV0 (*(vuint32*) 0x20200034)
#define GPLEV1 (*(vuint32*) 0x20200038)

#define GPEDS0 (*(vuint32*) 0x20200040)
#define GPEDS1 (*(vuint32*) 0x20200044)
#define GPREN0 (*(vuint32*) 0x2020004C)
#define GPREN1 (*(vuint32*) 0x20200050)
#define GPFEN0 (*(vuint32*) 0x20200058)
#define GPFEN1 (*(vuint32*) 0x2020005C)
#define GPHEN0 (*(vuint32*) 0x20200064)
#define GPHEN1 (*(vuint32*) 0x20200068)
#define GPLEN0 (*(vuint32*) 0x20200070)
#define GPLEN1 (*(vuint32*) 0x20200074)
#define GPAREN0 (*(vuint32*) 0x2020007C)
#define GPAREN1 (*(vuint32*) 0x20200080)
#define GPAFEN0 (*(vuint32*) 0x20200088)
#define GPAFEN1 (*(vuint32*) 0x2020008C)

#define GPPUD (*(vuint32*) 0x20200094)
#define GPPUD_OFF 0x0
#define GPPUD_ENABLE_PULLDOWN 0x1
#define GPPUD_ENABLE_PULLUP 0x2

#define GPPUDCLK0 (*(vuint32*) 0x20200098)
#define GPPUDCLK1 (*(vuint32*) 0x2020009C)





#define AUX_IRQ (*(vuint32*) 0x20215000)
#define AUX_IRQ_UART 0x1
#define AUX_IRQ_SPI1 0x2
#define AUX_IRQ_SPI2 0x4

#define AUX_ENABLES (*(vuint32*) 0x20215004)
#define AUX_ENABLES_UART 0x1
#define AUX_ENABLES_SPI1 0x2
#define AUX_ENABLES_SPI2 0x4

#define AUX_MU_IO_REG (*(vuint32*) 0x20215040)

#define AUX_MU_IER_REG (*(vuint32*) 0x20215044)
#define AUX_MU_IER_ENABLEINT_TRANSMIT 0x1
#define AUX_MU_IER_ENABLEINT_RECEIVE 0x2

#define AUX_MU_IIR_REG (*(vuint32*) 0x20215048)
#define AUX_MU_IIR_INTERRUPT_PENDING 0x1
#define AUX_MU_IIR_TRANSMITTED 0x2
#define AUX_MU_IIR_RECEIVED 0x4
#define AUX_MU_IIR_CLEAR_RECEIVER 0x2
#define AUX_MU_IIR_CLEAR_TRANSMITTER 0x4
#define AUX_MU_IIR_ENABLEFIFO_TRANSMIT 0x6
#define AUX_MU_IIR_ENABLEFIFO_RECEIVE 0x7


#define AUX_MU_LCR_REG (*(vuint32*) 0x2021504C)
#define AUX_MU_LCR_REG_5BIT 0x0
#define AUX_MU_LCR_REG_6BIT 0x1
#define AUX_MU_LCR_REG_7BIT 0x2
#define AUX_MU_LCR_REG_8BIT 0x3
#define AUX_MU_LCR_REG_DLABACCESS 0x80

#define AUX_MU_MCR_REG (*(vuint32*) 0x20215050)

#define AUX_MU_LSR_REG (*(vuint32*) 0x20215054)
#define AUX_MU_LSR_REG_DATA_READY 0x1
#define AUX_MU_LSR_REG_RECEIVER_OVERRUN 0x2
#define AUX_MU_LSR_REG_TRANSMITTER_EMPTY 0x20
#define AUX_MU_LSR_REG_TRANSMITTER_IDLE 0x40




#define AUX_MU_MSR_REG (*(vuint32*) 0x20215058)
#define AUX_MU_SCRATCH (*(vuint32*) 0x2021505C)

#define AUX_MU_CNTL_REG (*(vuint32*) 0x20215060)
#define AUX_MU_CNTL_ENABLE_RECEIVER 0x01
#define AUX_MU_CNTL_ENABLE_TRANSMITTER 0x02

#define AUX_MU_STAT_REG (*(vuint32*) 0x20215064)
#define AUX_MU_BAUD_REG (*(vuint32*) 0x20215068)

#define AUX_SPI0_CNTL0_REG (*(vuint32*) 0x20215080)
#define AUX_SPI0_CNTL1_REG (*(vuint32*) 0x20215084)
#define AUX_SPI0_STAT_REG (*(vuint32*) 0x20215088)
#define AUX_SPI0_IO_REG (*(vuint32*) 0x20215090)
#define AUX_SPI0_PEEK_REG (*(vuint32*) 0x20215094)

#define AUX_SPI1_CNTL0_REG (*(vuint32*) 0x202150C0)
#define AUX_SPI1_CNTL1_REG (*(vuint32*) 0x202150C4)
#define AUX_SPI1_STAT_REG (*(vuint32*) 0x202150C8)
#define AUX_SPI1_IO_REG (*(vuint32*) 0x202150D0)
#define AUX_SPI1_PEEK_REG (*(vuint32*) 0x202150D4)




#define TIMER_CS (*(vuint32*)0x20003000) 
#define TIMER_CS_MATCH0 0x01 // USED BY GPU
#define TIMER_CS_MATCH1 0x02 // SEEMS TO BE USED
#define TIMER_CS_MATCH2 0x04 // USED BY GPU
#define TIMER_CS_MATCH3 0x08 // RESERVED FOR LINUX / OS
#define TIMER_CLO (*(vuint32*)0x20003004) 
#define TIMER_CHI (*(vuint32*)0x20003008) 
#define TIMER_C0 (*(vuint32*)0x2000300C) // USED BY GPU
#define TIMER_C1 (*(vuint32*)0x20003010) // SEEMS TO BE USED / DOES NOT WORK PROPERLY
#define TIMER_C2 (*(vuint32*)0x20003014) // USED BY GPU
#define TIMER_C3 (*(vuint32*)0x20003018) // RESERVED FOR LINUX / OS

#define INTR_IRQ_BASIC_PENDING (*(vuint32*)0x2000B200) 
#define INTR_IRQ_BASIC_PENDING_TIMER 0x01
#define INTR_IRQ_BASIC_PENDING_MAILBOX 0x02
#define INTR_IRQ_BASIC_PENDING_DOORBELL0 0x04
#define INTR_IRQ_BASIC_PENDING_DOORBELL1 0x08
#define INTR_IRQ_BASIC_PENDING_GPUPENDING1 0x100
#define INTR_IRQ_BASIC_PENDING_GPUPENDING2 0x200

#define INTR_IRQ_PENDING1 (*(vuint32*)0x2000B204) 
#define INTR_IRQ_PENDING1_SYSTEMTIMER_C0 0x01 // USED BY GPU
#define INTR_IRQ_PENDING1_SYSTEMTIMER_C1 0x02 // SEEMS TO BE USED / DOES NOT WORK PROPERLY
#define INTR_IRQ_PENDING1_SYSTEMTIMER_C2 0x04 // USED BY GPU
#define INTR_IRQ_PENDING1_SYSTEMTIMER_C3 0x08 // RESERVED FOR LINUX / OS
#define INTR_IRQ_PENDING1_AUX 0x1D

#define INTR_IRQ_PENDING2 (*(vuint32*)0x2000B208) 
#define INTR_FIQ_CONTROL (*(vuint32*)0x2000B20C) 
#define INTR_ENABLE_IRQ_1 (*(vuint32*)0x2000B210) 
#define INTR_ENABLE_IRQ_2 (*(vuint32*)0x2000B214) 
#define INTR_ENABLE_BASIC (*(vuint32*)0x2000B218) 

#define INTR_DISABLE_IRQ_1 (*(vuint32*)0x2000B21C) 
#define INTR_DISABLE_IRQ_2 (*(vuint32*)0x2000B220) 
#define INTR_DISABLE_BASIC (*(vuint32*)0x2000B224) 

#endif

