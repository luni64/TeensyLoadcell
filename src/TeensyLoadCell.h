#pragma once

#include "ADC.h"

namespace TeensyLoadcell
{
   enum Gain : uint8_t
   {
      gain_1 = 1,
      gain_2 = 2,
      gain_4 = 4,
      gain_8 = 8,
      gain_16 = 16,
      gain_32 = 32,
      gain_64 = 64
   };

   class Loadcell
   {
   public:
      // Constructor, tau is the smoothing time constant in seconds.
      Loadcell(ADC_Module *adc, float tau = 1.0, Gain gain = gain_64, unsigned sampleFreq = 5000u);
      ~Loadcell();

      /* Starts continous background conversion.*/
      void start();

      /*Stops background conversions*/
      void stop();

      /* Gets the averaged, calibrated value. Calibration coefficients can be set by setCalib(c0, c1) */
      inline float getValue() { return c0 + c1 * average; }

      // set the smoothing time constant in seconds. Values will be stable after about 5* tau.
      void setTau(float tau);

      // set the calibration factors. getValue() returns c0 + c1 * average. Defaults to c0 = 0; c1 = 1
      void setCalib(float c0, float c1);

      // delete copy constructor
      Loadcell(const Loadcell &) = delete;

   protected:
      ADC_Module *adc;
      volatile float average = 0.0f;   // current averaged measurement value
      uint8_t pinP, pinN;              // used input pin pair

      float alpha;                     // smoothing factor
      unsigned sampleFreq;             // used sample frequency for the PDB
      float c0 = 0.0f, c1 = 1.0f;      // calibration coefficients

      template<unsigned n> friend void Loadcell_ISR(); // let the ADC ISR update the measurement value (average)
   };

} // namespace TeensyLoadcell