#include "sys_tick.h"

volatile  uint32_t timer_tick = 0;
volatile static uint32_t tick = 0;

//void timer_up(void );
void systick_config(void);
void delay_decrement(void);
// void systick_DisableIRQ(void);
// void systick_EnableIRQ(void);
void systick_Disable(void);
void systick_Enable(void);
// void systick_deinit(void);

uint32_t tick_update(void )
{
  uint32_t dt = get_tick() - timer_tick;
  timer_tick = get_tick();
  //if(dt>=0x7f) dt ^= 0xff;
  return dt;
}

void systick_up(void )
{
  systick_config();
  systick_Enable();
  systick_EnableIRQ();
}

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void) {
    /* setup systick timer for (SysTick_Freq) Hz interrupts */
    if (SysTick_Config(SystemCoreClock / SysTick_Freq)) {
        /* capture error */
        while (1) {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x05U);
}

/*!
    \brief      delay decrement
    \param[in]  none
    \param[out] none
    \retval     none
*/
inline void delay_decrement(void) {
    tick++;
}

inline uint32_t get_tick(void) {
    return tick;
}

/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay(uint32_t count) {
    uint32_t utick;
    utick = count + get_tick();/// count*tick_in_ms

    while (get_tick() < utick) {
    }
}

void systick_DisableIRQ(void) {
    /* Disable SysTick Interrupt */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void systick_EnableIRQ(void) {
    /* Enable SysTick Interrupt */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void systick_Disable(void) {
    /* Disable SysTick */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void systick_Enable(void) {
    /* Enable SysTick */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void systick_deinit(void) {
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    tick++;
}