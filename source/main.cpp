
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "main.h"
#include "bcm2835.h"
#include "uart.h"
#include "gapi/bitmap.h"
#include "gapi/font.h"
#include "gapi/rectangle.h"
#include "gapi/screenraspi.h"
#include "gapi/wconsole.h"
#include "fatfs/ff.h"

void writeit(char* text);
bool initScreen();


vuint32 timer1signal;
vuint32 timer200signal;
vuint32 timer200countdown;
vuint32 timer20signal;
vuint32 timer20countdown;
uint32 testledon;

vuint32 time;

uint32 color = 222233;

fbInfo * fb = 0;

#define SERIAL_BUFFER_LENGTH 100

volatile char serialbuffer[SERIAL_BUFFER_LENGTH] = "";
vuint32 serialbufferptr = 0;
volatile bool serialreceived = 0;
bool echo = 1;

/**********************************************************/
/* Serial Input Buffer                                    */
/**********************************************************/

void serial_input() {
   char c;
   c = toupper(_uart_getc());   
   
   if (!serialreceived) {
    if (c == 13)  {
     serialbuffer[serialbufferptr] = 0;
     serialreceived = 1;
     serialbufferptr = 0;
    } else {
     if (echo) _uart_putc(c);   
     serialbuffer[serialbufferptr] = c;
     if (serialbufferptr < SERIAL_BUFFER_LENGTH) serialbufferptr++;
    }
   }
}

/*************************************************/
/* Terminal                                      */
/*************************************************/
typedef char int8;
typedef short int int16;

#define COMMAND_COUNT 7

#define COMMAND_CLR 1
#define COMMAND_REC 2
#define COMMAND_LIN 3
#define COMMAND_COL 4
#define COMMAND_INF 5
#define COMMAND_EON 6
#define COMMAND_EOF 7

const char cmdtext[COMMAND_COUNT][4] = { 
"CLR",
"REC", 
"LIN", 
"COL", 
"INF", 
"EON", 
"EOF"
}; 

const char errtext[4][16] = {
"INVALID COMMAND",
"INVALID PARAM  ",
"EXECUTION ERROR"
};

void printcmderror(int errnum)
{
  fprintf(stdout,"err %03d %s\r",errnum,errtext[errnum-1]);
}

int8 findcommand()
{
   int i;
   for (i=0;i<COMMAND_COUNT;i++)
    if ((serialbuffer[0] == cmdtext[i][0]) && (serialbuffer[1] == cmdtext[i][1]) && (serialbuffer[2] == cmdtext[i][2])) return (i+1);
   printcmderror(1);
   return 0;
}

void printcmd(int8 cmd)
{
 fprintf(stdout,"%s\r",cmdtext[cmd-1]);
}

void printcmdvalue(int8 cmd, int16 value)
{
 fprintf(stdout,"%s %u\r",cmdtext[cmd-1],value);
}

void printcmdvaluehex(int8 cmd, int16 value)
{
 fprintf(stdout,"%s %X\r",cmdtext[cmd-1],value);
}

void printcmdtext(int8 cmd, char *s)
{
 fprintf(stdout,"%s %s\r",cmdtext[cmd-1],s);
}


/*************************************************/
/* Graphics                                      */
/*************************************************/

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_COLOR_DEPTH 16

/*************************************************/
/* Timer                                         */
/*************************************************/
void setup_1ms_timer() {
    timer1signal = 0;
    timer200signal = 0;
    timer200countdown = 200;
    TIMER_C3 = (TIMER_CLO + 1000);
    TIMER_CS &= ~(TIMER_CS_MATCH1);
    INTR_ENABLE_IRQ_1 |= INTR_IRQ_PENDING1_SYSTEMTIMER_C3;
}

void __attribute__((interrupt("FIQ"))) fiq_handler() {

}                      

void __attribute__((interrupt("IRQ"))) irq_handler() {

    if (INTR_IRQ_PENDING1 & INTR_IRQ_PENDING1_AUX) {
        if (AUX_MU_IIR_REG ^ AUX_MU_IIR_INTERRUPT_PENDING) {
            if (AUX_MU_IIR_REG & AUX_MU_IIR_RECEIVED) {
                
                serial_input();
            }
        }

    }

    if (INTR_IRQ_PENDING1 & INTR_IRQ_PENDING1_SYSTEMTIMER_C3) {
        
        TIMER_CS = TIMER_CS_MATCH3;
        TIMER_C3 = (TIMER_CLO + 1000);
        timer1signal = 1;
        
        time ++;
        
        if (timer20countdown > 0) {
            timer20countdown--;
        } else {
            timer20countdown = 20;
            timer20signal = 1;
        }
        
        if (timer200countdown > 0) {
            timer200countdown--;
        } else {
            timer200countdown = 200;
            timer200signal = 1;
        }
    }

}


