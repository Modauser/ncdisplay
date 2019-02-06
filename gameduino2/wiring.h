#include <atmel_start_pins.h>

extern uint8_t spi_xfer_byte(uint8_t);

#define YIELD()
#define lowByte(n)  ((n) & 0xFF)
#define highByte(n) (((n) >> 8) & 0xFF)

class GDTransport {
private:
  byte model;
public:
  void begin0() {
    hostcmd(0x42);    // SLEEP
    hostcmd(0x61);    // CLKSEL default
    hostcmd(0x00);    // ACTIVE
//#if (BOARD != BOARD_GAMEDUINO23)
//    hostcmd(0x44);    // CLKEXT
//#else
    hostcmd(0x48);    // CLKINT
//#endif
    hostcmd(0x49);    // PD_ROMS all up
    hostcmd(0x68);    // RST_PULSE
  }

  void begin1() {
    uint16_t val = 0;
    do {
      val = __rd16(0xC0000UL);
    } while ((val & 0xff) != 0x08); // or delay 120ms

    // Test point: saturate SPI
    while (0) {
      gpio_set_pin_level(SPI_CS_N, false);
      spi_xfer_byte(0x55);
      gpio_set_pin_level(SPI_CS_N, true);
    }

    // So that FT800,801      FT81x
    // model       0            1
    ft8xx_model = __rd16(0x0c0000) >> 12;  

    wp = 0;
    freespace = 4096 - 4;

    stream();
  }

  void cmd32(uint32_t x) {
    if (freespace < 4) {
      getfree(4);
    }
    wp += 4;
    freespace -= 4;
#if defined(ESP8266)
    // SPI.writeBytes((uint8_t*)&x, 4);
    SPI.write32(x, 0);
#else
    union {
      uint32_t c;
      uint8_t b[4];
    };
    c = x;
    spi_xfer_byte(b[0]);
    spi_xfer_byte(b[1]);
    spi_xfer_byte(b[2]);
    spi_xfer_byte(b[3]);
#endif
  }
  void cmdbyte(byte x) {
    if (freespace == 0) {
      getfree(1);
    }
    wp++;
    freespace--;
    spi_xfer_byte(x);
  }
  void cmd_n(byte *s, uint16_t n) {
    if (freespace < n) {
      getfree(n);
    }
    wp += n;
    freespace -= n;
    while (n > 8) {
      n -= 8;
      spi_xfer_byte(*s++);
      spi_xfer_byte(*s++);
      spi_xfer_byte(*s++);
      spi_xfer_byte(*s++);
      spi_xfer_byte(*s++);
      spi_xfer_byte(*s++);
      spi_xfer_byte(*s++);
      spi_xfer_byte(*s++);
    }
    while (n--)
      spi_xfer_byte(*s++);
  }

  void flush() {
    YIELD();
    getfree(0);
  }
  uint16_t rp() {
    uint16_t r = __rd16(REG_CMD_READ);
    if (r == 0xfff) {
      GD.alert("COPROCESSOR EXCEPTION");
    }
    return r;
  }
  void finish() {
    wp &= 0xffc;
    __end();
    __wr16(REG_CMD_WRITE, wp);
    while (rp() != wp)
      YIELD();
    stream();
  }

  byte rd(uint32_t addr)
  {
    __end(); // stop streaming
    __start(addr);
    spi_xfer_byte(0);  // dummy
    byte r = spi_xfer_byte(0);
    stream();
    return r;
  }

  void wr(uint32_t addr, byte v)
  {
    __end(); // stop streaming
    __wstart(addr);
    spi_xfer_byte(v);
    stream();
  }

  uint16_t rd16(uint32_t addr)
  {
    uint16_t r = 0;
    __end(); // stop streaming
    __start(addr);
    spi_xfer_byte(0);
    r = spi_xfer_byte(0);
    r |= (spi_xfer_byte(0) << 8);
    stream();
    return r;
  }

  void wr16(uint32_t addr, uint32_t v)
  {
    __end(); // stop streaming
    __wstart(addr);
    spi_xfer_byte(v);
    spi_xfer_byte(v >> 8);
    stream();
  }

