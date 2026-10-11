# Problem Log

| Date | Problem | How I found it | Fix |
|---|---|---|---|
| 2026-10-10 | `main.c` replaced with snippets and placeholder `{ ... }` bodies, so it wouldn't build | Compared against the generated file; no `HAL_Init`, no handles | Deleted `main.c`, regenerated from the `.ioc`, pasted code only between `USER CODE BEGIN/END` markers |
| 2026-10-10 | `hi2c1` undeclared when building the I2C scan | Build error; no `MX_I2C1_Init()` in `main.c` | Enabled I2C1 in the `.ioc` and regenerated |
| 2026-10-10 | Couldn't find PB8/PB9 as I2C pins | Opened the pin menu for PC8 by mistake | Searched for PB8/PB9 in the pinout view and assigned I2C1_SCL/SDA |
| 2026-10-11 | ADC readings were random with no sensor connected | Values jumped around with nothing on PA0 | Expected behavior: floating input. Readings became stable once the module was wired |
| 2026-10-11 | LDR module reads higher when darker | Covered and lit the sensor and compared raw values | Inverted in software: `brightness = 4095 - raw` |
| 2026-10-11| DHT22 read block was placed after the while(1) loop and never ran. No output at all, not even a timeout message, was the clue. Moved it inside the loop.|

## Lessons
- Only edit code inside `USER CODE` markers, or regeneration erases it.
- Check one peripheral at a time: wire it, confirm it works, then move on.