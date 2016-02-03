
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

void writeit(char* text);
bool initScreen();

vuint32 timer1signal;
vuint32 timer200signal;
vuint32 timer200countdown;
vuint32 timer20signal;
vuint32 timer20countdown;
uint32 testledon;

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

void fillRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color) {
    unsigned int cx;
    unsigned int cy;
    unsigned short int * d;
    unsigned short int lineoffset;

    if (x1 > SCREEN_WIDTH) return;
    if (y1 > SCREEN_HEIGHT) return;

    if (y2 > SCREEN_HEIGHT) y2 = SCREEN_HEIGHT;
    if (x2 > SCREEN_WIDTH) x2 = SCREEN_WIDTH;
    
    lineoffset = SCREEN_WIDTH - (x2 - x1) - 1;
    
    d = (short unsigned int *) (fb->pointer) + (((y1 -1)*SCREEN_WIDTH + x1));

    for (cy = y1; cy <= y2; cy++) {

        for (cx = x1; cx <= x2; cx++) {
            
                // d = (short unsigned int *) (fb->pointer + ((  ( (cy - 1) * SCREEN_WIDTH ) + cx) ) );
                *d = color;
                
                d++;

        }
        
        d += lineoffset;

    }

}

bool initScreen() {

    fb = InitialiseFrameBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR_DEPTH);

    if (fb) {

        SetGraphicsAddress(fb);
        return true;
    }

    return false;

}

void renderit() {

    static uint32_t lastRandom = 0;
    static uint32_t lastX = 0;
    static uint32_t lastY = 0;
    static uint32_t color = 0;
    static uint32_t x = 0;
    static uint32_t y = 0;
    static uint32_t xi = 0;
    static uint32_t yi = 0;


    x = Random(lastRandom);
    y = Random(x);
    lastRandom = y;


    SetForeColour(color);

    color = ((color + 1) << 16) >> 16;

    yi = (y >> 22);

    if (yi <= 768) {
        xi = (x >> 22);
        // DrawLine(lastX, lastY, xi, yi);
        DrawRect(lastX, lastY, xi, yi);

        lastX = xi;
        lastY = yi;
    }

}

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

//    char c;
//    uint32 sp;
    
    if (INTR_IRQ_PENDING1 & INTR_IRQ_PENDING1_AUX) {
        if (AUX_MU_IIR_REG ^ AUX_MU_IIR_INTERRUPT_PENDING) {
            if (AUX_MU_IIR_REG & AUX_MU_IIR_RECEIVED) {
                /*
        sp = GETSP();
        _uart_putc('a');
        hexstring(sp);
                 */
                
                serial_input();
                //c = AUX_MU_IO_REG;
                //uart_receive(c);
                
                //AUX_MU_IO_REG = c;
            }
        }

    }

    if (INTR_IRQ_PENDING1 & INTR_IRQ_PENDING1_SYSTEMTIMER_C3) {
        
        TIMER_CS = TIMER_CS_MATCH3;
        TIMER_C3 = (TIMER_CLO + 1000);
        timer1signal = 1;
        
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



/*************************************************/
/* Main                                          */
/*************************************************/

int main() {
    
    //bool haveScreen;
    uint32 tempcolorr;
    uint32 tempcolorg;
    uint32 tempcolorb;
    uint32 tx1;
    uint32 tx2;
    uint32 ty1;
    uint32 ty2;
//    uint32 sp;
    char tmp[200];
    int command;
   // heap_end = 0;
    serialbuffer[0] = 0;
    SetGpioFunction(16, 1);
    SetGpio(16, 1);
    setup_1ms_timer();
    
    timer1signal = 0;
    timer200signal = 0;
    timer200countdown = 0;
    timer20signal = 0;
    timer20countdown = 0;

    enable_irq();
    
    _uart_init();
    
    Bitmap * bmp;
    Font * fntvga;
    Font * fntega;
    Font * fnt6x8;
    Font * fntnori;
    Font * fntlucida;
    Font * fntcomic;
    WConsole * console;
    
    ScreenRaspi * scn;
    Rectangle * tmprect;
    
    printf("Welcome to NoOS 0.01\r\n%s\r\n > ","Repent, end of computing is nigh!");
    
    
    
    scn = new ScreenRaspi(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_COLOR_DEPTH);
    
    scn->Init();
    
    //haveScreen = initScreen();
    initScreen();
    
    fntvga = new Font(FontTypeVGA);
    fntega = new Font(FontTypeEGA);
    fnt6x8 = new Font(FontType6x8);
    fntnori = new Font(FontTypeNoritake);
    fntlucida = new Font(FontTypeLucidaConsole10);
    fntcomic = new Font(FontTypeComicSans12);
    
    // bmp = new Bitmap(SCREEN_WIDTH,SCREEN_HEIGHT,(uint16 *)fb->pointer);
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
    
    console = new WConsole(new Rectangle(600,1,423,766),54,44,fntvga);
    
    console->SetParent(bmp);
    
    scn->AddWindow(console,100);
    
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