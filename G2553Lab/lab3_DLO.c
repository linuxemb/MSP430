//***********************************************************
// Lab3a.c  Using VLO/8 as MCLK
//
// Green LED on LaunchPad should toggle about every 3 seconds
//e
// SFB 1/2012
//**********************************ew************************

#include  "msp430g2553.h"

#ifndef TIMER0_A1_VECTOR
#define TIMER0_A1_VECTOR    TIMERA1_VECTOR
#define TIMER0_A0_VECTOR    TIMERA0_VECTOR
#endif
void _delay_cycles (int count)
{
  for (int i=0; i<count; i++)
  {
  }
}
void main(void)
{

  WDTCTL = WDTPW + WDTHOLD;		// watchdog timer setup

  if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)
  {
    while(1);            			// If cal constants erased,
  }

  BCSCTL1 = CALBC1_1MHZ; 			// Set range
  DCOCTL = CALDCO_1MHZ;  			// Set DCO step + modulation
  P1DIR = 0x41;

  P1OUT =0x01;

  BCSCTL3 |= LFXT1S_2 + XCAP_3;	// clock system setup
  IFG1 &= ~OFIFG;
  //_bis_SR_register(SCG1 + SCG0);
   BCSCTL2 |= SELM_0 + DIVM_3;
  // BCSCTL3 |=LFXT1S_0 + XCAP_3; // colck systemsetup
  while (IFG1 &OFIFG)
  {
    IFG1 &=~OFIFG;
    _delay_cycles(100000);
  }

  P1OUT= 0;

  while(1)
  {
    P1OUT = 0x41; 			    // LED on


    _delay_cycles(10000);
    P1OUT = 0; 			    	// LED off
    _delay_cycles(10000);
  }



}
