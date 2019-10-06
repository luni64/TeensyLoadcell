# TeensyLoadcell
Direct load cell readout with a
[Teensy3.2](https://www.pjrc.com/teensy/teensy31.html)
Arduino controller board.

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

### Minimal Example
The following code shows a minimal usage example. It sets up the load cell
with default settings, starts the readout and prints averaged values every 100ms.

```c++
#include "ADC.h"
#include "TeensyLoadcell.h"
using namespace TeensyLoadcell;

ADC adc;
Loadcell loadcell(adc.adc0);

void setup(){
   loadcell.start();
}

void loop(){
   Serial.println(loadcell.getValue());
   delay(100);
}
```
### Simple Balance

This example shows how to implement a simple balance with TeensyLoadcell. It is
not super precise but works nicely.

Here a quick video showing the balance in action: 

[![Watch the video](https://img.youtube.com/vi/ls3YLmw5UEo/0.jpg)](https://youtu.be/ls3YLmw5UEo)

```
#include "Arduino.h"
#include "TeensyLoadCell.h"

using namespace TeensyLoadcell;

ADC adc;
Loadcell loadcell(adc.adc0);

void setup()
{
  while (!Serial);
  Serial.println("Enter s to start/stop, t to tare");

  loadcell.setTau(0.2);  // Changes will require about 5tau = 1s to stabilize
  loadcell.c1 = 5.4f;    // Manually calibrated scale factor, c0 will be set by zeroing later

  loadcell.start();
}

bool isRunning = false;
elapsedMillis stopwatch = 0;

void loop()
{
  int c = Serial.read();
  switch (c)
  {
    case 't': // zero the balance
      loadcell.tare();
      break;

    case 's': // start / stop display
      isRunning = !isRunning;
      break;
  }

  if (stopwatch > 250 && isRunning) // display value each 250ms
  {
    Serial.printf("%5.0f\n", loadcell.getValue());
    stopwatch = 0;
  }
}
```





