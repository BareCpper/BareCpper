# BareCpper
C++ Bare-metal GPIO library framework for PC, Arduino, Nordic etc

If you've reached the limit of Arduino or want a portable GPIO platform for low level hardware io the BareCpper aims to provide what you need. 

# FAQ
## How does is BareCpper more optimal for Bare-metal io than other libraries? 
The software makes heavy use of C++ templates and compile-time optimisation. This means unlike libraries such as Arduino unnecessary runtime logic is always avoided in release-builds. 
For example a ping mapping in BareCpper is a compile-time typedef instead of a runtime variable. 

## What's with the name?
Broken down it's Bare-CPP-er or Bare-C'o'pper which both elate to the low level C++ intent. Copper being prominiently used for electronic circuits at this time. 

## How is BareCpper easy to migrate to? 
BareCpper can be used inside you Arduino project as a library. You can therefore transition you code to BareCpper bit-by-bit... 
