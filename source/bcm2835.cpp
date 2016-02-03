

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include "bcm2835.h"
#include "startup.h"

void SetGpioOutput(uint32_t pin, uint32_t value)
{
    
}

void SetGpioFunction(uint32_t pin, uint32_t value) 
{
    if (pin > LAST_GPIO_PIN) return;
    if (value > 7) return;
    
    uint32_t * bankaddress = (uint32_t *)0x20200000;
    
    while (pin > 9) {
        pin -= 10;
        bankaddress += 4;
    }
    memory_barrier();
    *(bankaddress) &= ~(7 << pin);
    *(bankaddress) |=  (value << pin);
    memory_barrier();
}

