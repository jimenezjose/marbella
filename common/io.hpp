/*******************************************************************************
                                                    Jose Jorge Jimenez-Olivas

File Name:       io.hpp
Description:     I/O operations.
*******************************************************************************/
#ifndef IO_HPP
#define IO_HPP /* multiple inclusion guard */

#include <Arduino.h>

class IO {
  /**
   * Wrapper over the Stream library (e.g. HardwareSerial, SoftwareSerial) for
   * bundled USB and bluetooth UART communication. This class assumes the serial
   * ports have a set baud rate.
   */
private:
  HardwareSerial *serialUSB;
  long baudRate;
  bool silenceUSB;

public:
  IO(HardwareSerial *serialUSB) {
    this->serialUSB = serialUSB;
    this->silenceUSB = false;
    this->baudRate = 0;
  }

  void begin(long baudRate) {
    this->baudRate = baudRate;
    this->serialUSB->begin(baudRate);
  }

  void setSilenceUSB(bool silenceUSB) { this->silenceUSB = silenceUSB; }

  void print(double value) {
    if (!this->silenceUSB) {
      this->serialUSB->print(value);
    }
    // bluetoothSerial.print(value);
  }

  void println(double value) {
    if (!this->silenceUSB) {
      this->serialUSB->println(value);
    }
    // bluetoothSerial.println(value);
  }

  void print(char *msg) {
    if (!this->silenceUSB) {
      this->serialUSB->print(msg);
    }
    // bluetoothSerial.print(msg);
  }

  void println(char *msg) {
    if (!this->silenceUSB) {
      this->serialUSB->println(msg);
    }
    // bluetoothSerial.println(msg);
  }
};

#endif // IO_HPP