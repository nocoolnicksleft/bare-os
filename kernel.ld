/******************************************************************************
*	kernel.ld by Bjoern Seip
*******************************************************************************/

MEMORY
{
    ram (wx) : ORIGIN = 0x8000, LENGTH = 0x0FFF8000 /* 256MB / 0x10000000 - 0x8000 */
                                    
}
 
SECTIONS {
	.init : {
            *(.init)
	}
	
	.text : {
            _TEXT_START_ = .;
            *(.text)
            _TEXT_END_ = .;
	}
	
	.data : {
            _DATA_START_ = .;
            *(.data)
            *(.got.plt)
            *(.fini)
            *(.ARM.extab)
            _DATA_END_ = .;
	}

        .text2 : {
            __exidx_start = .;
            *(.ARM.exidx* .gnu.linkonce.armexidx.*) 
            __exidx_end = .;
        }

	.rodata : {
            _RODATA_START_ = .;
            *(.rodata)
            _RODATA_END_ = .;
        }

        .ctors : {
            _CTORS_START_ = .;
            KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors))
            _CTORS_END_ = .;
        }

        .dtors : {
            _DTORS_START_ = .;
            KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors))
            _DTORS_END_ = .;
        }

	.bss : {
            _BSS_START_ = .;
            *(.bss)
            _BSS_END_ = .;
	}

        .stack : {
            _STACK_START_ = .;
            . = . + 0x100000; /* 1 MB Stack */
            _STACK_END_ = .;
        }

        .heap :
        {
            _HEAP_START_ = .;
            . = . + 0x4000000; /* 64 MB Heap*/
            _HEAP_END_ = .;
        }	

		
}
