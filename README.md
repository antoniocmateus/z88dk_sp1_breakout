# z88dk_breakout

This is an example skeleton for a simple breakout game done with z88dk + sp1 for the zx spectrum.
The are lots of space for improvements and corrections. You can freely use the code or ideas as you wish. I have commented most of it, you should be able to follow it easily.
If it helps you in any way, I'll be happy.

# To build this for Windows

Just run make.bat and the result should be in the build directory.

# To build this using Docker

1. First of all you must have GNU make installed. 
2. Secondly you must have docker and docker-compose installed.

Simply run `make` or `make z88dk-breakout` to build the game. The resulting `.tap` file will be in the `build` directory and a .map in the `maps` directory. This build uses the SDCC compiler as opposed to the SCCZ80.

[Z88DK Docker Usage Reference Material](https://github.com/z88dk/z88dk/wiki/Docker-Usage)

# References
Pacman
Wonky
Blackstar (pad_numbers() method)
z88dk forums
World of Spectrum forums

# Kudos
To everyone who helped me in z88dk forums, thank you :)
