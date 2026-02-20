# How to configure Over the Air (OTA) uploads using bluetooth

Wirelessly upload sketeches to your Arduino with an HC-05 bluetooth module.

1. Upload the arduino_serial_bridge.ino program into your arduino.
2. Connect the HC-05 module to the Arduino Uno.
<wire diagram>
3. Set your HC-05 module in AT mode by setting the EN pin HIGH and holding the physical button on the module during a power cycle. The light should be flashing slowly (2 second intervals).
4. Configure the HC-05 module to trigger the bootloader for an upload with the following commands:

AT Commands:
```
AT+ORGL (Reset to default)

AT+ROLE=0 (Set as Slave)

AT+POLAR=1,0 (This ensures the STATE pin drops LOW when the connection is made, triggering the reset).

AT+UART=115200,0,0 (The Uno's upload speed is 115200. For a Pro Mini, use 57600).
```