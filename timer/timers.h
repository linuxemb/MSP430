


/*******************************************************************************
 *
 *  FILENAME:     timers.h
 *
 *  DESCRIPTION:  Header file for the timing layer:
 *
 *
 *  $Rev$
 *  $Date$
 *  $Author$
 *  $HeadURL$
 *
 *
 ******************************************************************************/

#ifndef __INCLUDE_timers_
#define __INCLUDE_timers_

/*****************************
 * Included Header Files     *
 *****************************/
#include <time.h>
#include <stdint.h>
/*****************************
 * Preprocessor Macros       *
 *****************************/
#define MASTER_CLK      20UL                                   // MHz

#define MCLK            (MASTER_CLK*1000000UL)
#define ACLK            (MCLK/2)                               // (See main.c)
//#define SMCLK           (MCLK/2)

#define SYSTEM_TICK     (250UL)                               // 'ticks' per second, or, 4ms/tick
#define CLOCKS_PER_TICK (ACLK / SYSTEM_TICK)
#define MS2TICKS(t)     (((uint32_t)t)/(1000UL/SYSTEM_TICK))    // Defines ms based on 'tick' units
#define TICKS2S(t)      (((uint32_t)t)/SYSTEM_TICK)

#define usDelay(us)     __delay_cycles((uint32_T)us * MASTER_CLK)




/*****************************
 * Data Structures           *
 *****************************/
typedef void (*fnDelayus)(int32_t us);
typedef void (*fnDelayms)(int16_t ms);

typedef enum
{
  TIMER_START,
  TIMER_STOP,
  TIMER_FORCE_TIMEOUT
} TIM_CTL;

typedef struct
{
  uint32_t timeout    : 31;
  uint32_t enabled    : 1;
} sanityTimer;


extern uint32_t systemTick;
extern uint32_t systemElapsedSeconds;
extern sanityTimer systemSanityTimer;   // Used for (rare) blocking instances (Eg flash die erases)




/*****************************
 * Public Functions          *
 *****************************/
void initTiming(void);
void delay(uint16_t ms);
uint16_t timer(uint32_t *counter, uint32_t period);
extern time_t makeTime(void);

#endif
