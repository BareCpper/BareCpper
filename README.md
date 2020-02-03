# BareCpper
C++ Bare-metal GPIO library framework for PC, Arduino, Nordic etc

If you've reached the limit of Arduino or want a portable GPIO platform for low level hardware io the BareCpper aims to provide what you need. 

# FAQ
## How can BareCpper be so optimal? 
We plan to provide timing examples soon. The software is oftimal by making clever use of C++ templates to privide compile-time optimisation. This means unnecessary runtime logic is significantly reduced in release-builds compared to the Arduino libraries. 
For example a pin mapping in BareCpper is a compile-time typedef/using instead of a runtime variable. 

## What's with the name?
Broken down it's Bare-CPP-er or Bare-C'o'pper which both elate to the low level C++ intent. Copper being prominiently used for electronic circuits at this time and Cpp being a common acrony for C-Plus-Plus language of course. 

## How can I easily migrate to BareCpper? 
BareCpper can be used inside you existing Arduino projects. You can therefore transition you code to BareCpper incrementally. When you've done so you can port your code to other build systems for serous development. 

# Building
BareCpper shall be mostly header-only but there is  lot of refactoring/rework going on at present so will require some expereicne in C/C++ to extract useable features.

## CMake
CMake is being implemented for CI+CD but is in development. To test locally please execute the following to conigure and build successfully before commiting:
```
mkdir -p ./build
cmake -B ./build
cmake --build ./build 
```