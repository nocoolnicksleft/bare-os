/***********************************************************************/
/*                                                                     */
/* SYSCALLS.C: System Calls Remapping                                  */
/* TODO: some more work has to be done on this                         */
/***********************************************************************/

#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <reent.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include <errno.h>
#undef errno
extern int errno;

char *__env[1] = { 0 };
char **environ = __env;


#include "uart.h"


 /**
  * Exit a program
  * 
  * @param rc
  */
 void _exit (int  rc)
{
  // register int  t1 asm ("r3") = rc;

  // asm volatile ("\tl.nop\t%0" : : "K" (NOP_EXIT), "r" (t1));

  while (1)
    {
    }
}     
 
/**
 * Closing a file
 * 
 * @param file
 * @return 
 */
int _close(int file) { 

    errno = EBADF;    
    return -1; 
    
}
 
/**
 * Transfer Control to a New Process
 * 
 * @param name
 * @param argv
 * @param env
 * @return 
 */
int _execve (char  *name,
         char **argv,
         char **env)
{
  errno = ENOMEM;
  return -1;                    /* Always fails */

}

/**
 * Create a new process
 * 
 * @return 
 */
int _fork ()
{
  errno = EAGAIN;
  return -1;                    /* Always fails */

}       /* _fork () */


/**
 * Provide the Status of an Open File
 * 
 * @param file
 * @param st
 * @return 
 */
int _fstat(int file, struct stat *st) {
  if ((STDOUT_FILENO == file) || (STDERR_FILENO == file))
    {
      st->st_mode = S_IFCHR;
      return  0;
    }
  else
    {
      errno = EBADF;
      return  -1;
    }
}

/**
 * Get the Current Process ID
 * 
 * @return 
 */
int _getpid ()
{
  return  1;                            /* Success */

}    

/**
 * Determine the Nature of a Stream
 * 
 * @param file
 * @return 
 */
int _isatty(int file) { 
 if ((file == STDOUT_FILENO) || (file == STDERR_FILENO))
    {
      return  1;
    }
  else
    {
      errno = EBADF;
      return  -1;
    }
}

/**
 * Send a Signal
 * 
 * @param pid
 * @param sig
 * @return 
 */
int _kill (int  pid,
           int  sig)
{
  errno = EINVAL;
  return -1;                    /* Always fails */

}      

/**
 * Rename an existing file
 * 
 * @param old
 * @param new
 * @return 
 */
int _link (char *old,
           char *new)
{
  errno = EMLINK;
  return -1;                    /* Always fails */

}     

/**
 * Set Position in a File
 * 
 * @param file
 * @param offset
 * @param whence
 * @return 
 */
int _lseek (int   file,
            int   offset,
            int   whence)
{
if ((STDOUT_FILENO == file) || (STDERR_FILENO == file))
    {
      return  0;
    }
  else
    {
      errno = EBADF;
      return  (long) -1;
    }

}   

/**
 * Open a file
 * 
 * @param name
 * @param flags
 * @param mode
 * @return 
 */
int _open (const char *name,
           int         flags,
           int         mode)
{
  errno = ENOSYS;
  return -1;                    /* Always fails */

} 
/**
 * Read from a File
 * 
 * @param file
 * @param ptr
 * @param len
 * @return 
 */
int _read(int file,
        char *buf,
        int len) {
    
    if (STDIN_FILENO == file) {
        int i;

        for (i = 0; i < len; i++) {
            buf[i] = _uart_getc();

            _uart_putc(buf[i]);

            /* Return partial buffer if we get EOL */
            if ('\n' == buf[i]) {
                return i;
            }
        }

        return i; /* Filled the buffer */
    } else {
        errno = EBADF;
        return -1;
    }

}


char *heap_end = 0;

/**
 * Allocate more Heap
 * 
 * @param incr
 * @return 
 */
caddr_t _sbrk(int incr) {
 extern char _HEAP_START_; /* Defined by the linker */
 extern char _HEAP_END_; /* Defined by the linker */
 
 static char *prev_heap_end;
 _uart_writes("_sbrk incr ");
 hexstrings(incr);
 _uart_writes(" heap ");
 hexstrings((int)&_HEAP_START_);
 _uart_writes(" - ");
 hexstrings((int)&_HEAP_END_);
 _uart_writes(" prev heap end ");
 hexstring((int)prev_heap_end);
 
 if (heap_end == 0) {
  heap_end = &_HEAP_START_;
 }
 prev_heap_end = heap_end;
 
 if (heap_end + incr > &_HEAP_END_) {
  /* Heap and stack collision */
     _uart_writes(" Failed: Heap and stack collision\r\n");
     
  return (caddr_t)0;
  
 }
 
 heap_end += incr;
 _uart_writes(" success, new heap end ");
 hexstring((int)heap_end);
 _uart_writes("\r\n");
 return (caddr_t) prev_heap_end;
 }

/**
 * Status of a File (by Name)
 * 
 * @param file
 * @param st
 * @return 
 */
int _stat (char        *file,
           struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;

}  

/**
 * Provide Process Timing Information
 * 
 * @param buf
 * @return 
 */
int _times (struct tms *buf)
{
  errno = EACCES;
  return  -1;

}    

/**
 * Remove a File's Directory Entry
 * 
 * @param name
 * @return 
 */
int _unlink (char *name)
{
  errno = ENOENT;
  return -1;                    /* Always fails */

}     

/**
 * Wait for a Child Process
 * 
 * @param status
 * @return 
 */
int
_wait (int *status)
{
  errno = ECHILD;
  return -1;                    /* Always fails */

}   


static void
outbyte (char  c)
{
  _uart_putc (c);

}       /* outbyte () */
	


/**
 * Write to a File
 * 
 * @param file
 * @param ptr
 * @param len
 * @return 
 */
int _write(int file, char *buf, int nbytes) {
    
  int i;

  /* We only handle stdout and stderr */
  if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
    {
      errno = EBADF;
      return -1;
    }

  /* Output character at at time */
  for (i = 0; i < nbytes; i++)
    {
      outbyte (buf[i]);
    }
        
  return nbytes;
  
 }

