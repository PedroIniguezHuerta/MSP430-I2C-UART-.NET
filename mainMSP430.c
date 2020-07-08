// ******************************************************************************
//
//               MSP430G2xx1
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |   CCI0B/TXD/P1.1|-------->
//           |                 | 9600 8N1
//           |   CCI0A/RXD/P1.2|<--------
//
// ******************************************************************************

#include "msp430g2231.h"
//------------------------------------------------------------------------------
// Hardware-related definitions
//------------------------------------------------------------------------------
#define UART_TXD    0x02                     // TXD on P1.1 (Timer0_A.OUT0)
#define UART_RXD    0x04                     // RXD on P1.2 (Timer0_A.CCI1A)
#define NORMAL_MODE 0x00                     // Echo same character
#define INC_MODE    0x01                     // Echo incremented character
#define DEC_MODE    0x02                     // Echo decremented character

#define READ_SAMPLE 0x32                     // Command to slave to read data

//------------------------------------------------------------------------------
// Conditions for 9600 Baud SW UART, SMCLK = 1MHz
//------------------------------------------------------------------------------
#define UART_TBIT_DIV_2     (1000000 / (9600 * 2))
#define UART_TBIT           (1000000 / 9600)

//------------------------------------------------------------------------------
// Global variables used for full-duplex UART communication
//------------------------------------------------------------------------------
unsigned int txData;                   // UART internal variable for TX
unsigned char rxBuffer;                // Received UART character
unsigned char mode=NORMAL_MODE;        // mode
char MST_Data = 0;                     // Variable for transmitted data
char SLV_Sample = 0;                   // data sample received from slave
char SLV_Addr = 0x90;                  // Address is 0x48 << 1 bit + 0 for Write
int I2C_State = 0;                     // State variable
char serialRxReady = 0;                // Data arrived from serial?

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------
void TimerA_UART_init(void);
void TimerA_UART_tx(unsigned char byte);
void TimerA_UART_print(char *string);

void main(void)
{
  volatile unsigned int i;             // Use volatile to prevent removal
  
  /////////////////////////////////////////////////////////////////////////
  // Disable Watchdog timer, 
  /////////////////////////////////////////////////////////////////////////
  WDTCTL = WDTPW + WDTHOLD;            // Stop watchdog
  if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)                                     
  {  
    while(1);                          // If calibration constants erased
                                       // do not load, trap CPU!!
  }   
  
  /////////////////////////////////////////////////////////////////////////
  // Configure clock to 1 MHz 
  /////////////////////////////////////////////////////////////////////////
  BCSCTL1 = CALBC1_1MHZ;               // Set DCO
  DCOCTL = CALDCO_1MHZ;

  /////////////////////////////////////////////////////////////////////////
  // Configure IO ports
  /////////////////////////////////////////////////////////////////////////
  P1OUT = 0xC0;                           // P1.6 & P1.7 Pullups, others to 0
  P1SEL = UART_TXD + UART_RXD;            // Timer function for TXD/RXD pins
  P1REN |= 0xC0;                          // P1.6 & P1.7 Pullups
  P1DIR = 0xFF & ~UART_RXD;               // Set all pins but RXD to output

  P2OUT = 0x00;
  P2SEL = 0x00;
  P2DIR = 0xFF;
  
  /////////////////////////////////////////////////////////////////////////
  // Configure I2C Communication as Master
  /////////////////////////////////////////////////////////////////////////
  USICTL0 = USIPE6+USIPE7+USIMST+USISWRST;// Port & USI mode setup, MASTER
  USICTL1 = USII2C+USIIE;                 // Enable I2C mode & USI interrupt
  USICKCTL = USIDIV_3+USISSEL_2+USICKPL;  // Setup USI clocks: SCL = SMCLK/8 (~125kHz)
  USICNT |= USIIFGCC;                     // Disable automatic clear control
  USICTL0 &= ~USISWRST;                   // Enable USI
  USICTL1 &= ~USIIFG;                     // Clear pending flag
  
  /////////////////////////////////////////////////////////////////////////
  // Enable Interrupts
  /////////////////////////////////////////////////////////////////////////
  __enable_interrupt();
    
  /////////////////////////////////////////////////////////////////////////
  // Configure Timer A0 FOR UART and send informational message through serial
  /////////////////////////////////////////////////////////////////////////
  TimerA_UART_init();                     // Start Timer_A UART  
  TimerA_UART_print("Controller v1.0 Ready\n");
    
  /////////////////////////////////////////////////////////////////////////
  // Informs Serial data is not ready
  /////////////////////////////////////////////////////////////////////////
  serialRxReady = 0;
    
  /////////////////////////////////////////////////////////////////////////
  // Enter in a never ending loop receiving requests from PC and sending responses
  /////////////////////////////////////////////////////////////////////////
  while(1)
  {
    /////////////////////////////////////////////////////////////////////////
    // Enter LMP0 mode and wait for USI interrupt
    /////////////////////////////////////////////////////////////////////////
     __bis_SR_register(LPM0_bits);
     
    _NOP();                              // Used for IAR
       
    /////////////////////////////////////////////////////////////////////////
    // If request received from PC, process it
    /////////////////////////////////////////////////////////////////////////
    if(serialRxReady == 0) // if no serial data, then it meants I2C data arrived
       TimerA_UART_tx(MST_Data+33);
    else
    {
        serialRxReady = 0;
        switch (rxBuffer)
        {
          case 33:  // '!'
            TimerA_UART_print("Master v1.0\n");
          break;
          case 35: // '#'
            P1OUT = 0;
          break;
          case 43: // '+'
            mode = INC_MODE;           
          break;
          case 45: // '-'
            mode = DEC_MODE;           
          break;
          case 61: // '='
            mode = NORMAL_MODE;           
          break;
          case 42: // '*'
            P1OUT ^= 0x01;
          break;
          case 47: // '/'  ==> Read sample
            MST_Data = READ_SAMPLE;
            USICTL1 |= USIIFG;  // Start I2C communication
       
            // wait until response comes from slave
            __bis_SR_register(LPM0_bits);
            
            // return the sample
            TimerA_UART_tx(SLV_Sample);
          break;
          default: // turn leds and Echo received character
            
            // reply character
            switch(mode)
            {
              case INC_MODE:
                rxBuffer++;
              break;
              case DEC_MODE:
                rxBuffer--;
              break;
              default:
                ; // return same character
            }
            
            TimerA_UART_tx(rxBuffer);
        }
    }
    
  }
}

