INCLUDE Generated_Source\PSoC4\cm0gcc.ld

SECTIONS 
{
    .data.ho : {
        KEEP (*(SORT_BY_NAME(.data.ho.*)))
    } >ram AT>rom

    .text.ho :{
        KEEP (*(SORT_BY_NAME(.text.ho.*)))
    } >rom
}

/* HINOKIORANGE ------------------------------------------------------------- */
EXTERN(HO_LINKER_META) 

/* COMPONENTS --------------------------------------------------------------- */
EXTERN(HO_LINKER_CYPC_DEBUG)
EXTERN(HO_LINKER_CYPC_TICK)
EXTERN(HO_LINKER_CYPC_BLE)
EXTERN(HO_LINKER_CYPC_META)
