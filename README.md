# Snowball Bowling Autoplay

inspired by [bertrandom/snowball-thrower](https://github.com/bertrandom/snowball-thrower)

## Improvements
* 64 <= PollingIntervalMS <= 255 eliminates the need of ECHOES
* refactored using LUFA's HID Class Driver
* the input sequence can deal with failed cases (non-strike)

## Test Environment
* Nintendo Switch (System Version: 4.1.0)
* The Legend of Zelda: Breath of the Wild (Ver. 1.3.4)
* Teensy++ 2.0
