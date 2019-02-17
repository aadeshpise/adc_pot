#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
int main(void)
{
uint32_t ui32ADC0Value[4];//We will be using sequencer 1 which has a FIFO depth of 4
volatile uint32_t ui32TempAvg;//variable is for storing the average of the temperature
volatile uint32_t ui32TempValueC; //temperature values for Celsius
volatile uint32_t ui32TempValueF; //temperature values for Fahrenheit
//Set up the system clock again to run at 40MHz
SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
//Let’s enable the ADC0 peripheral
SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
//For this lab, we’ll simply allow the ADC12 to run at its default rate of 1Msps.
//Reprogramming the sampling rate is left as an exercise for the student
// Configure the ADC to sample at 500KSps
ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC|ADC_CLOCK_RATE_FULL,32);
//SysCtlADCSpeedSet(SYSCTL_SET0_ADCSPEED_500KSPS);
//ADC0, sample sequencer 1, we want the processor to trigger the sequence and we
//want to use the highest priority
ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
//Configure steps 0 - 2 on sequencer 1 to sample the temperature sensor (ADC_CTL_TS)
ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);
//The final sequencer step requires a couple of extra settings.
//Sample the temperature sensor (ADC_CTL_TS) and configure the interrupt flag
//(ADC_CTL_IE) to be set
//when the sample is done. Tell the ADC logic that this is the last conversion on
//sequencer 1 (ADC_CTL_END).
ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
//enable ADC sequencer 1
ADCSequenceEnable(ADC0_BASE, 1);
while(1)
{
ADCIntClear(ADC0_BASE, 1);
//trigger the ADC conversion with software
ADCProcessorTrigger(ADC0_BASE, 1);
//wait for the conversion to complete
while(!ADCIntStatus(ADC0_BASE, 1, false))
{
}
//When code execution exits the loop in the previous step, we know that the
//conversion is
//complete and that we can read the ADC value from the ADC Sample Sequencer 1 FIFO.
ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] +
ui32ADC0Value[3] + 2)/4;
//In datasheet refer page no. 813
ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;
ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;
}
}
