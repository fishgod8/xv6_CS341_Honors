#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

struct fdt_header {
    uint32 magic;
    uint32 total_size;
    uint32 off_dt_struct;
    uint32 off_dt_strings;
    uint32 off_mem_rsvmap;
    uint32 version;
    uint32 last_comp_version;
    uint32 boot_cpuid_phys;
    uint32 size_dt_strings;
    uint32 size_dt_struct;
};

struct fdt_prop_info {
    uint32 len;
    uint32 nameoff;
};

// converts from Big to Little Endian
uint32 big_to_little(uint32 ptr) {
    return ((ptr >> 24) | ((ptr >> 8) & 0xff00) | ((ptr << 8) & 0xff0000) | (ptr << 24));
}

uint32 parse_dtb() {
    // need to parse from global variable dtb
    struct fdt_header* ptr = (struct fdt_header*) dtb;
    if (ptr->magic == big_to_little(0xd00dfeed)) { // 0xd00dfeed, but for RISC-V's little Endian format
        uint32* fdt_struct_block = (uint32*)((char*)ptr + big_to_little(ptr->off_dt_struct));
        char* fdt_strings_block = (char*)ptr + big_to_little(ptr->off_dt_strings);
        uint32* end_struct_block = (uint32*)((char*)fdt_struct_block + big_to_little(ptr->size_dt_struct));
        char* end_strings_block = fdt_strings_block + big_to_little(ptr->size_dt_strings);
        // check that we're still in bounds of struct block
        // search for FDT_BEGIN_NODE token
        // check whether associated node name is "memory"
        // if not, continue searching (can search using uint32*, as tokens are separated by 32 bit boundary)
        // if so, find FDT_PROP token and loop 
        // search for "reg" property (check by jumping to strings block, as specified by struct of prop token)
        // if "reg", read the value directly proceeding the struct (byte str of len)
    }
    return 0;
}