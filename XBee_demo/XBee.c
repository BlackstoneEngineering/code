//*****************************************************************************
//
// XBee.c - XBee AT Comand implementation for Stellaris / Tiva Launchpad
//
// Copyright Austin Blackstone Engineering 2013
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

//<- ##### README BEGIN ##### ->
//*****************************************************************************
//!
//! XBee.c is a low level implementation of the XBee AT Command Set and should
//! be paired with XBee.h . The '+++' command is used to enter command mode, 
//! and is the first command that must be entered before any other command
//! will be accepted. 
//!
//! This implementation is meant to be modular to any system. The Macro 
//! XBEEWRITE can be replaced by any function that will write to the UART port
//! connected to the XBEE. This version is set up to write characters one by 
//! one, but that is not necessary. 
//!
//! 
//!
//!
//!
//!
//!
//*****************************************************************************
//<- ##### README END ##### ->

#include <cstdlib>
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

//*****************************************************************************
//
// Enter AT Command Mode Command
// Input: n/a
// Response: 'OK'
// Use: To enter Command mode so other commands may be used. If no command is
//		entered for 10 seconds command mode is exited. On successful entry to
//		command mode a reply of 'OK' will be received. Do not type any commands
//		until the 'OK' command is received. (~3s)
//
//*****************************************************************************
int Cmd_EnterCmdMode(int argc, char *argv[])
{
	//
	// Required wait 
	//
	SysCtlDelay(SysCtlClockGet()*2);
	
	//
	// Send "+++" command to XBee to "Enter AT Command Mode"
	//
	UARTCharPut(UART1_BASE,'+');
	UARTCharPut(UART1_BASE,'+');
	UARTCharPut(UART1_BASE,'+');
	
	//
	// Requird wait 
	//
	SysCtlDelay(SysCtlClockGet());
	
	//
	// Assumed Success
	//
	return 0;
}

//*****************************************************************************
//
// Attention Command 
// Input: n/a
// Response: 'OK'
// Use: To check if XBee is in command mode. Should receive 'OK' if in command
//		mode. If 'OK' not received then you need to re-enter command mode.
//
//*****************************************************************************
int Cmd_AT(int argc, char *argv[])
{
	UARTCharPut(UART1_BASE,'A');
	UARTCharPut(UART1_BASE,'T');
	UARTCharPut(UART1_BASE,'\r');
	
	//
	// Assumed Success
	//
	return 0;
}

//*****************************************************************************
//
// Personal Aread Network ID Command
// Input: nothing to return PAN ID, desired PAN ID to set PAN ID
// Response: current PAN ID
// Use: to return or set PAN ID
//
// NOTE: PAN ID's are in Hex, max 4 characters
//
//*****************************************************************************
int Cmd_ATID(int argc, char *argv[])
{
	int x;
	char y = 'a';
	
	//
	// Process commands
	//
		if( 1 == argc )
	{
		//
		// most common case, just return PAN ID
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'I');
		UARTCharPut(UART1_BASE,'D');
		UARTCharPut(UART1_BASE,'\r');
		
	}
	else if( argc > 3 )
	{
		//
		// Error check: too many arguements, 
		//
		UARTprintf("Error: too many arguements, try again\n");
		return 1;
	}
	else
	{
		//
		// If address is given set it
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'I');
		UARTCharPut(UART1_BASE,'D');
		UARTCharPut(UART1_BASE,' ');
		
		//
		// Send address
		//
		for(x=0;y!=0 ;x++ )
			{
				//
				// Set y=hex value of ascii character
				//
				y= argv[1][x];
				
				//
				// Send character to XBee
				//
				UARTCharPut(UART1_BASE,y);
			}
		
		//
		// End of command character
		//
		UARTCharPut(UART1_BASE,'\r');
	}
	
	
	return 0;
}

//*****************************************************************************
//
// Serial Number High Command
// Input: n/a
// Response: high 32 bits of 64 bit unique serial number
// Use: to check the unique serial number, useful for authentication 
//
// NOTE: Serial Number is in Hex
//
//*****************************************************************************
int Cmd_ATSH(int argc, char *argv[])
{
	//
	// Send ATSH
	//
	UARTCharPut(UART1_BASE,'A');
	UARTCharPut(UART1_BASE,'T');
	UARTCharPut(UART1_BASE,'S');
	UARTCharPut(UART1_BASE,'H');
	UARTCharPut(UART1_BASE,'\r');
	
	//
	// Assumed Success
	//
	return 0;
}

