# Light Sensor (LDR module, AO to ADC)

## Setup
- Module: 4-pin LDR module (VCC, GND, DO, AO), powered from 3.3 V
- AO connected to PA0 (ADC1_IN0), 12-bit, software-triggered
- DO unused

## Behavior
- Output is **inverted**: higher raw value means darker
- Handled in software: `brightness = 4095 - raw`

## Measured raw values
| Condition | Raw reading |
|---|---|
| Fully covered = 4080 | |
| Dim room = 3100 | |
| Normal room light = 1800 | |
| Phone flashlight close = 35 | |

## Notes
- Measures relative brightness only, not lux
- ADC sampling time: 3 cycles (default). Noise: (describe what you see)