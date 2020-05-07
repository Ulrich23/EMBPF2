#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

#include "spi_config.h"

void write_spi(INT16U data)   // Send a bit with PB7 (TX pin)
{
    SSI2_DR_R = data; // DR = Data register
    while( (SSI2_SR_R & (1<<0)) == 0);  // SR = status register. Wait for Data Register to be empty before returning.
                                        // Check bit field 0 if it is set.
                                        // If bit 0 in SR register is set = transmit FIFO = Empty ( if bit 0 is 0 = not empty)
                                        // Data is located in the FIFO if bit 0 in SR register is 1.

}

INT16U read_spi()   // This function MUST be called right after the write SPI.
{
    INT16U receive_data = 0;
    while(SSI2_SR_R & (0<<3) ){     // Receive FIFO is not full wait for it to be full
        //receive_data = SSI2_DR_R;
    }
    receive_data = SSI2_DR_R;       // Receive FIFO is now full, return the data.
    return receive_data;
}


void init_master_spi() // Following the guide on page 965 (15.4) in the datasheet
{

// 1. Start by enabling the SSI module using the RCGCSSI register (page 346)
// Enable the "SSI Module 2" by setting bit/field 2 of the RCGCSSI register to 1 (-1--)
// SSI module 2 is described in table 15.1 (page 954) and described shortly here:
// SSI module 2 is using PB4-PB7 (xxxx----) highest nibble of PORTB
// ****************************************
// PB4 --> I/O --> Clock (SCLK/SCK)
// PB5 --> I/O --> Frame Signal (SS/CS/CE)
// PB6 --> I   --> Receive (MISO/SDI)
// PB7 --> O   --> Transmit (MOSI/SDO)
// ****************************************
    SYSCTL_RCGCSSI_R |= (1<<2); // (1<<2) = 0x2 = (0100)

// 2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register (page 340).
// To find out which GPIO port to enable, refer to Table 23-5 (page 1351).
// We are using PORTB and PORTB is bit/field 1 of the RCGCGPIO register, enable by setting this bit to 1 (--1-)
    SYSCTL_RCGCGPIO_R |= (1<<1);

// 3. Set the GPIO AFSEL bits for the appropriate pins (page 671). To determine which GPIOs to
// configure, see Table 23-4 (page 1344).
// PB4-PB7 was chosen above when choosing mode 2, so for these appropriate bits we set the GPIO AFSEL bits:
    GPIO_PORTB_AFSEL_R |= (1<<4)|(1<<5)|(1<<6)|(1<<7); // 0xF0 = (1111 0000)

// 4. Configure the PMCn fields in the GPIOPCTL register to assign the SSI signals to the appropriate
// pins. See page 688 and Table 23-5 (page 1351) (The last table is CRUCIAL to figuring out which to set). Because
// it shows the alternate functions corresponding to the PMCn bit field encoding.
// SSI function for PB4-PB7, PMC's corresponds to the GPIOpins we are using, so we need to use PMC4-PMC7.
// The PMC's need the value 2 (can be seen in table 23-5) which enables them for alternative function (SSI)
// the 2 corresponds to the alternative function SSI.

    GPIO_PORTB_PCTL_R |= (2<<16)|(2<<20)|(2<<24)|(2<<28);  // Ex: (0000 0000) |= (2<<4) --> (0010 000). Husk (0x2) = (0010)

// 5. Enable the pin's digital function.
    GPIO_PORTB_DEN_R |= (1<<4)|(1<<5)|(1<<6)|(1<<7);
// Some more needs to be configured here? Drive strength, drain select PULLUP/DOWN???
    //GPIO_PORTF_PUR_R |= (1<<6); //Pullup for the RX pin???

// For each of the frame formats, the SSI is configured using the following steps.

// 1. Ensure that the SSE bit (bit/field 1) in the SSICR1 register is clear before making any configuring changes.
// Remember that we were using MODE 2, so we need to configure SSI control 1 (SSICR1) for SSI2 (SSI mode 2)
// Disable the SSI:
    SSI2_CR1_R &= ~(1<<1); // We have to put the value 0, so i invert the bit (nibble 1101)

// 2. Select whether the SSI is a master or slave:
// a. For master operations, set the SSICR1 register to 0x0000.0000.
// b. For slave mode (output enabled), set the SSICR1 register to 0x0000.0004.
// c. For Slave mode (output Disabled), set the SSICR1 register to 0x000.000C
    SSI2_CR1_R = 0x00000000;

// 3. Configure the SSI clock source by writing to the SSICC register.
    SSI2_CC_R = 0x00; // Set SSI clock source to SYSTEM CLOCK (16 MHz)

// 4. Configure the clock prescale divisor by writing the SSICPSR register (SSI CLOCK PRESCALE REGISTER)
    SSI2_CPSR_R = 2; // Dividing the 16 MHz by the value? Test if you can write 0 and get all 16 MHz'

// 5. Write the SSICR0 register with the following configuration:
// - Serial Clock Rate (SCR)
// - Desired clock phase/polarity, if using Freescale SPI mode (SPH and SPO)
// - The Protocol mode: Freescale SPI, TI SSF, MICROWIRE (FRF)
// - The Data Size (DSS)
// * We are using the Freescale SPI mode.
// * Not changing the clock rate
// * 14 Bit data size (0xD)
// * Sample on the first clock edge
// Mode 00 (SPI MODE 0), CS is low when data is transferred, bit is read (sampled) on rising clock edge.
    SSI2_CR0_R = (0xD<<0);

// 6. Optionally...
// 7. Enable the SSI by setting the SSE  bit in the SSICR1 register.
    SSI2_CR1_R |= (1<<1);

}