//*****************************************************************************
//
// Serial Number Low Command
// Input: n/a
// Response: low 32bits of 64bit unique serial number
// Use: to check unique serial number, useful for authentication
//
// NOTE: Serial Number is in Hex
//
//*****************************************************************************
int Cmd_ATSL(int argc, char *argv[])
{
	//
	// Send 'ATSL'
	//
	UARTCharPut(UART1_BASE,'A');
	UARTCharPut(UART1_BASE,'T');
	UARTCharPut(UART1_BASE,'S');
	UARTCharPut(UART1_BASE,'L');
	UARTCharPut(UART1_BASE,'\r');
	
	//
	// Assumed Success
	//
	return 0;
}

//*****************************************************************************
//
// XBee Destination Address (or Serial Number) High
// Input: none / High part of destination address
// Response: n/a
// Use: to check / set the high part of the destination address.
//		follow the command by nothing to check, or by the high part of the 
//		address to set it. 
//
// NOTE: Serial Number is in Hex 
//
//*****************************************************************************
int Cmd_ATDH(int argc, char *argv[])
{
	int x;
	char y = 'a'; //dummy value, default 0 is a check case
	
	//
	// Process commands
	//
	if( 1 == argc )
	{
		//
		// Most common case, just return Destination Address
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'D');
		UARTCharPut(UART1_BASE,'H');
		UARTCharPut(UART1_BASE,'\r');
		
	}
	else if( argc > 3 )
	{
		//
		// Error check: too many arguements, 
		//
		UARTprintf("Error: too many arguements, try again\n");
		return 1;
	}
	else
	{
		//
		// If address is given set it
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'D');
		UARTCharPut(UART1_BASE,'H');
		UARTCharPut(UART1_BASE,' ');
		
		//
		// Send address
		//
		for(x=0;y!=0 ;x++ )
			{
				//
				// Set y=hex value of ascii character
				//
				y= argv[1][x];
				
				//
				// Send character to XBee
				//
				UARTCharPut(UART1_BASE,y);
			}
		
		//
		// End of command character
		//
		UARTCharPut(UART1_BASE,'\r');
	}
	
	return 0;
}

//*****************************************************************************
//
// XBee Destination Address (or Serial Number) Low
// Input: none / Low part of destination address
// Response: n/a
// Use: to check / set the low part of the destination address.
//		follow the command by nothing to check, or by the low part of the 
//		address to set it. 
//
// NOTE: Serial Number is in Hex 
//
//*****************************************************************************
int Cmd_ATDL(int argc, char *argv[])
{
	int x;
	char y = 'a'; //dummy value, default 0 is a check case
	
	//
	// Process commands
	//
	if( 1 == argc )
	{
		//
		// most common case, just return destination address
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'D');
		UARTCharPut(UART1_BASE,'L');
		UARTCharPut(UART1_BASE,'\r');
		
	}
	else if( argc > 3 )
	{
		//
		// Error check: too many arguements, 
		//
		UARTprintf("Error: too many arguements, try again\n");
		return 1;
	}
	else
	{
		//
		// If address is given set it
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'D');
		UARTCharPut(UART1_BASE,'L');
		UARTCharPut(UART1_BASE,' ');
		
		//
		// Send address
		//
		for(x=0;y!=0 ;x++ )
			{
				//
				// Set y=hex value of ascii character
				//
				y= argv[1][x];
				
				//
				// Send character to XBee
				//
				UARTCharPut(UART1_BASE,y);
			}
		
		//
		// End of command character
		//
		UARTCharPut(UART1_BASE,'\r');
	}
	
	return 0;
}

//*****************************************************************************
//
// Command Null
// Input: n/a
// Response: n/a
// Use: to drop out of command mode
//
//*****************************************************************************
int Cmd_ATCN(int argc, char *argv[])
{
	//
	// Send 'ATCN'
	//
	UARTCharPut(UART1_BASE,'A');
	UARTCharPut(UART1_BASE,'T');
	UARTCharPut(UART1_BASE,'C');
	UARTCharPut(UART1_BASE,'N');
	UARTCharPut(UART1_BASE,'\r');
	
	//
	// Assumed success
	//
	return 0;
}

//*****************************************************************************
//
// Write Setting to Firmware Command
// Input: n/a
// Response: n/a
// Use: write current configuration generated in command mode to firmware. 
//		If this is not done the settings will be lost on power cycle
//
//*****************************************************************************
int Cmd_ATWR(int argc, char *argv[])
{
	//
	// Send 'ATWR'
	//
	UARTCharPut(UART1_BASE,'A');
	UARTCharPut(UART1_BASE,'T');
	UARTCharPut(UART1_BASE,'W');
	UARTCharPut(UART1_BASE,'R');
	UARTCharPut(UART1_BASE,'\r');
	
	//
	// Assumed Success
	//
	return 0;
}

