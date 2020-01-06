#include <stdio.h>
#include <string.h>
#include "lpc17xx_i2c.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "i2c.h"
#include "delay.h"
#include "uart.h"
#include "serial.h"
#include "lcd.h"

#define KEYPAD_I2C_ADDRESS    (0x21)

int columnSet;
char previousChar = 'X';

uint8_t keypad_read_data(void) {

  uint8_t data[1];
  read_i2c(KEYPAD_I2C_ADDRESS, data, 1);

  return *data;
}

void keypad_column_set(int column) {

  uint8_t setHigh[1];

  if (column == 1) {
    setHigh[0] = 0x7F;
    columnSet = 1;
  }

  else if (column == 2) {
    setHigh[0] = 0xBF;
    columnSet = 2;
  }

  else if (column == 3) {
    setHigh[0] = 0xDF;
    columnSet = 3;
  }

  else if (column == 4) {
    setHigh[0] = 0xEF;
    columnSet = 4;
  }

  else {
    uart_debug_msg("Column is not in range 1 - 4");
  }

  write_i2c(KEYPAD_I2C_ADDRESS, setHigh, 1);

}

char keypad_get_char(void) {

  // read the value of the keypad
  uint8_t val = keypad_read_data();

  //bitmask to only see the lower nibble.,
  val = val & 0x0F;

  if (columnSet == 1) {

    if (val == 7) {
      return '1';
    }
    else if (val == 0x0B) {
      return '4';
    }
    else if (val == 0x0D) {
      return '7';
    }
    else if (val == 0x0E) {
      return '*';
    }
    else {
      return 'X';
    }

  }

  else if (columnSet == 2) {

    if (val == 7) {
      return '2';
    }
    else if (val == 0x0B) {
      return '5';
    }
    else if (val == 0x0D) {
      return '8';
    }
    else if (val == 0x0E) {
      return '0';
    }
    else {
      return 'X';
    }

  }

  else if (columnSet == 3) {

    if (val == 7) {
      return '3';
    }
    else if (val == 0x0B) {
      return '6';
    }
    else if (val == 0x0D) {
      return '9';
    }
    else if (val == 0x0E) {
      return '#';
    }
    else {
      return 'X';
    }

  }

  else if (columnSet == 4) {

    if (val == 7) {
      return 'A';
    }
    else if (val == 0x0B) {
      return 'B';
    }
    else if (val == 0x0D) {
      return 'C';
    }
    else if (val == 0x0E) {
      return 'D';
    }
    else {
      return 'X';
    }

  }

  else {
    uart_debug_msg("COLUMN NOT SET CORRECTLY");
    return 'X';
  }

}

char keypad_read(void) {

  int i;
  char c;
  char second;

  while (1) {

    for (i = 1; i < 5; i++) {

      keypad_column_set(i);
      c = keypad_get_char();

      if (c != 'X' && c != previousChar) {
        previousChar = c;
        systick_delay(100);
        return c;
      }
      else if (c == previousChar && c != 'X') {
          systick_delay(100);
          return c;
        }
      }

    }
  }
