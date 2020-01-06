#include "delay.h"
#include "uart.h"
#include "i2c.h"
#include "lcd.h"

#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_gpio.h"

#include <stdio.h>

#define LCD_I2C_ADDRESS       (0x59)
#define KEYPAD_I2C_ADDRESS    (0x21)
#define UPPERLEVEL            (0x80)
#define LOWERLEVEL            (0xC0)

void sys_init(void)
{
  timer_enable_systick();
  uart_init();
  i2c_init();
  lcd_init();
  lcd_clear();
}


int main(void) {
  sys_init();
  //Stage 1
  scani2c();
  systick_delay(1000);
  uart_debug_msg("1 Second");

  //Stage 2

  char hw[] = "Hello World!";

  lcd_write_string(hw, 12, UPPERLEVEL);
  systick_delay(1000);
  lcd_clear();
  systick_delay(1000);

  char hello[] = "Hello";
  char world[] = "World!";
  lcd_write_string(hello, 5, UPPERLEVEL);
  lcd_write_string(world, 6, LOWERLEVEL);
  systick_delay(1000);
  lcd_clear();

  //Stage 3
  char upperlevel[16];
  char lowerlevel[16];
  char out[6];
  int upper_count = 0;
  int lower_count = 0;

  while (1) {

    uint8_t val = keypad_read();

    if (upper_count < 16) {

    upperlevel[upper_count] = val;
    upperlevel[upper_count + 1] = '\0';
    upper_count += 1;
    lcd_write_string(&upperlevel, strlen(upperlevel), UPPERLEVEL);

  } else {

    lowerlevel[lower_count] = val;
    lowerlevel[lower_count + 1] = '\0';
    lower_count += 1;
    lcd_write_string(&lowerlevel, strlen(lowerlevel), LOWERLEVEL);

    if (upper_count == 16 && lower_count == 16) {
      upper_count = 0;
      lower_count = 0;
    }

  }

  sprintf(out, "Key: %c", val);
  uart_debug_msg(out);

}
  return(0);
}
