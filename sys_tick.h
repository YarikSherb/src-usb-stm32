#ifndef SYS_TICK_H
#define SYS_TICK_H

#include <stdint.h>
#include "stm32f10x.h"
#include "core_cm3.h"
#include "system_stm32f10x.h"

/// Частота тиков таймера, от которой считается все.
#define timertick_freq (1000U)

#define SysTick_Freq  timertick_freq  // Hz
//#define tick_in_ms (timertick_freq/(1000U))

void systick_up(void );

/* delay a time in milliseconds */
void delay(uint32_t count);

/* get system tick */
uint32_t get_tick(void);
/* suspend system tick */
void systick_DisableIRQ(void);
/* resume system tick */
void systick_EnableIRQ(void);

void systick_deinit(void);

uint32_t tick_update(void );

#endif /* SYS_TICK_H */
