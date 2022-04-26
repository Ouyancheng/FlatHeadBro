#include "uart.h"
#include "delay.h"

static uint32_t crc32_tab[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};
static uint32_t crc32_inc(const void *buf, unsigned size, uint32_t crc) {
    const uint8_t *p = buf;
    crc = crc ^ ~0U;
    while (size--)
        crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
    return crc ^ ~0U;
}
static uint32_t crc32(const void *buf, unsigned size) {
    return crc32_inc(buf, size, 0);
}

enum uart_boot_msgs {
    RISCV64BASE     = 0x40000000,       // where program gets linked.  we could send this.
    BOOT_START      = 0xFFFF0000,
    GET_PROG_INFO   = 0x11112222,       // board sends
    PUT_PROG_INFO   = 0x33334444,       // unix sends
    GET_CODE        = 0x55556666,       // board sends
    PUT_CODE        = 0x77778888,       // unix sends
    BOOT_SUCCESS    = 0x9999AAAA,       // board sends on success
    BOOT_ERROR      = 0xBBBBCCCC,       // board sends on failure.
    PRINT_STRING    = 0xDDDDEEEE,       // board sends to print a string.
    // error codes from the board to unix
    BAD_CODE_ADDR   = 0xdeadbeef,
    BAD_CODE_CKSUM  = 0xfeedface,
};
static uint32_t boot_get32(void) {
    uint32_t u = (uint32_t)uart_getc(uart_ctl); 
    u |= uart_getc(uart_ctl) << 8;
    u |= uart_getc(uart_ctl) << 16;
    u |= uart_getc(uart_ctl) << 24;
    return u; 
}

static void boot_put32(uint32_t u) {
    uart_putc(uart_ctl, (u >> 0)  & 0xff);
    uart_putc(uart_ctl, (u >> 8)  & 0xff);
    uart_putc(uart_ctl, (u >> 16) & 0xff);
    uart_putc(uart_ctl, (u >> 24) & 0xff);
}

static unsigned has_data_timeout(uint64_t timeout) {
    uint64_t starting_usec = get_current_time_us();
    uint64_t now_usec = 0;
    while (1) {
        now_usec = get_current_time_us(); 
        if (now_usec - starting_usec > timeout) {
            return 1;
        }
        if (uart_has_data(uart_ctl)) {
            return 0;
        }
    }
    return 1;
}


static void wait_for_data(uint64_t usec_timeout) {
    while (has_data_timeout(usec_timeout)) {
        boot_put32(GET_PROG_INFO);
    }
}

uintptr_t uart_get_code(void) {
    while (1) {
        // 0. keep sending GET_PROG_INFO every 300ms until there's data
        wait_for_data(300 * 1000);

        // 2. expect: [PUT_PROG_INFO, addr, nbytes, cksum] 
        // we echo cksum back in step 4 to help debugging.
        uint32_t put_prog_info = boot_get32();  // tag 
        uint32_t addr          = boot_get32();  // unused here 
        uint32_t nbytes        = boot_get32();  // number of bytes in the binary 
        uint32_t cksum         = boot_get32();  // crc checksum 

        if (put_prog_info != PUT_PROG_INFO) {
            boot_put32(BOOT_ERROR);
            continue; 
        }

        // // address is not 0x40000000? 
        // if (addr != (uint32_t)RISCV64BASE) {
        //     boot_put32(BAD_CODE_ADDR);
        //     continue; 
        // }

        uintptr_t code_address = 0x40000000UL; 

        // 3. If the binary will collide with us, abort. 
        //    you can assume that code must be below where the booloader code
        //    gap starts.
        // code too big that it wraps around, this should not happen anyways 
        if (code_address + nbytes < addr) {
            boot_put32(BAD_CODE_ADDR);
            continue; 
        }

        // do we want to check the heap? 
        // let's not care about it, however the program uses the heap depends on itself 


        // 4. send [GET_CODE, cksum] back.
        boot_put32(GET_CODE);
        boot_put32(cksum);

        // 5. expect: [PUT_CODE, <code>]
        //  read each sent byte and write it starting at 
        //  <addr> using put8
        uint32_t put_code = boot_get32();
        if (put_code != PUT_CODE) {
            boot_put32(BAD_CODE_ADDR); 
            continue; 
        }

        volatile char *addr_ptr = (volatile char*)(code_address); 
        for (uint32_t i = 0; i < nbytes; ++i) {
            char b = uart_getc(uart_ctl);
            (*addr_ptr) = b; 
            addr_ptr += 1;
        }

        // 6. verify the cksum of the copied code.
        uint32_t new_cksum = crc32((const void*)(code_address), nbytes);
        if (new_cksum != cksum) {
            boot_put32(BAD_CODE_CKSUM);
            continue; 
        }

        // 7. send back a BOOT_SUCCESS!
        const char msg[] = "<Yancheng Ou>: success: Received the program!";
        for (unsigned i = 0; i < sizeof(msg)-1; ++i) {
            uart_putc(uart_ctl, msg[i]); 
        }
        uart_putc(uart_ctl, '\r');
        uart_putc(uart_ctl, '\n');
        boot_put32(BOOT_SUCCESS);

        return code_address;

    }
    return 0x00000000UL;

}

