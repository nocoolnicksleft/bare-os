

#ifdef __cplusplus
extern "C" {
#endif

extern char _uart_getc ();
extern void _uart_putc ( const char c );
extern void _uart_writes( const char* text);
extern void _uart_init(void);

extern void hexstring ( unsigned int d );
extern void hexstrings ( unsigned int d );

#ifdef __cplusplus
}
#endif