  uint32_t rd32(uint32_t addr)
  {
    __end(); // stop streaming
    __start(addr);
    spi_xfer_byte(0);
    union {
      uint32_t c;
      uint8_t b[4];
    };
    b[0] = spi_xfer_byte(0);
    b[1] = spi_xfer_byte(0);
    b[2] = spi_xfer_byte(0);
    b[3] = spi_xfer_byte(0);
    stream();
    return c;
  }
  void rd_n(byte *dst, uint32_t addr, uint16_t n)
  {
    __end(); // stop streaming
    __start(addr);
    spi_xfer_byte(0);
    while (n--)
      *dst++ = spi_xfer_byte(0);
    stream();
  }
#if defined(ARDUINO) && !defined(__DUE__) && !defined(ESP8266) && !defined(ARDUINO_ARCH_STM32L4)
  void wr_n(uint32_t addr, byte *src, uint16_t n)
  {
    __end(); // stop streaming
    __wstart(addr);
    while (n--) {
      SPDR = *src++;
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
    }
    while (!(SPSR & _BV(SPIF))) ;
    stream();
  }
#else
  void wr_n(uint32_t addr, byte *src, uint16_t n)
  {
    __end(); // stop streaming
    __wstart(addr);
#if defined(ESP8266)
    SPI.writeBytes(src, n);
#else
    while (n--)
      spi_xfer_byte(*src++);
#endif
    stream();
  }
#endif

  void wr32(uint32_t addr, unsigned long v)
  {
    __end(); // stop streaming
    __wstart(addr);
    spi_xfer_byte(v);
    spi_xfer_byte(v >> 8);
    spi_xfer_byte(v >> 16);
    spi_xfer_byte(v >> 24);
    stream();
  }

  uint32_t getwp(void) {
    return RAM_CMD + (wp & 0xffc);
  }

  void bulk(uint32_t addr) {
    __end(); // stop streaming
    __start(addr);
  }
  void resume(void) {
    stream();
  }

  static void __start(uint32_t addr) // start an SPI transaction to addr
  {
    gpio_set_pin_level(SPI_CS_N, false);
    spi_xfer_byte(addr >> 16);
    spi_xfer_byte(highByte(addr));
    spi_xfer_byte(lowByte(addr));  
  }

  static void __wstart(uint32_t addr) // start an SPI write transaction to addr
  {
    gpio_set_pin_level(SPI_CS_N, false);
    spi_xfer_byte(0x80 | (addr >> 16));
    spi_xfer_byte(highByte(addr));
    spi_xfer_byte(lowByte(addr));  
  }

  static void __end() // end the SPI transaction
  {
    gpio_set_pin_level(SPI_CS_N, true);
  }

  void stop() // end the SPI transaction
  {
    wp &= 0xffc;
    __end();
    __wr16(REG_CMD_WRITE, wp);
    // while (__rd16(REG_CMD_READ) != wp) ;
  }

  void stream(void) {
    __end();
    __wstart(RAM_CMD + (wp & 0xfff));
  }

  static unsigned int __rd16(uint32_t addr)
  {
    unsigned int r;

    __start(addr);
    spi_xfer_byte(0);  // dummy
    r = spi_xfer_byte(0);
    r |= (spi_xfer_byte(0) << 8);
    __end();
    return r;
  }

  static void __wr16(uint32_t addr, unsigned int v)
  {
    __wstart(addr);
    spi_xfer_byte(lowByte(v));
    spi_xfer_byte(highByte(v));
    __end();
  }

  static void hostcmd(byte a)
  {
    gpio_set_pin_level(SPI_CS_N, false);
    spi_xfer_byte(a);
    spi_xfer_byte(0x00);
    spi_xfer_byte(0x00);
    gpio_set_pin_level(SPI_CS_N, true);
  }

  void getfree(uint16_t n)
  {
    wp &= 0xfff;
    __end();
    __wr16(REG_CMD_WRITE, wp & 0xffc);
    do {
      uint16_t fullness = (wp - rp()) & 4095;
      freespace = (4096 - 4) - fullness;
    } while (freespace < n);
    stream();
  }

  byte streaming;
  uint16_t wp;
  uint16_t freespace;
};
