# DHT22 Driver

## Protocol
- Single wire on PA1 (open-drain output, module has a built-in pull-up)
- MCU pulls the line low for more than 1 ms (I use 2 ms), then releases it
- Sensor replies: ~80 us low, ~80 us high, then 40 bits
- Each bit: ~50 us low, then a high of ~27 us (0) or ~70 us (1)
- 40 bits = 16 humidity + 16 temperature + 8 checksum
- Temperature top bit is the sign; values are in tenths

## Timing
- TIM2 at 84 MHz with prescaler 83 gives a 1 MHz counter (1 us per tick)
- Interrupts are disabled during the read (a few ms)

## Verification
No logic analyzer was available, so I logged the measured high-pulse widths over UART.

Sample (us): 23 23 22 23 23 23 23 65 64 65 23 23 23 65 65 65 23 23 23 23 23 22 23 65 23 23 22 23 65 23 23 22 64 64 22 65 23 22 22 64


## Problems
- Read block was placed after the while(1) loop and never ran