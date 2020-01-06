// Serial code
#include "lpc17xx_uart.h" // Central include files
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "lpc17xx_gpio.h" // Local functions
#include <LPC17xx.h>

volatile uint32_t msTicks = 0;

void timer_enable_systick(void) {
    SysTick_Config(SystemCoreClock/1000);
}

void SysTick_Handler(void) {
  msTicks++;
}

void systick_delay(uint32_t delayTicks) {
  uint32_t currentTicks;

  // get tick count
  currentTicks = msTicks;
  // loop for delay
  while ((msTicks - currentTicks) < delayTicks);
}
