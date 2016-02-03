/* Copyright (C) 2013 by John Cronin <jncronin@tysos.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MBOX_H
#define MBOX_H

#include <stdint.h>


/* Get firmware revision */
#define PROP_GETFIRMWAREREV_TAG 0x00000001
#define PROP_GETFIRMWAREREV_IN 0
#define PROP_GETFIRMWAREREV_OUT 4

/* Get board model */
#define PROP_GETBOARDMODEL_TAG 0x00010001
#define PROP_GETBOARDMODEL_IN 0
#define PROP_GETBOARDMODEL_OUT 4




/* Get board revision */
#define PROP_GETBOARDREV_TAG 0x00010002
#define PROP_GETBOARDREV_IN 0
#define PROP_GETBOARDREV_OUT 4

/*
0x2	B1	Original Model B, 256MB RAM, Ethernet, two USB sockets, five LEDs, (P2) JTAG pins, no mounting holes, Pin3=GPIO0, Pin5=GPIO1, Pin13=GPIO21, I2C-0
0x3	B1+	Original Model B with no polyfuses, 256MB RAM, Ethernet, two USB sockets, five LEDs, no mounting holes, Pin3=GPIO0, Pin5=GPIO1, Pin13=GPIO21, I2C-0
0x4	B2	Model B, 256MB RAM, Ethernet, two USB sockets, five LEDs, mounting holes, Pin3=GPIO1, Pin5=GPIO2, Pin13=GPIO27, 12C-1, 8 extra IO pads (P5)
0x5	B2	Model B, 256MB RAM, Ethernet, two USB sockets, five LEDs, mounting holes, Pin3=GPIO1, Pin5=GPIO2, Pin13=GPIO27, 12C-1, 8 extra IO pads (P5)
0x6	B2	Model B, 256MB RAM, Ethernet, two USB sockets, five LEDs, mounting holes, Pin3=GPIO1, Pin5=GPIO2, Pin13=GPIO27, 12C-1, 8 extra IO pads (P5)
0x7	A	Model A, 256MB RAM, no Ethernet, one USB socket, two LEDs, mounting holes, Pin3=GPIO1, Pin5=GPIO2, Pin13=GPIO27, I2C-1, 8 extra IO pads (P5)
0x8	A	Model A, 256MB RAM, no Ethernet, one USB socket, two LEDs, mounting holes, Pin3=GPIO1, Pin5=GPIO2, Pin13=GPIO27, I2C-1, 8 extra IO pads (P5)
0x9	A	Model A, 256MB RAM, no Ethernet, one USB socket, two LEDs, mounting holes, Pin3=GPIO1, Pin5=GPIO2, Pin13=GPIO27, I2C-1, 8 extra IO pads (P5)
0xd	B2	Rev2 Model B, 512MB RAM, Ethernet, two USB sockets, five LEDs, mounting holes, Pin3=GPIO1, Pin5=GPIO2, Pin13=GPIO27, 12C-1, 8 extra IO pads (P5)
0xe	B2	Rev2 Model B, 512MB RAM, Ethernet, two USB sockets, five LEDs, mounting holes, Pin3=GPIO1, Pin5=GPIO2, Pin13=GPIO27, 12C-1, 8 extra IO pads (P5)
0xf	B2	Rev2 Model B, 512MB RAM, Ethernet, two USB sockets, five LEDs, mounting holes, Pin3=GPIO1, Pin5=GPIO2, Pin13=GPIO27, 12C-1, 8 extra IO pads (P5)
*/

/* Get board MAC address */
#define PROP_GETBOARDMAC_TAG 0x00010003
#define PROP_GETBOARDMAC_IN 0
#define PROP_GETBOARDMAC_OUT 6

/* Get board serial */
#define PROP_GETBOARDSER_TAG 0x00010004
#define PROP_GETBOARDSER_IN 0
#define PROP_GETBOARDSER_OUT 8

/* Get ARM memory */
#define PROP_GETARMMEMORY_TAG 0x00010005
#define PROP_GETARMMEMORY_IN 0
#define PROP_GETARMMEMORY_OUT 8

/* Get VC memory */
#define PROP_GETVCMEMORY_TAG 0x00010006
#define PROP_GETVCMEMORY_IN 0
#define PROP_GETVCMEMORY_OUT 8

/* Get clocks */
#define PROP_GETCLOCKS_TAG 0x00010007
#define PROP_GETCLOCKS_IN 0
#define PROP_GETCLOCKS_OUT 8 /* *n */

/* Get command line */
#define PROP_GETCOMMANDLINE_TAG 0x00050001
#define PROP_GETCOMMANDLINE_IN 0
#define PROP_GETCOMMANDLINE_OUT 1 /* *n */

/* Get DMA channels */
#define PROP_GETDMACHANNELS_TAG 0x00060001
#define PROP_GETDMACHANNELS_IN 0
#define PROP_GETDMACHANNELSOUT 4

/* Get power state */
#define PROP_GETPOWERSTATE_TAG 0x00020001
#define PROP_GETPOWERSTATE_IN 4
#define PROP_GETPOWERSTATE_OUT 8

/* Get timing */
#define PROP_GETTIMING_TAG 0x00020002
#define PROP_GETTIMING_IN 4
#define PROP_GETTIMING_OUT 8



#define MBOX_BASE 0x2000b880

#define MBOX_PEEK 0x10
#define MBOX_READ 0x00
#define MBOX_WRITE 0x20
#define MBOX_STATUS 0x18
#define MBOX_SENDER 0x14
#define MBOX_CONFIG 0x1c

#define MBOX_FB		1
#define MBOX_PROP	8

#define MBOX_SUCCESS	0x80000000
#define MBOX_PARSE_ERROR	0x80000001

#ifdef __cplusplus
extern "C" {
#endif
    
uint32_t mbox_read(uint8_t channel);
void mbox_write(uint8_t channel, uint32_t data);

#ifdef __cplusplus
}
#endif


#endif

