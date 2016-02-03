#ifndef _main_h
#define _main_h

#define ___NOOS_VERSION___ "0.1"

#ifdef __cplusplus
extern "C" {
#endif

extern int main(void);
extern void fiq_handler(void);
extern void irq_handler(void);
extern void setup_1ms_timer(void);




// void renderit(void);

extern char *heap_end;

typedef struct {
    uint32_t FirmwareRevision;
    uint32_t BoardModel;
    uint32_t BoardRevision;
    uint64_t BoardSerial;
    uint64_t MACAddress;
    uint32_t ARMMemoryAddress;
    uint32_t ARMMemorySize;
    uint32_t VCMemoryAddress;
    uint32_t VCMemorySize;    
} sysinfo_t;

#ifdef __cplusplus
}
#endif

#endif

