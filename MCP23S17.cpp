//----------------------------------------------------------------------
// MCP23S17.cpp
//
// Provides an SPI based interface to the MCP23S17 port expander
//
// Author: Steve Sawtelle
// ----------------------------------------------------------------------------

// ---- VERSIONS  ----
// 20150429 sws
// - remove atomic stuff to be compatible with MAX32, use Interrupts



#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <SPI.h>
#include "CS.h"
#include "MCP23S17.h"



//---------- constructor ----------------------------------------------------

MCP23S17Class MCP23S17;


//---------- public ----------------------------------------------------

void MCP23S17Class::begin(int cs)
{
    csPin = cs;

    writeReg(BB_ADR, IOCON, IOCON_VAL);  // set up banks, ints, etc
    writeReg(DIO_ADR, IOCON, IOCON_VAL);
    writeReg(AIO_ADR, IOCON, IOCON_VAL);

    writeReg(BB_ADR, GPPUA, 0xFF);  // use pull ups on BB inputs
    writeReg(BB_ADR, GPPUB, 0xFF);  //
    writeReg(BB_ADR, GPIOA, 0x55); // set low initially
    writeReg(BB_ADR, GPIOB, 0x00); // set low initially
    writeReg(DIO_ADR, GPIOA, 0x00); // init output states as low
    writeReg(DIO_ADR, GPIOB, 0x00); // (not used but set it to known value)
    writeReg(AIO_ADR, GPIOA, 0x00); //  these will set 1T45s as inputs
    writeReg(AIO_ADR, GPIOB, 0x00); //   and disable digital lines on analog channels

    writeReg(BB_ADR, IODIRA, 0xFF); // init BB IO as inputs to start off
    writeReg(BB_ADR, IODIRB, 0xFF); //
    writeReg(DIO_ADR, IODIRA, 0x00); // set the 1T45 control porst as outputs
    writeReg(DIO_ADR, IODIRB, 0x00); // not used but set it to known value
    writeReg(AIO_ADR, IODIRA, 0x00); // dir control for digital
    writeReg(AIO_ADR, IODIRB, 0x00); // digital or analog

    // for now we only use interrupt on falling low
     writeReg(BB_ADR, GPINTENA, 0x00); // no pins enabled
     writeReg(BB_ADR, GPINTENB, 0x00);
     writeReg(BB_ADR, DEFVALA, 0xFF);  // default will be high
     writeReg(BB_ADR, INTCONA, 0xFF);  // compare input for change from default register
     writeReg(BB_ADR, DEFVALB, 0xFF);  // default will be high
     writeReg(BB_ADR, INTCONB, 0xFF);  // compare input for change from default register
}

// -------------------------------------------
// --- S E T   I N T E R R U P T   P I N S ---
// -------------------------------------------
// - set the chosen pins (pins) as interrupts on the specified port
// - this only for the beam break interface

void MCP23S17Class::setInterruptPins(uint8_t port, uint8_t pins)
{
    writeReg(BB_ADR, GPINTENA + port, pins);  // any pin bit = 1 enabled for int
}

// ---------------------------------------------
// --- R E A D   I N T E R R U P T   P I N S ---
// ---------------------------------------------
// - read the interrupt pins on the specified port
// - this only for the beam break interface

uint8_t MCP23S17Class::readInterruptBits(uint8_t port)
{
    return(readReg(BB_ADR, INTFA + port) );
}


void MCP23S17Class::setIO( uint8_t port, uint8_t pin, uint8_t io)
{

uint8_t pinvals;

    pinvals = readReg(BB_ADR, IODIRA + port);
    pinvals &= ~(1 << pin);   // reset pin
    pinvals |= io << pin;  // new direction
    writeReg(BB_ADR, IODIRA + port, pinvals);

}

void MCP23S17Class::setPin(uint8_t port, uint8_t pin)
{
uint8_t pinvals;

    pinvals = readReg(BB_ADR, GPIOA + port);
    pinvals != 1 << pin;  // set pin
    writeReg(BB_ADR, GPIOA + port, pinvals);
}

void MCP23S17Class::clrPin(uint8_t port, uint8_t pin)
{
 uint8_t pinvals;

     pinvals = readReg(BB_ADR, GPIOA + port);
     pinvals &=  ~(1 << pin);  //clear pin
     writeReg(BB_ADR, GPIOA + port, pinvals);
}

uint8_t MCP23S17Class::readPin(uint8_t port, uint8_t pin)
{
uint8_t pinvals;

     pinvals = readReg(BB_ADR, GPIOA + port);
     pinvals &= 1 << pin;
     return pinvals;

}

uint8_t MCP23S17Class::readPort(uint8_t port)
{
    return(readReg(BB_ADR, GPIOA + port) );
}


//---------- private ------------

uint8_t MCP23S17Class::readReg( uint8_t adr, uint8_t reg)
{
 uint8_t data;

    noInterrupts(); //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      SPI.setDataMode(SPI_MODE0);  // set proper mode, clk idle high, rising edge
      CS.SPIselect(csPin);
      SPI.transfer(MCP23S17_RD | adr);
      SPI.transfer(reg);
      data = SPI.transfer(0);
      CS.SPIselect(NO_CS);
    } // end int off
    interrupts();
}


void MCP23S17Class::writeReg(uint8_t adr, uint8_t reg, uint8_t data )
{
      noInterrupts();  // ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      SPI.setDataMode(SPI_MODE0);  // set proper mode, clk idle high, rising edge
      CS.SPIselect(csPin);
      SPI.transfer(MCP23S17_WR | adr);
      SPI.transfer(reg);
      SPI.transfer(data);
      CS.SPIselect(NO_CS);
    } // end int off
    interrupts();
}





