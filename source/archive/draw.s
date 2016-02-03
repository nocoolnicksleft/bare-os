
.section .data
.align 1
foreColour:
.hword 0xFFFF

.align 2
graphicsAddress:
.int 0

.section .text

.globl SetForeColour
SetForeColour:
cmp r0,#0x10000
movhs pc,lr
ldr r1,=foreColour
strh r0,[r1]
mov pc,lr

.globl SetGraphicsAddress
SetGraphicsAddress:
ldr r1,=graphicsAddress
str r0,[r1]
mov pc,lr


.globl DrawPixel
DrawPixel:

px .req r0
py .req r1
addr .req r2
	ldr addr,=graphicsAddress
	ldr addr,[addr]
height .req r3
	ldr height,[addr,#4]
	sub height,#1
	cmp py,height
	movhi pc,lr
.unreq height

width .req r3
	ldr width,[addr,#0]
	sub width,#1
	cmp px,width
	movhi pc,lr

	ldr addr,[addr,#32]
	add width,#1
	mla px,py,width,px
.unreq width
.unreq py
	add addr, px,lsl #1
.unreq px

fore .req r3
	ldr fore,=foreColour
	ldrh fore,[fore]

	strh fore,[addr]
.unreq fore
.unreq addr
	mov pc,lr


.global DrawRect
DrawRect:
push {r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
mov r4, r0
mov r5, r1
mov r6, r2
mov r7, r3



px .req r0
py .req r1
addr .req r2
	ldr addr,=graphicsAddress
	ldr addr,[addr]
height .req r3
	ldr height,[addr,#4]
	sub height,#1
	cmp py,height
	pophi {r4,r5,r6,r7,r8,r9,r10,r11,r12,pc}
.unreq height

width .req r3
	ldr width,[addr,#0]
	sub width,#1
	cmp px,width
	pophi {r4,r5,r6,r7,r8,r9,r10,r11,r12,pc}

	ldr addr,[addr,#32]
	add width,#1
fore .req r10
	ldr fore,=foreColour
	ldrh fore,[fore]

    mov r11, addr


mov r8, r5
DRLineLoop:

mov r9, r4
DRPixelLoop:

mov r0, r9
mov r1, r8
bl DrawPixel
/*
mov addr, r11
	mla px,py,width,px
.unreq width
.unreq py
	add addr, px,lsl #1
.unreq px

	strh fore,[addr]
*/
add r9,r9,#1
cmp r9,r6
bls DRPixelLoop

add r8,r8,#1
cmp r8,r7
bls DRLineLoop

pop {r4,r5,r6,r7,r8,r9,r10,r11,r12,pc}



.globl DrawLine
DrawLine:
push {r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
x0 .req r9
x1 .req r10
y0 .req r11
y1 .req r12

mov x0,r0
mov x1,r2
mov y0,r1
mov y1,r3

dx .req r4
dyn .req r5 /* Note that we only ever use -deltay, so I store its negative for speed. (hence dyn) */
sx .req r6
sy .req r7
err .req r8

cmp x0,x1
subgt dx,x0,x1
movgt sx,#-1
suble dx,x1,x0
movle sx,#1

cmp y0,y1
subgt dyn,y1,y0
movgt sy,#-1
suble dyn,y0,y1
movle sy,#1

add err,dx,dyn
add x1,sx
add y1,sy

pixelLoop$:
teq x0,x1
teqne y0,y1
popeq {r4,r5,r6,r7,r8,r9,r10,r11,r12,pc}

mov r0,x0
mov r1,y0
bl DrawPixel

cmp dyn, err,lsl #1
addle err,dyn
addle x0,sx

cmp dx, err,lsl #1
addge err,dx
addge y0,sy

b pixelLoop$

.unreq x0
.unreq x1
.unreq y0
.unreq y1
.unreq dx
.unreq dyn
.unreq sx
.unreq sy
.unreq err