// ***************************************************
// USI interrupt service routine
// ***************************************************
#pragma vector = USI_VECTOR
__interrupt void USI_TXRX (void)
{
  switch(I2C_State)
    {
      case 0: // Generate Start Condition & send address to slave
              P1OUT |= 0x01;           // LED on: sequence start
              USISRL = 0x00;           // Generate Start Condition...
              USICTL0 |= USIGE+USIOE;
              USICTL0 &= ~USIGE;
              USISRL = SLV_Addr;       // ... and transmit address, R/W = 0
              USICNT = (USICNT & 0xE0) + 0x08; // Bit counter = 8, TX Address
              I2C_State = 2;           // Go to next state: receive address (N)Ack
              break;

      case 2: // Receive Address Ack/Nack bit
              USICTL0 &= ~USIOE;       // SDA = input
              USICNT |= 0x01;          // Bit counter = 1, receive (N)Ack bit
              I2C_State = 4;           // Go to next state: check (N)Ack
              break;

      case 4: // Process Address Ack/Nack & handle data TX
              USICTL0 |= USIOE;        // SDA = output
              if (USISRL & 0x01)       // If Nack received...
              { // Send stop...
                USISRL = 0x00;
                USICNT |=  0x01;       // Bit counter = 1, SCL high, SDA low
                I2C_State = 10;        // Go to next state: generate Stop
                P1OUT |= 0x01;         // Turn on LED: error
              }
              else
              { // Ack received, TX data to slave...
                USISRL = MST_Data;     // Load data byte
                USICNT |=  0x08;       // Bit counter = 8, start TX
                I2C_State = 6;         // Go to next state: receive data (N)Ack
                P1OUT &= ~0x01;        // Turn off LED
              }
              break;

      case 6: // Receive Data Ack/Nack bit
              USICTL0 &= ~USIOE;       // SDA = input
              USICNT |= 0x01;          // Bit counter = 1, receive (N)Ack bit
              I2C_State = 8;           // Go to next state: check (N)Ack
              break;

      case 8: // Process Data Ack/Nack & and receive data
              USICTL0 |= USIOE;
              if (USISRL & 0x01)       // If Nack received...
              {
                P1OUT |= 0x01;         // Turn on LED: error
                // Send stop...
                USISRL = 0x00;
                USICNT |=  0x01;       // Bit counter = 1, SCL high, SDA low
                I2C_State = 10;        // Go to next state: generate Stop
              }
              else                     // Ack received, read sample now
              {
                USICTL0 &= ~USIOE;     // SDA = input
                USICNT |= 0x08;        // Bit counter = 8, receive Sample
                I2C_State = 12;        // Go to next state: check (N)Ack
              }
              
              break;

      case 10:// Generate Stop Condition
              USISRL = 0x0FF;          // USISRL = 1 to release SDA
              USICTL0 |= USIGE;        // Transparent latch enabled
              USICTL0 &= ~(USIGE+USIOE);// Latch/SDA output disabled
              I2C_State = 0;           // Reset state machine for next transmission
              __bic_SR_register_on_exit(LPM0_bits);  // Clear LPM0 bits from 0(SR)
              break;
              
      case 12: // Receive Data sample & send Ack/Nack
              USICTL0 |= USIOE;
              SLV_Sample = USISRL;     // Store the sample received from Slave
              P1OUT &= ~0x01;          // Turn off LED

              // Send ack...
              USISRL = 0x00;
              USICNT |=  0x01;         // Bit counter = 1, SCL high, SDA low
              I2C_State = 10;          // Go to next state: generate Stop
              break;              
    }

  USICTL1 &= ~USIIFG;                  // Clear pending flag
}

