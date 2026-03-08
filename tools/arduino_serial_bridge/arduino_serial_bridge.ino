// Configure and send AT commands to a bluetooth module.

// =============================================================================
//  BLUETOOTH MODULE AT COMMAND REFERENCE
// =============================================================================
//  DEVICE   |  MODE              | LINE ENDING (Serial Monitor)
//  ---------|--------------------|----------------------------------
//  HC-06    |  Always (Unpaired) | No Line Ending
//  HC-05    |  Button + Power On | Both NL & CR (\r\n)
// -----------------------------------------------------------------------------
//
//  [!] HC-05 PHYSICAL TRIGGER (TO ENABLE AT MODE):
//  1. Disconnect VCC (Power).
//  2. Set the EN pin to HIGH.
//  2. Press and HOLD the physical button on the module.
//  3. Reconnect VCC while holding the button.
//  4. LED should blink SLOWLY (once every 2 seconds). Mode is now active.
//
//  COMMON COMMANDS:
//  [ COMMAND ]      [ HC-06 RESPONSE ]    [ HC-05 SYNTAX ]      [ DESCRIPTION ]
//  AT               OK                    AT                    Test Connection
//  AT+VERSION       OK+VERSION...         AT+VERSION?           Get Firmware
//  AT+NAME<Name>    OKsetname             AT+NAME=<Name>        Set Device Name
//  AT+PIN<1234>     OKsetPIN              AT+PSWD=<1234>        Set Pair Code
//  AT+BAUD<1-C>     OK<Rate>              AT+UART=<R,S,P>       Set Baud Rate
//
// -----------------------------------------------------------------------------
//  HC-06 BAUD INDEX (AT+BAUD<X>):
//  1: 1200    | 2: 2400    | 3: 4800    | 4: 9600 (Default)
//  5: 19200   | 6: 38400   | 7: 57600   | 8: 115200
//
//  HC-05 UART CONFIG (AT+UART=<Baud>,<Stop>,<Parity>):
//  Example: AT+UART=115200,0,0
//
// -----------------------------------------------------------------------------
//  MacOS:
//  To reset the bluetooth: $ sudo pkill bluetoothd
// =============================================================================

#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial(10, 11); // rx, tx


void setup() {
  Serial.begin(9600);
  bluetoothBaudRateSweep();

  Serial.println("Ready to receive AT commands.");
}

void loop() {
  // Send messages to the bluetooth module.
  if( Serial.available() ) {
    bluetoothSerial.write(Serial.read());
  }

  // Print the data received by the bluetooth module.
  if( bluetoothSerial.available() ) {
    Serial.write(bluetoothSerial.read());
  }

}

void bluetoothBaudRateSweep() {
  Serial.println("Starting Baud Sweep...");
  long bauds[] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
  for (int i = 0; i < sizeof(bauds) / sizeof(long); i++) {
    Serial.print("Testing Baud: ");
    Serial.println(bauds[i]);
    bluetoothSerial.begin(bauds[i]);

    // ---------- HC-06 ----------
    delay(100);
    bluetoothSerial.print("AT"); // Send AT to the HC-06 module (no ending characters)
    delay(500);
    
    if (bluetoothSerial.available()) {
      Serial.println("Device Detected: HC-06");
      Serial.print("Response found at ");
      Serial.print(bauds[i]);
      Serial.print(": ");
      while(bluetoothSerial.available()) Serial.write(bluetoothSerial.read());
      Serial.println();
      break; 
    }

    // ---------- HC-05 ----------
    delay(100);
    bluetoothSerial.println("AT"); // Send AT to the HC-05 module (with '\r\n' ending characters)
    delay(500);
    if (bluetoothSerial.available()) {
      Serial.println("Device Detected: HC-05");
      Serial.print("Response found at ");
      Serial.print(bauds[i]);
      Serial.print(": ");
      while(bluetoothSerial.available()) Serial.write(bluetoothSerial.read());
      Serial.println();
      break; 
    }
  }
}
