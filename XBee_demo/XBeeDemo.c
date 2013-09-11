//*****************************************************************************
//
// XBeeDemo.c - Demo for Xbee AT Command Set
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "utils/cmdline.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/lm4f120h5qr.h"
#include "rgb.h"
#include "XBee.h"

//LED Defines
#define REDON 		GPIO_PORTF_DATA_R |= 0x02
#define REDOFF 		GPIO_PORTF_DATA_R &= ~0x02
#define GREENON		GPIO_PORTF_DATA_R |= 0x08
#define GREENOFF	GPIO_PORTF_DATA_R &= ~0x08
#define BLUEON		GPIO_PORTF_DATA_R |= 0x04
#define BLUEOFF		GPIO_PORTF_DATA_R &= ~0x04

//*****************************************************************************
//
// Defines the size of the buffer that holds the command line.
//
//*****************************************************************************
#define CMD_BUF_SIZE            64

//*****************************************************************************
//
// The buffer that holds the command line.
//
//*****************************************************************************
static char g_pcCmdBuf[CMD_BUF_SIZE];


//*****************************************************************************
//
// The UART1 interrupt handler.
// Get response from XBee (UART1), print to terminal (UART0)
//
//*****************************************************************************
void
UART1IntHandler(void)
{
	unsigned char x;
    uint32_t ui32Status;

    //
    // Get the interrrupt status.
    //
    ui32Status = ROM_UARTIntStatus(UART1_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    ROM_UARTIntClear(UART1_BASE, ui32Status);

	  UARTprintf("Response:'");
    //
    // Loop while there are characters in the receive FIFO.
    //
    while(ROM_UARTCharsAvail(UART1_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        // if special character print out the integer for it
			  //
				x=ROM_UARTCharGetNonBlocking(UART1_BASE);
				if(x<' ' | x>'~')
				{
					UARTprintf("/%d",(int)x);
					//ROM_UARTCharPutNonBlocking(UART0_BASE,x);
				}
				else
				{
					ROM_UARTCharPutNonBlocking(UART0_BASE,x);
				}
					
    }
		UARTprintf("'\n");
}

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

int
test(int argc, char *argv[])
{
	char buff[2][10]={{'A','T','I','D',' '},{'2','4'}};
	EnterCmdMode();
	AT();
	//Cmd_ATID(2,[['A','T','I','D'],['2','5']]);
	//buff[0]="ATID";
	//buff[1]="25";
	Cmd_ATID(2,buff);
	//ATID(25);
	

	//UARTprintf("Test Value is: %d",TEST(123));
	
	//
	// Assumed success
	//
	return 0;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions, and
// brief description.
//
//*****************************************************************************
int Cmd_help(int argc, char *argv[]);
int Cmd_help(int argc, char *argv[]);
int Cmd_help(int argc, char *argv[]);
tCmdLineEntry g_sCmdTable[] =
{
    { "help",   Cmd_help,   "Display list of commands" },
    { "h",      Cmd_help,   "alias for help" },
    { "?",      Cmd_help,   "alias for help" },
		{ "+++",    Cmd_EnterCmdMode,   "Enter XBee Command Mode" },
		{ "AT",     Cmd_AT,     "Attention Command: returns 'OK' when in Command Mode" },
		{ "ATID",  	Cmd_ATID,   "ATID Command: Return / Set PAN ID" },
		{ "ATSH",		Cmd_ATSH,   "ATSH Command: Return Hex High half of XBEE Serial Number" },
		{ "ATSL",  	Cmd_ATSL,   "ATSL Command: Return Hex Low  half of XBEE Serial Number" },
		{ "ATDH",  	Cmd_ATDH,   "ATDH Command: Return / Set Hex High Destination Address" },
		{ "ATDL",  	Cmd_ATDL,   "ATDL Command: Return / Set Hex Low Destination Address" },
		{ "ATCN",  	Cmd_ATCN,   "Null Command: Use to drop out of Command Monde" },
		{ "ATWR",  	Cmd_ATWR,   "Write Command: Write current config to firmware (make current config default)" },
		{ "ATMY",  	Cmd_ATMY,   "My ID Command: Return XBee 16bit Address (non-hex)" },
		{ "ATD",  	Cmd_ATD,    "Config I/0 pins 0-7: usage: ATD <pin#> <command>  " },
		{ "ATP",  	Cmd_ATP,    "Config I/O pins 10-11: usage: ATP <pin#> <command> " },
		{ "ATIR",  	Cmd_ATIR,   "I/O Rate Set: Hex Value sets rate in miliseconds, 0 turns off " },
		{ "ATIT",  	Cmd_ATIT,   "Itteration Tailor: Set number of samples (hex) taken before transmit (max 0x44): ATIT <hex #> " },
		{ "ATIA",  	Cmd_ATIA,   "Input Address allows updates from given XBee address: ATIA <address> " },
		{ "AT%V",  	Cmd_ATV,    "% Voltage Command: Returns supply voltage, useful for tracking battery" },
		{ "ATPR",  	Cmd_ATPR,   "Pull Up Resistor: ATPR <1=on, 0=off>" },
		{ "ATRE",  	Cmd_ATRE,   "Reset Command: Reset all configs to factory presets" },
		{ "test",  	test,   		"test functionality" },

    { 0, 0, 0 }
};


//*****************************************************************************
//
// This function implements the "help" command.  It prints a simple list of the
// available commands with a brief description.
//
//*****************************************************************************
int
Cmd_help(int argc, char *argv[])
{
    tCmdLineEntry *psEntry;

    //
    // Print some header text.
    //
    UARTprintf("\nAvailable commands\n");
    UARTprintf("------------------\n");

    //
    // Point at the beginning of the command table.
    //
    psEntry = &g_sCmdTable[0];

    //
    // Enter a loop to read each entry from the command table.  The end of the
    // table has been reached when the command name is NULL.
    //
    while(psEntry->pcCmd)
    {
        //
        // Print the command name and the brief description.
        //
        UARTprintf("%6s: %s\n", psEntry->pcCmd, psEntry->pcHelp);

        //
        // Advance to the next entry in the table.
        //
        psEntry++;
    }

    //
    // Return success.
    //
    return(0);
}

//*****************************************************************************
//
// main loop - sets up UART1, UART 0, and the command loop to process commands
//
//*****************************************************************************
int
main(void)
{
	int nStatus;
	int x;
	
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    //
    // Set the clocking to run directly from the crystal.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

	//Setup UART1 on PB0 / PB1
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		GPIOPinConfigure(GPIO_PB0_U1RX);
		GPIOPinConfigure(GPIO_PB1_U1TX);
		GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
		UARTStdioConfig(1, 9600, SysCtlClockGet());
	
		ConfigureUART(); //UART0
		
		//
    // Enable the UART1 interrupt.
    //
    ROM_IntEnable(INT_UART1);
    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);


	//Initialize LED's
		SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
		x = SYSCTL_RCGC2_R; //dummy cycle
    GPIO_PORTF_DIR_R |= 0x08;
    GPIO_PORTF_DEN_R |= 0x08;
		GPIO_PORTF_DIR_R |= 0x04;
    GPIO_PORTF_DEN_R |= 0x04;
		GPIO_PORTF_DIR_R |= 0x02;
    GPIO_PORTF_DEN_R |= 0x02;
		
		//
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();
		
		//
    // Enter an infinite loop for reading and processing commands from the
    // user.
    //
		Cmd_help(0,0);
    while(1)
    {
        //
        // Print a prompt to the console.  Show the CWD.
        //
        UARTprintf("\n> ");

        //
        // Get a line of text from the user.
        //
        UARTgets(g_pcCmdBuf, sizeof(g_pcCmdBuf));

        //
        // Pass the line from the user to the command processor.  It will be
        // parsed and valid commands executed.
        //
        nStatus = CmdLineProcess(g_pcCmdBuf);

        //
        // Handle the case of bad command.
        //
        if(nStatus == CMDLINE_BAD_CMD)
        {
            UARTprintf("Bad command!\n");
        }

        //
        // Handle the case of too many arguments.
        //
        else if(nStatus == CMDLINE_TOO_MANY_ARGS)
        {
            UARTprintf("Too many arguments for command processor!\n");
        }

        //
        // Otherwise the command was executed.  Print the error code if one was
        // returned.
        //
        else if(nStatus != 0)
        {
            UARTprintf("Command returned error code\n");
        }
    }
			
    
}
