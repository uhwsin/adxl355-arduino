#
There are some critical errors in the arduino code.
Fixed them and now work good.
2020-12-03
# ADXL355 on Arduino

Code to read ADXL355 PMDZ accelerometer on Arduino using SPI protocol.

# How does it work?

## Requirements

* Arduino 
  * Tested on MKR1000
  * Tested on UNO, but it is not recommend. It will damaged to your ADXL355. ADXL355's GPIO is  3.3V , but UNO's is 5V. You can add a TTL-CMOS level convertor.
* ADXL355 PMDZ
  * More info: [here](https://wiki.analog.com/resources/eval/user-guides/eval-adicup360/hardware/adxl355)

## Wiring ADXL355 to Arduino MKR1000

| ADXL355 Pin Numer  | ADXL Pin description       | Arduino Pin   |
| -----------------: |----------------------------|---------------|
| 1                  | Chip Select                | 07            |
| 2                  | Master Out Slave In (MOSI) | 08            |
| 3                  | Master In Slave Out (MISO) | 10            |
| 4                  | Serial Clock (SCLK)        | 09            |
| 5                  | Digital Ground             | GND           |
| 6                  | Digital Power              | VCC           |
| 7                  | Interrupt 1                | Not Connected |
| 8                  | Not Connected              | Not Connected |
| 9                  | Interrupt 2                | Not Connected |
| 10                 | Data Ready                 | Not Connected |
| 11                 | Digital Ground             | Not Connected |
| 12                 | Digital Power              | Not Connected |
