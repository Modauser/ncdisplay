#ifndef SPI_HPP_
#define SPI_HPP_

#include <driver/spi_slave.h>

// SPI pin numbers
#define SPI_MOSI 13
#define SPI_MISO 12
#define SPI_SCK  14
#define SPI_CS   15
#define SPI_HS   2

/**
 * Provides the ability to conduct SPI transfers as an SPI slave.
 * We expect to receive data from the display through this interface.
 */
class SPIClass
{
private:
  // SPI driver callbacks, currently unused
  static void postSetupCallback(spi_slave_transaction_t *trans);
  static void postTransferCallback(spi_slave_transaction_t *trans);
  
  static const spi_bus_config_t config;
  static const spi_slave_interface_config_t slvcfg;

  static char txBuffer[128];
  static char rxBuffer[128];
  
public:
  /**
   * Initializes the SPI hardware component. Must be called before conducting a
   * transfer.
   */
  void begin(void)
  {
    auto error = spi_slave_initialize(HSPI_HOST, &config, &slvcfg, 2);
    if (error != ESP_OK)
      while (1);
  }

  /**
   * Releases the SPI hardware component from slave usage.
   */
  void end(void)
  {
    spi_slave_free(HSPI_HOST);
  }

  /**
   * Starts an SPI transfer that expects to receive the given number of bytes.
   * @param size Number of bytes to be expected
   * @return The receiving buffer containing received data
   */
  const char *receive(size_t size)
  {
    if (size > 128)
      size = 128;
    spi_slave_transaction_t trans;
    memset(&trans, 0, sizeof(trans));
    memset(rxBuffer, 0, 128);
    memset(txBuffer, 0x2A, 128);
    trans.length = size * 8;
    trans.tx_buffer = txBuffer;
    trans.rx_buffer = rxBuffer;

    spi_slave_transmit(HSPI_HOST, &trans, portMAX_DELAY);
    return rxBuffer;
  }
};

// SPI bus configuration
const spi_bus_config_t SPIClass::config = {
  .mosi_io_num = SPI_MOSI,
  .miso_io_num = SPI_MISO,
  .sclk_io_num = SPI_SCK
};
// SPI slave driver configuration
const spi_slave_interface_config_t SPIClass::slvcfg = {
  .spics_io_num = SPI_CS,
  .flags = 0,
  .queue_size = 1,
  .mode = SPI_MODE0,
  .post_setup_cb = postSetupCallback,
  .post_trans_cb = postTransferCallback
};
char SPIClass::txBuffer[128];
char SPIClass::rxBuffer[128];
void SPIClass::postSetupCallback(spi_slave_transaction_t *trans)
{}
void SPIClass::postTransferCallback(spi_slave_transaction_t *trans)
{}

// Create an instance of the SPI slave object.
static SPIClass SPI;

#endif // SPI_HPP_

