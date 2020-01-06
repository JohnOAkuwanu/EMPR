#include <stdio.h>
#include <string.h>
#include "lpc17xx_i2c.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "i2c.h"
#include "delay.h"
#include "uart.h"
#include "serial.h"

#define LCD_I2C_ADDRESS			(0x3b)
#define UPPERLEVEL					(0x80)
#define LOWERLEVEL					(0xC0)

void lcd_init(void) {

  uint8_t setupBytes[] = {0x00, 0x34, 0x0C, 0x06, 0x35, 0x04, 0x10, 0x42, 0x9f, 0x34, 0x02};
  write_i2c(LCD_I2C_ADDRESS, setupBytes, 11);

}

void lcd_write_char(uint8_t addr, uint8_t c, int level) {
	uint8_t data[] = {0x00, addr + level};
	write_i2c(LCD_I2C_ADDRESS, data, 2);
	systick_delay(1);
	data[0] = 0x40;
	data[1] = c;
	write_i2c(LCD_I2C_ADDRESS, data, 2);
	systick_delay(1);
}

void lcd_clear(void) {

	int i;
	int clear_char = 0xA0; //Space

	for (i = 0; i < 16; i++) {
		lcd_write_char(i, clear_char, UPPERLEVEL);
		lcd_write_char(i, clear_char, LOWERLEVEL);
	}

}

void lcd_write_string(char str[], uint8_t str_length, uint8_t level) {

  int i;

  for (i = 0; i < str_length; i++) {

    lcd_write_char(i, str[i]+128, level);

  }

}




/*
uint8_t *lcdmap(uint8_t *buf, const char *str)
{
	while (*str)
	{
		*buf = (uint8_t)*str;
		if (*str < 'a' || *str > 'z') *buf += 128;
		++buf;
		++str;
	}
	return buf;
}*/

/*
void LCDTopLine(void)
{
	uint8_t topLine[] = {0
    char c = string[i];x00, 0x80};
	write_i2c(LCD_I2C_ADDRESS, topLine, 2);
  systick_delay(1000);
}

void LCDWriteChar(char C)
{
	char ASCII[2] = {C};
	uint8_t letter[2];
	text2LCDBytes(ASCII, 2, letter);
	write_i2c(LCD_I2C_ADDRESS, letter, 2);
}
*/
/*

uint8_t * text2LCDBytes(char string[], int len, uint8_t out[])
{
	out[0] = 0x40;
	int i;
	for(i = 0; i < len-1; i++)
	{
		// Check if the character is a letter (UPPER or lower)
		if((' ' <= string[i] && string[i] <= 'Z') || ('a' <= string[i] && string[i] <= 'z'))
		{
			out[i+1] = string[i] + 128;
		}
		else
		{
			out[i+1] = string[i];
		}
	}
	return out;
}
*/
