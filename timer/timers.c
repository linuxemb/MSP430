


/*****************************
 * Included Header Files     *
 *****************************/

#include "timers.h"
#include <stdint.h>

#include <msp430g2553.h>

/*****************************
 * Externs                   *
 *****************************/
//extern void timerCallback(void);

#define TASSEL__ACLK        (1*0x100u) /* Timer A clock source select: 1 - ACLK  */
/*****************************
 * Preprocessor Macros       *
 *****************************/




/*****************************
 * Data Structures           *
 *****************************/


/*****************************
 * Global Data               *
 *****************************/
uint32_t systemTick = 0;
uint32_t systemElapsedSeconds = 0;
sanityTimer systemSanityTimer = {0};




/*****************************
 * Private Functions         *
 *****************************/
static void initTimerA(void);
__interrupt static void timerA(void);
__interrupt static void timerB(void);





/*********************************************************************
 * Function:        timer
 *
 * PreCondition:    None
 *
 * Input:           *counter: Pointer to timer data
 *                  period:   'RESET', 'STOP', 'FORCE_TIMEOUT' or the period in ms to time
 *
 * Output:          Return code: 0= expired; !0= not expired
 *
 * Side Effects:    None
 *
 * Overview:        General purpose non-blocking timer control
 *
 * Note:            None
 *
 ********************************************************************/
uint16_t timer(uint32_t *counter, uint32_t period)
{
  if (period == TIMER_FORCE_TIMEOUT)
  {
    *counter = 1;
  }
  else if (period == TIMER_STOP)
  {
    *counter = 0;
  }
  else if (period == TIMER_START)                // means "timer initialize"
  {
    *counter = systemTick;
  }
  else if (*counter)
  {
    TA0CCTL0 = 0;
    if (systemTick < *counter)              // Handle the "Y2K" scenario...
    {
      if ((((uint64_t)systemTick + 0x100000000ULL) - *counter) > MS2TICKS(period))
      {
        *counter = period = 0;              // Flag expiry
      }
    }
    else
    {
      if ((systemTick - *counter) > MS2TICKS(period)) // Otherwise check for expiry ?
      {
        *counter = period = 0;              // Flag expiry
      }
    }
    TA0CCTL0 = CCIE;
  }
  else
  {
  }

  return period;
}






/*********************************************************************
 * Function:        delay
 *
 * PreCondition:    None
 *
 * Input:           ms: Milliseconds to delay
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Delays (blocks!) for the requested period
 *
 * Note:            None
 *
 ********************************************************************/
void delay(uint16_t ms)
{
  uint16_t startCount = systemTick;

  ms = MS2TICKS(ms);

  while ((systemTick - startCount) < ms)
  {
  }
}





/*********************************************************************
 * Function:        initTimerA
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Initializes the MSP timer A, used for system & core OS heartbeat
 *
 * Note:            None
 *
 ********************************************************************/
static void initTimerA(void)
{
  TA0CCR0 = CLOCKS_PER_TICK & 0xffff;
  TA0CCR1 = CLOCKS_PER_TICK >> 16;
  TA0CTL = TASSEL__ACLK + MC_1 + TACLR; // ACLK, count to CCR0 then roll, clear TAR
  TA0CCTL0 = CCIE;                  // Gen int at rollover (TIMER0_A0 vector)
}



/*********************************************************************
 * Function:        timerA
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          System timers and OS heartbeat updated
 *
 * Side Effects:    None
 *
 * Overview:        Internal timer A ISR
 *
 * Note:            None
 *
 ********************************************************************/
//#pragma vector=TIMERA1_VECTOR
#pragma vector=TIMER0_A0_VECTOR
__interrupt static void timerA(void)
{
 //entering_isr();

  //OSTimer();

  systemTick++;

  if (!(systemTick % SYSTEM_TICK))
  {
    systemElapsedSeconds++;       // Relative
	  //pack.systemDateTimeSeconds++; // Absolute
  }

  //timerCallback();

  if (systemSanityTimer.enabled)
  {
    if (!--systemSanityTimer.timeout)
    {
      //RESET_PROCESSOR();
    }
  }

  __low_power_mode_off_on_exit();

  //leaving_isr();
}



/*********************************************************************
 * Function:        makeTime
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Returns seconds since EPOCH
 *
 * Side Effects:    None
 *
 * Overview:        Based on soft seconds counter (see above)
 *
 * Note:            None
 *
 ********************************************************************/
time_t makeTime(void)
{
 // return pack.systemDateTimeSeconds;
}




/*********************************************************************
 * Function:        timerB
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:
 *
 * Side Effects:    None
 *
 * Overview:        Unused at present
 *
 * Note:            None
 *
 ********************************************************************/
//#pragma vector=TIMER0_B0_VECTOR


__interrupt static void timerB(void)
{
}





/*********************************************************************
 * Function:        initTiming
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Top-level function to set up all the clocks/timers
 *
 * Note:            None
 *
 ********************************************************************/
void initTiming(void)
{
  initTimerA();
}




/*********************************************************************
 * Function:        void timerA(void)
 *
 * PreCondition:    None
 *
 * Input:           mode    - None
 *
 * Output:          Internal timer interrupt service routine
 *
 * Side Effects:    None
 *
 * Overview:
 *
 * Note:
 *
 ********************************************************************/
//#pragma vector=TIMERA0_VECTOR
#pragma vector=TIMER0_A0_VECTOR


__interrupt void timera(void)
{
  ccr0 += clocks_per_tick;

// systemcounter++;
}
