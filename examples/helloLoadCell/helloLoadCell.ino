#include <TeensyLoadcell.h>

using namespace TeensyLoadcell;

ADC adc;
Loadcell loadCell(adc.adc0);  // use the ADC0 module (input pin pair A10/A11)
//Loadcell loadCell(adc.adc1);  // use the ADC1 module (input pin pair A12/A13)

void setup()
{
   loadCell.start();
}

void loop()
{
   Serial.println(loadCell.getValue());
   delay(5);
}
