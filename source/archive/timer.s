
.globl GetTimerAddress

GetTimerAddress:

	ldr r0,=0x20003000
	
	mov pc,lr


.globl GetTimeStamp

GetTimeStamp:

	push {lr}
	bl GetTimerAddress
	ldrd r0,r1,[r0,#4]
	pop {pc}

.globl Delay	
	
Delay:
	
delayt .req r2
	mov delayt,r0
	push {lr}
	bl GetTimeStamp
start .req r3
	mov start,r0	
	
loop$:
	bl GetTimeStamp
elapsed .req r1
	sub elapsed,r0,start
	cmp elapsed,delayt
.unreq elapsed
	bls loop$

.unreq delayt
.unreq start
	pop {pc}	
