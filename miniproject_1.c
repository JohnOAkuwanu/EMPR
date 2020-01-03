// Serial code
#include "lpc17xx_uart.h" // Central include files
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "lpc17xx_gpio.h" // Local functions
#include "serial.h"
#include <LPC17xx.h>
#include <string.h>

#define LED1 0b000001000000000000000000
#define LED2 0b000100000000000000000000
#define LED3 0b001000000000000000000000
#define LED4 0b100000000000000000000000

volatile uint32_t msTicks = 0;

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

void uart_debug_msg(char msg[]) {

  int length = strlen(msg) + 2; //increase length to compensate for EOL chars.
  char temp[length];            //define temp buffer with added length
  strcpy(temp, msg);            //copy msg into new buffer
  strcat(temp, "\r\n");         // add EOL chars
  write_usb_serial_blocking(temp, length);  //Write msg.

}

int LedOn(int LED) {
  GPIO_SetDir(1, LED, 1);
  GPIO_SetValue(1, LED);
  return (0);
}

int LedOff(int LED) {
  GPIO_ClearValue(1, LED);
  return (0);
}

void LEDflash(int LED) {
  LedOn(LED);
  systick_delay(1000);
  LedOff(LED);
}

char[] dec_to_bin(int i) {

  char bin[4] = ""; //string for binary representation.

  if ((i & 8) == 8) {
    strcat(bin, "1");

  } else {
    strcat(bin, "0");
  }

  if ((i & 4) == 4) {
    strcat(bin, "1");
  } else {
    strcat(bin, "0");
  }

  if ((i & 2) == 2) {
    strcat(bin, "1");
  } else {
    strcat(bin, "0");
  }

  if ((i & 1) == 1) {
    strcat(bin, "1");
  } else {
    strcat(bin, "0");
  }

}

void count(void) {

  uart_debug_msg("Starting Count");

int i;

  for (i=0; i < 16; i++) {

    int flash_val = 0;
    char bin[4] = ""; //string for binary representation.

    if ((i & 8) == 8) {
      flash_val |= LED4;

    } else {
    }

    if ((i & 4) == 4) {
      flash_val |= LED3;
    } else {;
    }

    if ((i & 2) == 2) {
      flash_val |= LED2;
    } else {
    }

    if ((i & 1) == 1) {
      flash_val |= LED1;
    } else {
    }

    char str[45];
    sprintf(str, "Decimal: %d Hexadecimal: 0x%d Binary: ", i,i);
    strcat(str, dec_to_bin(i));
    uart_debug_msg(str);
    LEDflash(flash_val);

  }

  uart_debug_msg("Finished count");

}

int main(void) {

  //generate an interrupt every millisecond //
  SysTick_Config(SystemCoreClock / 1000);

  //Inintialise UART //
  serial_init();
  count();
  while (1);
}
