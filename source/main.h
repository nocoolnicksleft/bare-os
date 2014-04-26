#ifdef __cplusplus
extern "C" {
#endif

int main(void);
void fiq_handler(void);
extern void irq_handler(void);
void setup_1ms_timer(void);

extern unsigned int GETSP(void);


void renderit(void);

extern char *heap_end;

#ifdef __cplusplus
}
#endif
