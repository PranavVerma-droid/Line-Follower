# Private Line Follower Details.

## Repository Links
- [STM32](Code/STM32)
- [Arduino](Code/ARDUINO)
  
## Total Parts Needed:
Ignore the Parts list in the Enginnering Drawings.
- [DONE] QTR-8RC Sensor Array
- [DONE] Arduino Nano 
- [DONE] Lipo Charger
- (ATL) L298N
- (ATL) Vero Board
- STM-32 Blue Pill x 2
- 11.1v Lipo Battery
- TB6612FNG
- Castor Wheel x 2
- N20 12V 600 RPM x 4
- N20 Wheels x 4
- Male & Female Berg Strips
- Jumper Wires
- USB to Serial Converter (CH340G)
- Mini Breadboard (XF-25) x 2
- XT60 Male Connectors x 2

### Other Parts:
- High Speed Micro SD Card Reader
- 650nm 6mm 5V DC 5mW Mini Laser Dot Diode Module x 2
- MPU6050 - Triple Axis Gyro Accelerometer Module
  
## Description
Create Two Modular PCB's using Vero Boards, to make use of Limited Parts. My Idea is that the Motor Driver and the QTR can be Swapped from both the boards, using Female and Male Berg Strips. Rest of the Components will also be using both berg strips, incase of any component getting damaged, and needing replacement.



### PCB 1 [(STM32)](Code/STM32)

#### Permanent Parts:
- STM32 Blue Pill
- N20 12V 600 RPM Motors x 2
- N20 Wheels x 2
- XT60 Male Connector
- L298N Motor Driver
- Castor Wheel

#### Swapable Parts:
- Lipo Battery
- QTR-8RC Sensor

### PCB 2 [(Arduino)](Code/ARDUINO)

#### Permanent Parts:
- Aruino Nano / Arduino Pro Mini
- N20 12V 600 RPM Motors x 2
- N20 Wheels x 2
- XT60 Male Connector
- TB6612FNG Motor Driver
- Castor Wheel
  
#### Swapable Parts:
- Lipo Battery
- QTR-8RC Sensor

