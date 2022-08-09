#include "system.h"
#include "../include/thirdparties/stivale2.h"

//TODO CHANGE GRUBINFO STRUCT TO HAVE A STIVALE2_STRUCT CONTAINING THE TAGS CHAINEDLIST

uint64_t get_nb_entries(multibootinfo_t * src)
{
    uint64_t count = 0;
    for(uint64_t i = 0; i < src->mmap_length; i += sizeof(memblock_t))
    {
        count += 1;
    }
    return count;
}

struct stivale2_struct_tag_framebuffer get_framebuffer(multibootinfo_t * src)
{
    struct stivale2_struct_tag_framebuffer temp;

    struct stivale2_tag ttemp;
    ttemp.identifier = 0x506461d2950408fa;
    temp.tag = ttemp;

    temp.framebuffer_addr = (uint64_t) src->framebuffer_addr;
    temp.framebuffer_width = (uint16_t) src->framebuffer_width;
    temp.framebuffer_height = (uint16_t) src->framebuffer_heigth;
    temp.framebuffer_pitch = (uint16_t) src->framebuffer_pitch;
    temp.framebuffer_bpp = (uint16_t) src->framebuffer_bpp;
    temp.memory_model = (uint8_t) 1;
    temp.red_mask_size = (uint8_t) src->framebuffer_red_mask_size;
    temp.red_mask_shift = (uint8_t) src->framebuffer_red_field_position;
    temp.green_mask_size = (uint8_t) src->framebuffer_green_mask_size;
    temp.green_mask_shift = (uint8_t) src->framebuffer_green_field_position;
    temp.blue_mask_size = (uint8_t) src->framebuffer_blue_mask_size;
    temp.blue_mask_shift = (uint8_t) src->framebuffer_blue_field_position;

    return temp;
}

struct stivale2_struct_tag_memmap get_memmap(multibootinfo_t * src)
{
    struct stivale2_struct_tag_memmap temp;

    struct stivale2_tag ttemp;
    ttemp.identifier = 0x2187f79e8612de07;
    temp.tag = ttemp;

    temp.entries = get_nb_entries(src);

    //TODO : Debug.
    struct stivale2_mmap_entry tab[temp.entries];

    for(uint64_t i = 0; i < src->mmap_length; i += sizeof(memblock_t))
    {
        memblock_t* mmmt = (memblock_t *) (src->mmap_addr + i);

        sprintf((unsigned char *) "Start Addr: %x%x | Length: %x%x | Size: %x | Type: %d\n",
            mmmt->addr_high, mmmt->addr_low, mmmt->len_high, mmmt->len_low, mmmt->size, mmmt->type);
    }

    return temp;
}

struct stivale2_struct_tag_modules get_modules(multibootinfo_t * src)
{
    struct stivale2_struct_tag_modules temp;
    struct stivale2_tag ttemp;
    ttemp.identifier = 0x4b6fe466aade04ce;

    temp.tag = ttemp;
    temp.module_count = (uint64_t) src->mods_count;

    //TODO : temp.module = code a function to set an array of modules (Ask Daniel)
    return temp;
}


grubinfo convert(multibootinfo_t * src)
{
    sprintf("multiboot info struct addr : %x\n", src);
    grubinfo dst;
    //dst.rsdp = get_rsdp(multibootinfo_t * src, struct grubinfo dst);

    dst.modules = get_modules(src);

    dst.mmap_tag = get_memmap(src);

    dst.framebuffer = get_framebuffer(src);

    //dst.smp = get_smp(multibootinfo_t * src, stuct grubinfo dst);

    return dst;
}

