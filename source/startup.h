/* 
 * File:   startup.h
 * Author: bseip
 *
 * Created on May 2, 2014, 10:19 AM
 */

#ifndef STARTUP_H
#define	STARTUP_H

#ifdef	__cplusplus
extern "C" {
#endif
// uint32_t GET32(uint32_t address);
// void PUT32(uint32_t address, uint32_t value);
extern unsigned int GETSP(void);
void delay(int a);
void enable_irq(void);
void enable_fiq(void);
void abort(void);
void stop(void);

void flush_cache(void);
void memory_barrier(void);
void read_sctlr(void);
void quick_memcpy(void);

        

#ifdef	__cplusplus
}
#endif

#endif	/* STARTUP_H */

