#include "TeensyLoadcell.h"
#include "ADC.h"

#if ADC_USE_PGA == 0
#error "Board has no PGA"
#endif

namespace TeensyLoadcell
{
   static Loadcell *activeLCO[2]; // will point to current Loadcell objects, the pointers are required by the ISR to access the object

   // ISR,  Read values and calculate exponential average.
   template<unsigned n>
   void Loadcell_ISR()
   {
      Loadcell *l = activeLCO[n];
      float curVal = l->adc->readSingle();
      l->average = l->average + l->alpha * (curVal - l->average); //https://en.wikipedia.org/wiki/Exponential_smoothing
   }

   //----------------------------------------------------------------------------------

   Loadcell::Loadcell(ADC_Module *adc, float tau, Gain gain, unsigned sampleFreq)
   {
      this->adc = adc;
      this->sampleFreq = sampleFreq;

      pinP = adc->ADC_num == 0 ? A10 : A12;
      pinN = adc->ADC_num == 0 ? A11 : A13;

      pinMode(pinP, INPUT);
      pinMode(pinN, INPUT);

      adc->setAveraging(32);
      adc->setResolution(13);
      adc->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
      adc->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
      adc->setReference(ADC_REFERENCE::REF_1V2);
      adc->enablePGA(gain);

      c0 = 0.0f;
      c1 = 1.0f;

      setTau(tau);
   }

   //-----------------------------------------------------------------

   Loadcell::~Loadcell()
   {
      stop();
   }

   //-----------------------------------------------------------------

   void Loadcell::start()
   {
      average = 0.0f;
      adc->startSingleDifferential(pinP, pinN);

      // attach correct ISR depending on ADC module number
      if (adc->ADC_num == 0)
      {
         activeLCO[0] = this;
         attachInterruptVector(IRQ_ADC0, Loadcell_ISR<0>);
      }
      else
      {
         activeLCO[1] = this;
         attachInterruptVector(IRQ_ADC1, Loadcell_ISR<1>);
      }

      adc->enableInterrupts();
      adc->startPDB(sampleFreq);
   }

   //-----------------------------------------------------------------

   void Loadcell::stop()
   {
      adc->stopPDB();
      adc->disableInterrupts();
      activeLCO[adc->ADC_num] = nullptr;
   }

   //-----------------------------------------------------------------

   void Loadcell::setTau(float tau)
   {
      if (tau <= 0.0f)
      {
         alpha = 1.0f;
      }
      else
      {
         alpha = 1 - expf(-1.0f / (tau * sampleFreq)); //https://en.wikipedia.org/wiki/Exponential_smoothing
      }
   }

   //-----------------------------------------------------------------

   void Loadcell::setCalib(float c0, float c1)
   {
      this->c0 = c0;
      this->c1 = c1;
   }

} // namespace TeensyLoadcell
