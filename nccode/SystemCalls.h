/**
 * @file SystemCalls.h
 * @brief Exposes core functionality provided by main firmware.
 */
#ifndef SYSTEMCALLS_H_
#define SYSTEMCALLS_H_

/**
 * Sends the given byte over SPI, and returns the byte received during the
 * transfer.
 */
uint8_t spi_xfer_byte(uint8_t send);

int serialTest(void);

#endif // SYSTEMCALLS_H_

