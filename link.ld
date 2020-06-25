/*
 * The stack starts at the top of memory, defined by __sp, and grows down.
 * Unfortunately this means we must hardcode the size of memory.
 * Here we choose 64K. Change this value to change the working memory size.
 */
__sp = 0x10000;

SECTIONS
{
    . =0x00000000;
    .text.init : {
        *(.text.init)
    }

    .text : {
        *(.text)
    }

    __global_pointer$ = .  + 0x800;
    .data : {
        *(.sbss)
        *(COMMON)
        *(.bss)

        *(.data*)
        *(.sdata)
	*(.srodata*)	
         *(.rodata)
        *(.got*)
	/**(._edata*)*/
	*(._end)
    	/*_edata=.;*/
	 _edata = .; PROVIDE (edata = .);
 	. = .;
    }

    /*_edata = .; PROVIDE (edata = .);*/
    _end=.;
}
