SECTIONS {
  . = 0x0;

  .text : {
    *(.text)
  }

  .rodata : {
    *(.rodata)
  }
}