static
void put_rc (int rc)
{
	const char *str =
		"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
		"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
		"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
		"LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
	int i;

	for (i = 0; i != rc && *str; i++) {
		while (*str++) ;
	}
	printf("rc=%u FR_%s\n", (UINT)rc, str);
}
/*************************************************/
/* Main                                          */
/*************************************************/

int main() {
    
    uint32 tempcolorr;
    uint32 tempcolorg;
    uint32 tempcolorb;
    uint32 tx1;
    uint32 tx2;
    uint32 ty1;
    uint32 ty2;

    char tmp[200];
    int command;

    serialbuffer[0] = 0;
    SetGpioFunction(16, 1);
    SetGpio(16, 1);
    setup_1ms_timer();
    
    timer1signal = 0;
    timer200signal = 0;
    timer200countdown = 0;
    timer20signal = 0;
    timer20countdown = 0;
    
    time = 1;

    enable_irq();
    
    _uart_init();
    
    
    Font * fntvga;
    Font * fntega;
    Font * fnt6x8;
    Font * fntnori;
    Font * fntlucida;
    Font * fntcomic;
    
    
    ScreenRaspi * scn;
    WConsole * console;
    Bitmap * bmp;
    
    Rectangle * tmprect;
    
FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */

    printf("Welcome to NoOS 0.01\r\n%s\r\n > ","Repent, end of computing is nigh!");
    
    fntvga = new Font(FontTypeVGA);
    fntega = new Font(FontTypeEGA);
    fnt6x8 = new Font(FontType6x8);
    fntnori = new Font(FontTypeNoritake);
    fntlucida = new Font(FontTypeLucidaConsole10);
    fntcomic = new Font(FontTypeComicSans12);
    
    
    scn = new ScreenRaspi(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_COLOR_DEPTH);
    
    scn->Init();
    
    console = new WConsole(new Rectangle(600,1,423,766),54,44,fntvga);
    
    scn->AddWindow(console,100);
    
    bmp = scn->GetBuffer();
    
    bmp->DrawText(fntvga, new Rectangle(1, 1,400,20), "\xD5\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xB8", RGB15(255,0,0));
    bmp->DrawText(fntvga, new Rectangle(1,17,400,20), "\xB3 Welcome to NoOS 0.01 \xB3", RGB15(255,0,0));
    bmp->DrawText(fntvga, new Rectangle(1,33,400,20), "\xD4\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBE", RGB15(255,0,0));
    bmp->DrawText(fntega, new Rectangle(1,50,400,20), "\xD5\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xB8", RGB15(255,0,0));
    bmp->DrawText(fntega, new Rectangle(1,58,400,20), "\xB3 Welcome to NoOS 0.01 \xB3", RGB15(255,0,0));
    bmp->DrawText(fntega, new Rectangle(1,66,400,20), "\xD4\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBE", RGB15(255,0,0));
    bmp->DrawText(fnt6x8, new Rectangle(1,90,400,40), "Repent, end of computing is nigh!", RGB15(255,0,0));
    
    bmp->DrawText(fntnori, new Rectangle(1,120,150,40), "YES, HAVE SOME! SYMMETRIC BOOKSTACKING!", RGB15(255,0,0));
    
    bmp->DrawText(fnt6x8, new Rectangle(1,160,150,40), "YES, HAVE SOME! Symmetric Bookstacking!", RGB15(255,0,0));
    
    bmp->DrawText(fntega, new Rectangle(1,200,150,40), "YES, HAVE SOME! Symmetric Bookstacking!", RGB15(255,0,0));
    
    bmp->DrawText(fntvga, new Rectangle(1,240,150,40), "YES, HAVE SOME! Symmetric Bookstacking!", RGB15(255,0,0));
    
    bmp->DrawText(fntlucida, new Rectangle(1,290,150,40), "YES, HAVE SOME! Symmetric Bookstacking!", RGB15(255,0,0));

    bmp->DrawText(fntcomic, new Rectangle(1,340,150,40), "YES, HAVE SOME! Symmetric Bookstacking!", RGB15(255,0,0));
    
    f_mount(&FatFs, "0", 1);		/* Give a work area to the default drive */

    uint32 nread;
    char buf[1024];
    
    uint32 pinkysize = 0;
    uint16 * pinkybuffer = 0;
    
    if (f_open(&Fil, "PINKY.BIN", FA_READ) == FR_OK) {	
       
        pinkysize = f_size(&Fil);
        printf("Pinky is %d bytes long\n",pinkysize);

        
        pinkybuffer = (uint16 *)malloc(pinkysize);
        if (pinkybuffer != NULL) {
         printf("Memory allocated\n");    
         f_read(&Fil, pinkybuffer, pinkysize, &nread);
        }
  
        /*
        while (nread > 0) {
            fwrite(buf, 1, nread, stdout);
            f_read(&Fil, buf, sizeof buf, &nread);
        }
         */
    
        f_close(&Fil);								/* Close the file */
        

    }
    
    Bitmap * pinky = new Bitmap(pinkybuffer,true);
    
    bmp->DrawBitmap(pinky,200,500);
    
    //bmp->LoadCompressedFile((uint16 *)pinkybuffer, 96, 200, 400);
    
    int res,s1,s2,p1;
    DIR Dir;
    FILINFO Finfo;
    char Lfname[512];
    
        res = f_opendir(&Dir, "");
        if (res) { put_rc(res); }
        p1 = s1 = s2 = 0;
        for(;;) {
                res = f_readdir(&Dir, &Finfo);
                if ((res != FR_OK) || !Finfo.fname[0]) break;
                if (Finfo.fattrib & AM_DIR) {
                        s2++;
                } else {
                        s1++; p1 += Finfo.fsize;
                }
                printf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %-12s  %s\n",
                                (Finfo.fattrib & AM_DIR) ? 'D' : '-',
                                (Finfo.fattrib & AM_RDO) ? 'R' : '-',
                                (Finfo.fattrib & AM_HID) ? 'H' : '-',
                                (Finfo.fattrib & AM_SYS) ? 'S' : '-',
                                (Finfo.fattrib & AM_ARC) ? 'A' : '-',
                                (Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
                                (Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63,
                                Finfo.fsize, Finfo.fname,
#if _USE_LFN
                                Lfname);
#else
                                "");
#endif
        }
       
    
    while (true) {
        
        if (timer200signal) {
            timer200signal = 0;
            
            if (testledon) {
                //SetGpio(16, 1);
                GPSET0 = (1 << 16);
                testledon = 0;
            } else {
                //SetGpio(16, 0);
                GPCLR0 = (1 << 16);
                testledon = 1;
            }
        }
        
        if (timer20signal) {
            timer20signal = 0;
            scn->Prepare();
            scn->Display();
        }
            
        if (serialreceived) {
            serialreceived = 0;
            _uart_writes("\r\n");
           
            command = findcommand();

            switch (command) {

              case COMMAND_CLR:
                  _uart_writes("Clearing screen\r\n");
                  console->Print("Clearing screen\n");
                  
                  break;
                    
              case COMMAND_LIN:
                  _uart_writes("Drawing line\r\n");
                  console->Print("Drawing line\n");
                  break;
                  
              case COMMAND_REC:
                  if (sscanf((const char *)serialbuffer,"REC %d %d %d %d",&tx1,&ty1,&tx2,&ty2) == 4) {
                        // fillRectangle(tx1,ty1,tx2,ty2,color);
                        sprintf(tmp,"Drawing rectangle %d,%d to %d,%d\r\n", tx1, ty1, tx2, ty2);
                        bmp->FillRectangle(tx1,ty1,tx2,ty2,color);
                        _uart_writes(tmp);
                        console->Print(tmp);
                        tmprect = new Rectangle(tx1,ty1,tx2-tx1,ty2-ty1);
                        bmp->DrawText(fnt6x8, tmprect, tmp, RGB15(255,255,0));
                        delete tmprect;
                        _uart_writes("e");
                  }
                 // if (haveScreen) renderit();
               break;

              case COMMAND_COL:
                 if (serialbuffer[3] == 0) printcmderror(2);
                 else {
                  if (sscanf((const char *)serialbuffer,"COL %d %d %d",&tempcolorr,&tempcolorg,&tempcolorb) == 3) {
                        
                        color = RGB15(tempcolorr,tempcolorg,tempcolorb);
                        sprintf(tmp,"Set color to %d\r\n", color);
                        console->Print(tmp);
                        _uart_writes(tmp);
                  }
                  
                 }
                 break;
                 
              default:
                   _uart_writes("Unknown command\r\n");
                   console->Print("Unknown command\n");
                  break;
                 
            }
            
            _uart_writes(">");
     
            
        }
 
      //  delay(2000);
        // if (haveScreen) renderit();

    }

    return 0;
}