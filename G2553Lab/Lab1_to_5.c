

//***********************************************************
// Lab3f.c  Optimized code using 1.1MHz DCO/8 as MCLK and VLO as ACLK
//
// Green LED on LaunchPad should toggle about 30 times per second
//
// SFB 1/2012
//***********************************************************

#include <msp430g2553.h>
#include "timers.h"

#ifndef TIMER0_A1_VECTOR
#define TIMER0_A1_VECTOR    TIMERA1_VECTOR
#define TIMER0_A0_VECTOR    TIMERA0_VECTOR
#endif


volatile long tempRaw;

void FaultRoutine(void);

/*********************************************************************
 * Function:        void initTimerA(void)
 *
 * PreCondition:    None
 *
 * Input:           mode    - None
 *
 * Output:          Internal timers configured
 *
 * Side Effects:    None
 *
 * Overview:
 *
 * Note:
 *
 ********************************************************************/
void initTimerA(void)
{
  TACTL &= ~MC1;
 // TACTL  = (TASSEL_2 + TIMERA_DIVIDER + TACLR);    // SMCLK, clear TAR
  CCTL0  = CCIE;                  // CCR0 interrupt enabled
  TACTL |= MC1;                   // Start Timer_A in continuous mode
}




void Delay(unsigned int Value)
{
   volatile unsigned int j = 0;

   for(j=Value; j>0; j--);
}



void main(void)
{
  // init===========
  initTimerA();

 static  uint32_t relexTimer =1000000000UL;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  P1DIR = 0x41;                             // P1.0 & 6 output (red/green LEDs)
  P1OUT = 0;                                // LEDs off
//
//if (calbc1_1mhz ==0xff || caldco_1mhz == 0xff)
//   faultroutine();		            // if calibration data is erased
// 				            // run faultroutine()
//  bcsctl1 = calbc1_1mhz; 		    // set range
//  dcoctl = caldco_1mhz;  		    // set dco step + modulation
//
//  bcsctl3 |= lfxt1s_2;                      // lfxt1 = vlo
//  ifg1 &= ~ofifg;                           // clear oscfault flag
//  bcsctl2 |= selm_0 + divm_3;               // mclk = dco/8

   timer(&relexTimer, TIMER_START);
  while(1)
  {
   if (!timer(&relexTimer ,TIMER_START))
   {
     timer(&relexTimer ,TIMER_STOP);
   P1OUT = 0x01;   			    // green LED on
   //_delay_cycles(100);
    Delay(10000);

    P1OUT = 0;
     Delay(100000);
   }
   /* Configure ADC Temp Sensor Channel */
  ADC10CTL1 = INCH_10 + ADC10DIV_0;         // Temp Sensor ADC10CLK/4
  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON; //  + ADC10IE;
  //__delay_cycles(1000);
   Delay(1000);// Wait for ADC Ref to settle
  ADC10CTL0 |= ENC + ADC10SC;               // Sampling and conversion start



    P1OUT = 0x40; 			    // green LED on
   //_delay_cycles(100);
    Delay(5000);


    ADC10CTL0 &= ~ENC;
    ADC10CTL0 &= ~(REFON + ADC10ON);
    tempRaw = ADC10MEM;


    P1OUT = 0;



    // green LED off


    Delay(5000);
    //_delay_cycles(5000);
  }
}

void FaultRoutine(void)
 {
   P1OUT = 0x01;                            // red LED on
   while(1); 			            // TRAP
 }







