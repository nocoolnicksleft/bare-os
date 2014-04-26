.section .init
.global _start
.arm /* = ARM Mode */


_start:

/******************************************************************************
 *  Exception vector table                                                    *
 ******************************************************************************/

    ldr pc,reset_handler_address
    ldr pc,undef_handler_address
    ldr pc,swi_handler_address
    ldr pc,pabort_handler_address
    ldr pc,dabort_handler_address
    ldr pc,unused_handler_address
    ldr pc,irq_handler_address
    ldr pc,fiq_handler_address
reset_handler_address:      .word reset
undef_handler_address:      .word undef_handler
swi_handler_address:        .word swi_handler
pabort_handler_address:     .word pabort_handler
dabort_handler_address:     .word dabort_handler
unused_handler_address:     .word stop
irq_handler_address:        .word irq_handler
fiq_handler_address:        .word fiq_handler

/******************************************************************************
 * Main startup sequence                                                     *
 ******************************************************************************/
reset:
/*******************************/
/* Copy vector table to 0x0000 */
/*******************************/
    mov r0,#0x8000
    mov r1,#0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}


/*************************/
/* Set stack pointers    */
/*************************/
.equ UND_Stack_Size, 0x2000  /*  8 kByte */
.equ ABT_Stack_Size, 0x2000  /*  8 kByte */
.equ FIQ_Stack_Size, 0x10000 /* 64 kByte */
.equ IRQ_Stack_Size, 0x10000 /* 64 kByte */
.equ SVC_Stack_Size, 0x10000 /* 64 kByte */
.equ F_Bit, 0x20
.equ I_Bit, 0x40
.equ Mode_UND, 0b11011
.equ Mode_ABT, 0b10111
.equ Mode_FIQ, 0b10001
.equ Mode_IRQ, 0b10010
.equ Mode_SVC, 0b10011
.equ Mode_USR, 0b10000

    /* Setup stack pointers */
    ldr     r0, =_STACK_END_

    /* Undefined Instruction Mode */
    msr     cpsr_c, #Mode_UND|I_Bit|F_Bit
    mov     sp, r0
    sub     r0, r0, #UND_Stack_Size

    /* Abort Mode */
    msr     cpsr_c, #Mode_ABT|I_Bit|F_Bit
    mov     sp, r0
    sub     r0, r0, #ABT_Stack_Size

    /* FIQ Mode */
    msr     cpsr_c, #Mode_FIQ|I_Bit|F_Bit
    mov     sp, r0
    sub     r0, r0, #FIQ_Stack_Size

    /* IRQ Mode */
    msr     cpsr_c, #Mode_IRQ|I_Bit|F_Bit
    mov     sp, r0
    sub     r0, r0, #IRQ_Stack_Size

    /* Supervisor Mode */
    msr     cpsr_c, #Mode_SVC|I_Bit|F_Bit
    mov     sp, r0
    sub     r0, r0, #SVC_Stack_Size

    /* User mode */
    msr     cpsr_c, #Mode_USR
    mov     sp, r0

/*************************/
/* Clear bss memory area */
/*************************/
    ldr	r4, =_BSS_START_
    ldr	r9, =_BSS_END_
    mov	r5, #0
    mov	r6, #0
    mov	r7, #0
    mov	r8, #0
    b       2f

1:
    stmia r4!, {r5-r8}  /* Store 4x32 bit at r4 */
 
2:
    cmp	r4, r9 /* Loop until _BSS_END_ is reached */
    blo	1b

/*************************/
/* Call constructors     */
/*************************/
    ldr 	r0, =_CTORS_START_
    ldr 	r1, =_CTORS_END_
_ctor_loop:
    cmp 	r0, r1
    beq 	_ctor_end
    ldr 	r2, [r0], #4
    stmfd 	sp!, {r0-r1}
    mov 	lr, pc
    mov 	pc, r2
    ldmfd 	sp!, {r0-r1}
    b 		_ctor_loop
_ctor_end:

/****************************/
/* Off to C function main() */
/****************************/
    mov r0, #0
    mov r1, #0

    b main
    
        
/******************************************************************************
 * Stop doing anything                                                        *
 ******************************************************************************/
stop:
    cpsid i 
    cpsid f
    wfe
    b stop

/******************************************************************************
 * Abort (needed by C++ exceptions)                                           *
 ******************************************************************************/
.globl abort
abort:
    b abort

/******************************************************************************
 * Enable IRQ interrupts                                                      *
 ******************************************************************************/
.globl enable_irq
enable_irq:
    cpsie i
    bx lr

/******************************************************************************
 * Enable FIQ interrupts                                                      *
 ******************************************************************************/
.globl enable_fiq
enable_fiq:
    cpsie f
    bx lr

/******************************************************************************
 *                                                                            *
 ******************************************************************************/
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

/******************************************************************************
 *                                                                            *
 ******************************************************************************/
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

/******************************************************************************
 *                                                                            *
 ******************************************************************************/
.globl GETSP
GETSP:
    mov r0, sp
    bx lr

/******************************************************************************
 * Delay execution for r0 cycles (precise for even numbers > 6)               *
 ******************************************************************************/
.globl delay
delay:
    cmp r0, #4
    bxls lr
    lsr r0, #1
 loop:
    subs r0, #1
    bne loop
    bx lr

/******************************************************************************
 * Default exception handlers                                                 *
 * These are declared weak symbols so they can be redefined in user code.     * 
 ******************************************************************************/
.equ    UARTport, 0x20215040
undef_handler:
  ldr r1, =UARTport
  mov r0, #0x55
  str r0, [r1]
  b undef_handler
  
swi_handler:
  ldr r1, =UARTport
  mov r0, #0x53
  str r0, [r1]
  b swi_handler
  
pabort_handler:
  ldr r1, =UARTport
  mov r0, #0x50
  str r0, [r1]
  b pabort_handler
  
dabort_handler:
  ldr r1, =UARTport
  mov r0, #0x44
  str r0, [r1]
  b dabort_handler
  
fiq_handler:
  b fiq_handler

irq_handler:
  b irq_handler

  .weak undef_handler, swi_handler, pabort_handler, dabort_handler, fiq_handler, irq_handler
                           


.globl flush_cache
flush_cache:
	mov 	r0, #0
	mcr	p15, #0, r0, c7, c14, #0
	mov	pc, lr

.globl memory_barrier
memory_barrier:
	mov	r0, #0
	mcr	p15, #0, r0, c7, c10, #5
	mov	pc, lr

.globl read_sctlr
read_sctlr:
	mrc	p15, #0, r0, c1, c0, #0
	mov	pc, lr

.globl quick_memcpy
quick_memcpy:
	push 	{r4-r9}
	mov	r4, r0
	mov	r5, r1

.loopb:
	ldmia	r5!, {r6-r9}
	stmia	r4!, {r6-r9}
	subs	r2, #16
	bhi	.loopb

	pop	{r4-r9}
	mov	pc, lr

                         