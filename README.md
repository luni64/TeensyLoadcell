# TeensyLoadcell
Direct load cell readout with a [Teensy
3.2](https://www.pjrc.com/teensy/teensy31.html]) Arduino controller board.

## Purpose of the Library
The MK20DX256 processor of the Teensy 3.2 includes an on chip differential ADC
and a 1-64 PGA (programmable gain amplifier). This is pretty much all you need
to directly connect a standard load cell without any other hardware and read it
out. The image below shows the required wiring for the differential input pair
A10/A11.

![Overview](/media/overview.png?raw=true)

The drawn cable colors match the colors of most of the available, cheap load
cells. The two capacitors C1 and C2 are not needed but help to reduce the readout noise.

And here a small test setup:

![Setup](/media/setup.png?raw=true)


Basically TeensyLoadcell is just a shallow wrapper around Pedro Villanueva's ADC library
([https://github.com/pedvide/ADC](https://github.com/pedvide/ADC)).
It sets up the ADC and PGA for differential readout and continuously
reads out the load cell in the background using the PDB timer. For noise suppression TeensyLoadcell
applies a simple exponential smoothing algorithm with adjustable time constant.

## Usage
The following code shows a minimal usage example. It sets up the load cell
with default settings, starts the readout and prints averaged values every 100ms.

```c++
#include "TeensyLoadcell.h"
using namespace TeensyLoadcell;

Loadcell loadcell;

void setup(){
   loadcell.start();
}

void loop(){
   Serial.println(loadcell.getValue());
   delay(100);
}
```


TBC



