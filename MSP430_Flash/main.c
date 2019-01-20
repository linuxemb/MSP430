//***********************************************************
//
//
// Flash REad and Write test 
//
// 
//**********************************ew************************

#include  "msp430g2553.h"
#include "head.h"
#include <stdint.h>
#include <stdio.h>
#ifndef TIMER0_A1_VECTOR
#define TIMER0_A1_VECTOR    TIMERA1_VECTOR
#define TIMER0_A0_VECTOR    TIMERA0_VECTOR
#endif
#define MASTER_CLK      20UL                                   // MHz
#define UART_SPEED             (115200)
#define MCLK            (MASTER_CLK*1000000UL)
#define ACLK            (MCLK/2)
#define UCRXIE         (0x0001u)


#define UCSSEL__ACLK        (0x40)    /* USCI 0 Clock Source: ACLK */
#define UCSSEL__SMCLK       (0x80)    /* USCI 0 Clock Source: SMCLK */
char Wdata[10]={'7','2','0','4','2','8','6','2','2','8'};
char WRdata[10] = {'a','b','3','4','5','6','7','2','2','8'};
char  Rdata[10];

char RDdata[10];


//UART  init
void initUART_USCIA1(uint8_t whichClock, uint32_t Hz)
{
  uint16_t divider = Hz / UART_SPEED;

  UCA0CTL0 = UCSWRST;

  UCA0CTL0 = 0;                         // 8-bit, no parity, 1 stop
  UCA0CTL0 |= whichClock;               // ACLK

  UCA0BR0 = divider & 0xff;
  UCA0BR1 = divider >> 8;
  UCA0MCTL = UCBRS2 + UCBRS0;           // See Table

  P1SEL |= (BIT2 + BIT3);               // P8.2,3 = UCA1 TXD/RXD

  UCA0CTL0 &= ~UCSWRST;

 // UCA0IFG = 0;
  //UCA0RXIFG|= UCRXIE;                     // Enable the USART0 Rx interrupts */
}

void _delay_cycles (int count)
{
  for (int i=0; i<count; i++)
  {
  }
}


/*************************************
****     Read
***********************************/

void read (void)
{
      char  *Flash_ptr;                          // Flash pointer
      unsigned int i;
      Flash_ptr = (char *) 0x1040;              // Initialize Flash pointer

      for(i=0;i<10;i++)
      {
         // while ((FCTL3 & BUSY) != 0);   not needed
          Rdata[i]=*Flash_ptr++;

      }
}
/*************************************
****  Write
***********************************/
void write ()
{
  char *Flash_ptr;                          // Flash pointer
  unsigned int i;

  Flash_ptr = (char *) 0x1040;              // Initialize Flash pointer
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Cleardff Lock bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash segment

  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i=0; i<10; i++)
  {
   // *Flash_ptr++ = Wdata[i];                   // Write value to flash
    *Flash_ptr++  = WRdata[i];
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}



/*************main *****************************/
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
  {
    while(1);                               // do not load, trap CPU!!
  }
  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_8MHZ;                    // Set DCO to 8MHz
  DCOCTL = CALDCO_8MHZ;
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator

write();

read();

while(1);  //debugging


}




