#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include "DAC.h"
#include "core/mzapo_phys.h"
#include "core/mzapo_regs.h"

#define PMOD1 *(volatile uint32_t*)(pmod1 + DCSPDRV_REG_CR_o)
#define PMOD2 *(volatile uint32_t*)(pmod2 + DCSPDRV_REG_CR_o)

#define SER 4
#define SRCLK 5
#define RCLK 4

#define DAC_RESOLUTION 16
#define MSB (1 << DAC_RESOLUTION)
#define OFFSET 6

struct timespec interval = {.tv_sec = 0, .tv_nsec = 1000000};


void DAC::init() {
    pmod1 = (unsigned char*) map_phys_address(DCSPDRV_REG_BASE_PHYS_0, SPILED_REG_SIZE, 0);
    pmod2 = (unsigned char*) map_phys_address(DCSPDRV_REG_BASE_PHYS_1, SPILED_REG_SIZE, 0);
}

void DAC::write(uint16_t value) {
   value <<= OFFSET;
    uint32_t reg_val = PMOD1;

    for (int i = 0; i < DAC_RESOLUTION; i++) {
       
        if ((value >> i) & 1) reg_val |= (1 << SER);
        else reg_val &= ~(1 << SER);
        
        PMOD1 = reg_val;

        PMOD1 = reg_val | (1 << SRCLK);
        PMOD1 = reg_val & ~(1 << SRCLK);
    }

}

void DAC::latch() {
    PMOD2 |= (1 << RCLK);
    PMOD2 &= ~(1 << RCLK);
}