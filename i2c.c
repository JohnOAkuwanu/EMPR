#include "LPC17xx.h"
#include "lpc_types.h"
#include "i2c.h"
#include <stdio.h>
#include <string.h>
#include "lpc17xx_i2c.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lcd.h"
#include "uart.h"

void i2c_init(void) {

	// Configure mbed pins to act as I2C pins.
	PINSEL_CFG_Type pin = { .Funcnum = 3 };
	PINSEL_ConfigPin(&pin);
	pin.Pinnum = 1;
	PINSEL_ConfigPin(&pin);
	// Set the I2C controller Clock Rate
	I2C_Init(LPC_I2C1, 100000);
	// Enable the I2C controller.
	I2C_Cmd(LPC_I2C1, ENABLE);
}

Status write_i2c(char addr, uint8_t *data, uint8_t len) {

	//I2C transmission config
	I2C_M_SETUP_Type setupSequence;
	setupSequence.sl_addr7bit = addr;
	setupSequence.tx_data = data;
	setupSequence.tx_length = len;
	setupSequence.rx_data = NULL;
	setupSequence.rx_length = 0;
	setupSequence.retransmissions_max = 0;

	// Transfer data & return status
	return I2C_MasterTransferData(LPC_I2C1, &setupSequence, I2C_TRANSFER_POLLING);
}

Status read_i2c(char addr, uint8_t *data, uint8_t len) {

	//I2C transmission config
	I2C_M_SETUP_Type setupSequence;
	setupSequence.sl_addr7bit = addr;
	setupSequence.tx_data = NULL;
	setupSequence.tx_length = 0;
	setupSequence.rx_data = data;
	setupSequence.rx_length = len;
	setupSequence.retransmissions_max = 0;

	// Transfer data & return status
	return I2C_MasterTransferData(LPC_I2C1, &setupSequence, I2C_TRANSFER_POLLING);
}

void scani2c(void) {

	uint8_t data[1] = {0};
	int connected_devices = 0;
	char msg[36];
  int i;

	for (i = 0; i < (1 << 7); i++) {

		//If I2C address can recieve data, increment connected_devices.
		if (write_i2c(i, data, 2) == SUCCESS) {

			connected_devices++;
			sprintf(msg, "Device present at address: 0x%x", i);
			uart_debug_msg(msg);

		}
	}

	sprintf(msg, "%d devices connected to the i2c bus", connected_devices);
	uart_debug_msg(msg);

}
