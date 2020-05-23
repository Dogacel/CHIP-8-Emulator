# CHIP-8 Emulator written in C++
This is an emulator for CHIP-8 Interpreted programming language used in DIY computer systems in late 1970s. It is controlled by a 4x4 Keypad, it contains 4K RAM and has a 64x32 display resolution.

### Notes:
- CPU Frequency is 500Hz but DelayTimer should be running on 60Hz. That's why instructions are scaled 8 times to match (500 / 8 = 62.5Hz) roughly to the frequency of the delay timer.

`Compiled with SDL2 library and gcc 9.3.0`

## Resources
- [Cowgod's CHIP-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [Laurence Muller - How to write an emulator (CHIP-8 interpreter)](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
- [Bestcoder CHIP-8 Test ROM](https://slack-files.com/T3CH37TNX-F3RKEUKL4-b05ab4930d) - [ROM LINK](https://slack-files.com/T3CH37TNX-F3RF5KT43-0fb93dbd1f)
- [Github/corax89 Test ROM](https://github.com/corax89/chip8-test-rom)