//*****************************************************************************
//
// MY ID Command
// Input: read / set 16bit address
// Response: 16bit address in non-hex 
// Use: display the XBee's 16bit Address in non-hex form
//
//*****************************************************************************
int Cmd_ATMY(int argc, char *argv[])
{
	int x;
	char y = 'a'; //dummy value, default 0 is a check case
	
	//
	// Process commands
	//
	if( 1 == argc )
	{
		//
		// Send basic command
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'M');
		UARTCharPut(UART1_BASE,'Y');
		UARTCharPut(UART1_BASE,'\r');
		
	}
	else if( argc > 3 )
	{
		//
		// Error check: too many arguements, 
		//
		UARTprintf("Error: too many arguements, try again\n");
		return 1;
	}
	else
	{
		//
		// If sample rate is given set it
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'M');
		UARTCharPut(UART1_BASE,'Y');
		UARTCharPut(UART1_BASE,' ');
		
		//
		// Send sample rate
		//
		for(x=0;y!=0 ;x++ )
			{
				//
				// Set y=hex value of ascii character
				//
				y= argv[1][x];
				
				//
				// Send character to XBee
				//
				UARTCharPut(UART1_BASE,y);
			}
		
		//
		// End of command character
		//
		UARTCharPut(UART1_BASE,'\r');
	}
	
	return 0;
}

//*****************************************************************************
//
// I/O Pin 0-7 Configuration Command
// Input: Pin#,config
// Response: ??
// Use: to set the pin config for pin 0-7
//
// Valid config commands:
//		0: Disable I/O on that pin
//		1: Built in function, if available on poin
//		2: Analog input, only pins 0-3
//		3: Digital Input
//		4: Digital Output, LOW  (0 Volts)
//		5: Digital Output, HIGH (3.3 Volts)
//
// WARNING: this command has NO error checking / user protection. Due to the
// wide variation in how this command is implemented. It is provided as a 
// direct path to the underlayer for the advanced user. Check the data sheet
// before use.
//
//*****************************************************************************
int Cmd_ATD(int argc, char *argv[])
{

	//
	// check for correct number of arguements
	//
	if(argc !=3)
		{
			UARTprintf("ERRROR: Invalid number of inputs");
			return 1;
		}
	else{
		//
		// Send Base command
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'D');
		UARTCharPut(UART1_BASE,argv[1][0]);
		UARTCharPut(UART1_BASE,' ');
		UARTCharPut(UART1_BASE,argv[2][0]);
		UARTCharPut(UART1_BASE,'\r');

		return 0;
	}
}

//*****************************************************************************
//
// I/O PWM 0 / 1 Configuration Command
// Input: Pin#, config
// Response:??
// Use: to set the pin config for pin 10-11
//
// Valid config commands:
//		0: Disable I/O on that pin
//		1: Built in function, if available o
//		2: Analog input, only pins 0-3
//
//*****************************************************************************
int Cmd_ATP(int argc, char *argv[])
{
	int x;
	char y = 'a'; //dummy value, default 0 is a check case
	
	//
	// Validate Input
	//
	if( 
			(argc != 3) |  //wrong number of commands
			(argv[2][0]!='0') | (argv[2][0]!='1') | (argv[2][0]!='2') | //invalid config
			(argv[1][0]!='0') | (argv[1][0]!='1')  //invalid pin
		)
	{
		//
		// Error check: too many arguements, 
		//
		UARTprintf("Error: invalid input, try again\n");
		return 1;
	}
	else
	{
		//
		// Send Base command
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'P');
		UARTCharPut(UART1_BASE,argv[1][0]);
		UARTCharPut(UART1_BASE,' ');
		
		//
		// Send specific command
		//
		for(x=0;y!=0 ;x++ )
			{
				//
				// Set y=hex value of ascii character
				//
				y= argv[1][x];
				
				//
				// Send character to XBee
				//
				UARTCharPut(UART1_BASE,y);
			}
		
		//
		// End of command character
		//
		UARTCharPut(UART1_BASE,'\r');
	}
	
	return 0;
}

//*****************************************************************************
//
// I/O Sample Rate Command
// Input: sample rate in milliseconds in hex
// Response: ??
// Use: to set sample rate give the rate in miliseconds in hex format. To turn
//		off sampling set rate of 0.
//
//*****************************************************************************
int Cmd_ATIR(int argc, char *argv[])
{
	int x;
	char y = 'a'; //dummy value, default 0 is a check case
	
	//
	// Process commands
	//
	if( 1 == argc )
	{
		//
		// Send basic command
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'I');
		UARTCharPut(UART1_BASE,'R');
		UARTCharPut(UART1_BASE,'\r');
		
	}
	else if( argc > 3 )
	{
		//
		// Error check: too many arguements, 
		//
		UARTprintf("Error: too many arguements, try again\n");
		return 1;
	}
	else
	{
		//
		// If sample rate is given set it
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'I');
		UARTCharPut(UART1_BASE,'R');
		UARTCharPut(UART1_BASE,' ');
		
		//
		// Send sample rate
		//
		for(x=0;y!=0 ;x++ )
			{
				//
				// Set y=hex value of ascii character
				//
				y= argv[1][x];
				
				//
				// Send character to XBee
				//
				UARTCharPut(UART1_BASE,y);
			}
		
		//
		// End of command character
		//
		UARTCharPut(UART1_BASE,'\r');
	}
	
	return 0;
}

