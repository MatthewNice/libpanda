# Interface library for comm.ai's USB panda.

A library that handle communication with a comma.ai Panda interface device.  Simple demos collect data from the panda (grey version) to the screen.

## Dependencies:
1. Required - libusb-1.0-0-dev
2.  Optional - libcurses5-dev
3. Included - NMEAParser

### libusb-1.0-0-dev
The core of this library is based on libusb, the default version installable through Ubuntu's apt-get.

### libcurses5-dev
An optional exmaple for data visualization is based on ncurses.  If desired to run this example, libcurses needs tobe installed but is not required

### NMEAParser
The NMEAParser is already included as a modified version with some bug fixes and added features.
1. Bugs fixed:
* Incomplete time precision for seconds in RMC strings
* Year calculation incorrect byte offset resulting in incorrect year parsings
2. Additional eatures:
* ZDA NMEA message parsing

## Setup:
These instructions were successful on a Raspberry pi 4 running Ubuntu 18.04
### install libusb-1.0-0-dev
`$ sudo apt-get install libusb-1.0-0-dev`

### Optional: installlibsurses5-dev
`$ sudo apt-get install libncurses5-dev`

### Compile
`$ mkdir build`
`$ cd build`
`$ cmake ..`
`$ make`

## Examples
### panda
Barebones example showing a minimal implementation
`$ sudo ./panda`

### pandaSetTime
This example listens to the GPS and when data is valid, synchronizes the system clock to the GPS's UTC if the difference is larger than a defined delta.
`$ sudo ./pandaSetTime`

### pandaCurses
An example that currently plots GPS data to the console in a pretty fashion.  This also takes arguments for various data collection and tuning.
`$ sudo ./pandaCurses`

Save NMEA strings to a file anmed nmeaFile.txt:
`$ sudo ./pandaCurses -g nmeaFile.txt`

Run with USB in synchronous mode (default is asynchronous):
`$ sudo ./pandaCurses -u s`

## Setup:
The lobrary is self-documenting using Doxygen, producing both HTML and LaTex.  Generate the documentation as follows:
`$ doxygen doxygen.conf`

If you do not have doxyen, you may install using, on Ubuntu:
`$ sudo apt-get install doxygen`
On macOS with amcport:
`$ sud port install doxygen`

## Todo
- [x] Panda GPS interface
- [ ] Isochronous USB transfer
- [x] CAN frame parsing
- [ ] CAN frame handling
- [ ] ROS based publishing
- [ ] Testing