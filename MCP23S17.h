// ----------------------------------------------------------------------------
// MAX6956.h
//
// Provides an I2C based interface to the MAX6956 LED driver
//   and port expander
//
// Author: Steve Sawtelle
// ----------------------------------------------------------------------------

// ---- VERSIONS  ----
// 20150429 sws
// - remove atomic stuff to be compatible with MAX32, use Interruopts


#ifndef MCP23S17_H_
#define MCP23S17_H_
//#include <util/atomic.h>
#include "BCSIII.h"


#define LED_DRIVER  0x00
#define GPIO_OUT    0x01
#define GPIO_IN     0x02
#define GPIO_IN_PU  0x03

#define LED1 16
#define LED2 17
#define LED3 18
#define LED4 19
#define LED5 20
#define LED6 21
#define LED7 22

#define BB1 24
#define BB2 25
#define BB3 26
#define BB4 27
#define BB5 28
#define BB6 29
#define BB7 30

#define BB_ADR  0
#define DIO_ADR 2  // offset one bit higher to OR in with hardware address and R/W
#define AIO_ADR 4
#define MCP23S17_ADR 0x40
#define MCP23S17_RD  0x41
#define MCP23S17_WR  0x40
#define BBPORTA 0
#define BBPORTB 1

#define IOCON_VAL  0x78  // 0111 1000

#define IODIRA      0x00
#define IODIRB      0x01
#define IPOLA       0x02
#define IPOLB       0x03
#define GPINTENA    0x04
#define GPINTENB    0x05
#define DEFVALA     0x06
#define DEFVALB     0x07
#define INTCONA     0x08
#define INTCONB     0x09
#define IOCON       0x0A
//#define IOCON       0x0B
#define GPPUA       0x0C
#define GPPUB       0x0D
#define INTFB       0x0E
#define INTFA       0x0F
#define INTCAPA     0x10
#define INTCAPB     0x11
#define GPIOA       0x12
#define GPIOB       0x13
#define OLATA       0x14
#define OLATB       0x15

#define BBINT 31

class MCP23S17Class
{
 public:

  void begin(int);
  void setInterruptPins(uint8_t port, uint8_t pins);
  uint8_t readInterruptBits(uint8_t port);
  void setIO(uint8_t port,  uint8_t pin, uint8_t io);
  void setPin(uint8_t port, uint8_t pin);
  void clrPin(uint8_t port, uint8_t pin);
  uint8_t readPort(uint8_t port );
  uint8_t readPin(uint8_t port, uint8_t pin);
  uint8_t readReg( uint8_t adr, uint8_t reg);
  void writeReg(uint8_t adr, uint8_t reg, uint8_t data );

 private:

  int csPin;



};

extern MCP23S17Class MCP23S17;

#endif //  MCP23S17



