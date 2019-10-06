#include "Arduino.h"
#include "TeensyLoadCell.h"

using namespace TeensyLoadcell;

ADC adc;
Loadcell loadcell(adc.adc0);

void setup()
{
  while (!Serial);

  Serial.println("Enter s to start/stop, t to tare");

  loadcell.setTau(0.2);
  loadcell.c1 = 5.4f;

  loadcell.start();
}

bool isRunning = false;
elapsedMillis stopwatch = 0;

void loop()
{
  int c = Serial.read();
  switch (c)
  {
    case 't':
      loadcell.tare();
      break;

    case 's':
      isRunning = !isRunning;
      break;
  }

  if (stopwatch > 250 && isRunning)
  {
    Serial.printf("%5.0f\n", loadcell.getValue());
    stopwatch = 0;
  }
}
