OUTPUT_ARCH(arm)
ENTRY(_entry)
SECTIONS
{
	. = 0x1FFF4C80;
	.text : {
		*(.text.start)
		*(.text)
	}
	.data : {
		*(.data)
	}
	.bss : {
		*(.bss COMMON)
	}
	.rodata : {
		*(.rodata)
	}
	. = ALIGN(4);
	_stack = . + 0x100;
}
