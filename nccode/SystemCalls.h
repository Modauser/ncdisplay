/**
 * @file SystemCalls.h
 * @brief Exposes core functionality provided by main firmware.
 */

#ifndef SYSTEMCALLS_H_
#define SYSTEMCALLS_H_

/**
 * Gets the address of the system binary's printf function, used for UART
 * communication with the main board.
 * @return Address of the system's printf
 */
extern uint32_t getPrintf(void);

/**
 * Allows for serial connection testing, by reading from UART with a timeout.
 * Expects to read a '$' followed by a byte of data.
 * @return -1 if timeout or '$' not received, data byte otherwise
 */
int serialTest(void);

/**
 * Reads a character from UART. Function blocks until read complete.
 * @return Byte read from UART
 */
int serialGetchar(void);

/**
 * Sends a byte over SPI, reading in a byte during the transfer.
 * @param send The byte to send
 * @return The byte received
 */
uint8_t spi_xfer_byte(uint8_t send);

/**
 * Initiates a firmware update.
 * @param addr Address of the firmware update in memory
 * @param count The size in byte of the update
 */
void firmwareUpdate(uint32_t addr, uint32_t count);

#endif // SYSTEMCALLS_H_

