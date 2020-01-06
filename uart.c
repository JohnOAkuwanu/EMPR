#include "serial.h"
#include <string.h>

void uart_init(void) {
  serial_init();
}

void uart_debug_msg(char msg[]) {

  int length = strlen(msg) + 2; //increase length to compensate for EOL chars.
  char temp[length];            //define temp buffer with added length
  strcpy(temp, msg);            //copy msg into new buffer
  strcat(temp, "\r\n");         // add EOL chars
  write_usb_serial_blocking(temp, length);  //Write msg.

}
