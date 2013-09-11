//*****************************************************************************
//
// XBee.h - Headers for use with XBee.c
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

#ifndef __XBEE_H__
#define __XBEE_H__

//*****************************************************************************
//
// XBee AT Command function deffinitions
// used by g_sCmdTable in XBee.c
// 
// NOTE: before calling any command EnterCmdMode (+++) must be called
//
//*****************************************************************************
extern int Cmd_EnterCmdMode(int argc, char *argv[]);
extern int Cmd_AT(int argc, char *argv[]);
extern int Cmd_ATID(int argc, char *argv[]);
extern int Cmd_ATSH(int argc, char *argv[]);
extern int Cmd_ATSL(int argc, char *argv[]);
extern int Cmd_ATDH(int argc, char *argv[]);
extern int Cmd_ATDL(int argc, char *argv[]);
extern int Cmd_ATCN(int argc, char *argv[]);
extern int Cmd_ATWR(int argc, char *argv[]);
extern int Cmd_ATMY(int argc, char *argv[]);
extern int Cmd_ATD(int argc, char *argv[]);
extern int Cmd_ATP(int argc, char *argv[]);
extern int Cmd_ATIR(int argc, char *argv[]);
extern int Cmd_ATIT(int argc, char *argv[]);
extern int Cmd_ATIA(int argc, char *argv[]);
extern int Cmd_ATV(int argc, char *argv[]);
extern int Cmd_ATPR(int argc, char *argv[]);
extern int Cmd_ATRE(int argc, char *argv[]);

//*****************************************************************************
//
// Macro deffinitions to convert between high level functions and low level
// command line functions.
//
//*****************************************************************************
#define EnterCmdMode() Cmd_EnterCmdMode(0,0)
#define AT() Cmd_AT(0,0)
#define ATID(x) Cmd_ATID((int) 2,char[2][7]={{'A','T','I','D'},{TEST(x)}})
#define TEST(x) #x

//#define ATID(x) \
//									do{ \
//											
//											Cmd_ATID(0,)
//									}while(0)

#endif //__XBEE_X__