//------------------------------------------------------------------------------
// Function configures Timer_A for full-duplex UART operation
//------------------------------------------------------------------------------
void TimerA_UART_init(void)
{
    TACCTL0 = OUT;                          // Set TXD Idle as Mark = '1'
    TACCTL1 = SCS + CM1 + CAP + CCIE;       // Sync, Neg Edge, Capture, Int
    TACTL = TASSEL_2 + MC_2;                // SMCLK, start in continuous mode
}

//------------------------------------------------------------------------------
// Outputs one byte using the Timer_A UART
//------------------------------------------------------------------------------
void TimerA_UART_tx(unsigned char byte)
{
    while (TACCTL0 & CCIE);                 // Ensure last char got TX'd
    TACCR0 = TAR;                           // Current state of TA counter
    TACCR0 += UART_TBIT;                    // One bit time till first bit
    TACCTL0 = OUTMOD0 + CCIE;               // Set TXD on EQU0, Int
    txData = byte;                          // Load global variable
    txData |= 0x100;                        // Add mark stop bit to TXData
    txData <<= 1;                           // Add space start bit
}

//------------------------------------------------------------------------------
// Prints a string over using the Timer_A UART
//------------------------------------------------------------------------------
void TimerA_UART_print(char *string)
{
    while (*string)
        TimerA_UART_tx(*string++);
}
//------------------------------------------------------------------------------
// Timer_A UART - Transmit Interrupt Handler
//------------------------------------------------------------------------------
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A0_ISR(void)
{
    static unsigned char txBitCnt = 10;

    TACCR0 += UART_TBIT;                    // Add Offset to CCRx
    if (txBitCnt == 0)                      // All bits TXed?
    {
        TACCTL0 &= ~CCIE;                   // All bits TXed, disable interrupt
        txBitCnt = 10;                      // Re-load bit counter
    }
    else
    {
        if (txData & 0x01)
          TACCTL0 &= ~OUTMOD2;              // TX Mark '1'
        else
          TACCTL0 |= OUTMOD2;               // TX Space '0'

        txData >>= 1;
        txBitCnt--;
    }
}      
//------------------------------------------------------------------------------
// Timer_A UART - Receive Interrupt Handler
//------------------------------------------------------------------------------
#pragma vector = TIMERA1_VECTOR
__interrupt void Timer_A1_ISR(void)
{
    static unsigned char rxBitCnt = 8;
    static unsigned char rxData = 0;

    switch (__even_in_range(TAIV, TAIV_TAIFG))   // Use calculated branching
    {
        case TAIV_TACCR1:                        // TACCR1 CCIFG - UART RX
          TACCR1 += UART_TBIT;                 // Add Offset to CCRx
          if (TACCTL1 & CAP)                   // Capture mode = start bit edge
          {
              TACCTL1 &= ~CAP;                 // Switch capture to compare mode
              TACCR1 += UART_TBIT_DIV_2;       // Point CCRx to middle of D0
          }
          else
          {
              rxData >>= 1;
              if (TACCTL1 & SCCI)              // Get bit waiting in receive latch
                  rxData |= 0x80;

              rxBitCnt--;
              
              if (rxBitCnt == 0)               // All bits RXed?
              {
                  rxBuffer = rxData;           // Store in global variable
                  serialRxReady = 1;

                  rxBitCnt = 8;                // Re-load bit counter
                  TACCTL1 |= CAP;              // Switch compare to capture mode
                  __bic_SR_register_on_exit(LPM0_bits);  // Clear LPM0 bits from 0(SR)
              }
          }
        break;
    }
}
