SECTIONS
{
    .smio_mod_dispatch : ALIGN(8)
    {
        _smio_mod_dispatch = .;
        KEEP(*(.smio_mod_dispatch))
        _esmio_mod_dispatch = .;
    }
}
INSERT AFTER .rodata;