//*****************************************************************************
//
// Iteration Tailor Command (series 1)
// Input: number of samples (in hex, largest value possible = 0x44)
// Response: ??
// Use: set number of samples to take from I/O before transmit
//
//*****************************************************************************
int Cmd_ATIT(int argc, char *argv[])
{
	int x;
	char y = 'a'; //dummy value, default 0 is a check case
	
	//
	// Validate Input
	//
	if( (argc > 2 )| (argv[1][0]>='4' & argv[1][1]>'4') )
	{
		UARTprintf("Error: invalid input, try again\n");
		
		return 1;
	}
	else
	{
		//
		// Send base command
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'I');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,' ');
		
		//
		// Send sample rate
		//
		for(x=0;y!=0 ;x++ )
			{
				//
				// Set y=hex value of ascii character
				//
				y= argv[1][x];
				
				//
				// Send character to XBee
				//
				UARTCharPut(UART1_BASE,y);
			}
		
		//
		// End of command character
		//
		UARTCharPut(UART1_BASE,'\r');
	}
	
	return 0;
}

//*****************************************************************************
//
// Input Address Command
// Input: address of XBee to accept commands from
// Response: ??
// Use: enable pin output modes to be set from another XBee
//
//*****************************************************************************
int Cmd_ATIA(int argc, char *argv[])
{
	int x;
	char y = 'a'; //dummy value, default 0 is a check case
	
	//
	// Validate Input
	//
	if( argc > 2 )
	{
		//
		// Invalid Input, notify users
		//
		UARTprintf("Error: invalid input, try again\n");
		return 1;
	}
	else
	{
		//
		// Send base command
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'I');
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,' ');
		
		//
		// Send sample rate
		//
		for(x=0;y!=0 ;x++ )
			{
				//
				// Set y=hex value of ascii character
				//
				y= argv[1][x];
				
				//
				// Send character to XBee
				//
				UARTCharPut(UART1_BASE,y);
			}
		
		//
		// End of command character
		//
		UARTCharPut(UART1_BASE,'\r');
	}
	
	return 0;
}

//*****************************************************************************
//
// Percent Voltage Command
// Input: n/a
// Response: current supply voltage to XBee, 'ERROR' if nothing connected
// Use: to keep track of battery levels
//
//*****************************************************************************
int Cmd_ATV(int argc, char *argv[])
{
	//
	// Send 'AT%V'
	//
	UARTCharPut(UART1_BASE,'A');
	UARTCharPut(UART1_BASE,'T');
	UARTCharPut(UART1_BASE,'%');
	UARTCharPut(UART1_BASE,'V');
	UARTCharPut(UART1_BASE,'\r');
	
	//
	// Assumed Success
	//
	return 0;
}

//*****************************************************************************
//
// Pull-Up Resistor Command
// Input: 1 to turn on, 0 to turn off
// Response: ??
// Use: Configure internal 30Kohm pull up resistor on all pins configured as 
//		input.
//
// NOTE: internal pullups are available on all input pins, default preset on.
//
//*****************************************************************************
int Cmd_ATPR(int argc, char *argv[])
{
	
	//
	// Validate input 
	//
	if( (argc > 2) | (argv[1][0]!='0') | (argv[1][0] != '1') )
	{
		//
		// Error check: too many arguements, 
		//
		UARTprintf("Error: invalid input, try again\n");
		
		return 1;
	}
	else if( 2 == argc )
	{
		//
		// Send the ATDH command 
		//
		UARTCharPut(UART1_BASE,'A');
		UARTCharPut(UART1_BASE,'T');
		UARTCharPut(UART1_BASE,'D');
		UARTCharPut(UART1_BASE,'H');
		UARTCharPut(UART1_BASE,' ');
		UARTCharPut(UART1_BASE,argv[1][0]);
		UARTCharPut(UART1_BASE,'\r');
		
	}
	
	return 0;
}

//*****************************************************************************
//
// Reset Command
// Input: n/a
// Response: ??
// Use: Reset config to factory presets
//
//*****************************************************************************
int Cmd_ATRE(int argc, char *argv[])
{
	//
	// Send 'ATRE'
	//
	UARTCharPut(UART1_BASE,'A');
	UARTCharPut(UART1_BASE,'T');
	UARTCharPut(UART1_BASE,'R');
	UARTCharPut(UART1_BASE,'E');
	UARTCharPut(UART1_BASE,'\r');
	
	//
	// Assumed Success
	//
	return 0;
}
