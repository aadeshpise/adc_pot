project is ment for texas tiva ware c series 
this is related to set adc clock frequency to 500kbs using inbulit api from adc.h
we will sample at 500kbps

ADCClockConfigSet
sets the clock configuration for adc
Prototype:
void 
ADCClockConfigSet(uint32_t ui32Base,uint32_t ui32Config,uint32_t)

Parameters:
ui32Base is the base address of the ADC to configure, which must always be ADC0_BASE.
ui32Config is a combination of the ADC_CLOCK_SRC_ and ADC_CLOCK_RATE_∗ values
used to configure the ADC clock input.
ui32ClockDiv is the input clock divider for the clock selected by the ADC_CLOCK_SRC value.


For TM4C123x devices, if the PLL is enabled, the PLL/25 is used as the ADC clock unless
ADC_CLOCK_SRC_PIOSC is specified. If the PLL is disabled, the MOSC is used as the clock
source unless ADC_CLOCK_SRC_PIOSC is specified.
ADC_CLOCK_SRC_PLL - The main PLL output (TM4x129 class only).
ADC_CLOCK_SRC_PIOSC - The internal PIOSC at 16 MHz.
ADC_CLOCK_SRC_ALTCLK - The output of the ALTCLK in the system control module
(TM4x129 class only).
ADC_CLOCK_SRC_MOSC - The external MOSC (TM4x129 class only).

we are using 
***full clock rate ie every samples and dividing internal oscillator fequency 16M by 32 we get 
500kbps***
function:  
    **ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC|ADC_CLOCK_RATE_FULL,32);**
