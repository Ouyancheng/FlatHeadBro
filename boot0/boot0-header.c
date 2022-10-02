#include "boot0-header.h"
#include "boot0.h"

#define ROUND_UP_TO_8(x) (((x) + 7) / 8 * 8)

/** encoding of the jump instruction to skip over the header */
#define BROM_FILE_HEAD_SIZE         (ROUND_UP_TO_8(sizeof(struct boot0_header)) & 0x00FFFFF)  /* for some reasons, the entry point is aligned to 8 bytes */
#define BROM_FILE_HEAD_BIT_10_1     ((BROM_FILE_HEAD_SIZE & 0x7FE) >> 1)
#define BROM_FILE_HEAD_BIT_11       ((BROM_FILE_HEAD_SIZE & 0x800) >> 11)
#define BROM_FILE_HEAD_BIT_19_12    ((BROM_FILE_HEAD_SIZE & 0xFF000) >> 12)
#define BROM_FILE_HEAD_BIT_20       ((BROM_FILE_HEAD_SIZE & 0x100000) >> 20)
#define BROM_FILE_HEAD_SIZE_OFFSET \
    ((BROM_FILE_HEAD_BIT_20 << 31) | \
    (BROM_FILE_HEAD_BIT_10_1 << 21) | \
    (BROM_FILE_HEAD_BIT_11 << 20) | \
    (BROM_FILE_HEAD_BIT_19_12 << 12))
#define JUMP_INSTRUCTION        (BROM_FILE_HEAD_SIZE_OFFSET | 0x6f)

const struct boot0_header __attribute__((section (".boot0header.header"))) BT0_header = {
    {
        JUMP_INSTRUCTION,
        BOOT0_MAGIC, 
        STAMP_VALUE,
        0x4000,  // 16K 
        sizeof(struct boot0_public_header), 
    },
    {
        sizeof(struct boot0_private_header),
        {
            0x00000318,
            0x00000003,
            0x007b7bfb,
            0x00000001,
            0x000010d2,
            0x00000000,
            0x00001c70,
            0x00000042,
            0x00000018,
            0x00000000,
            0x004a2195,
            0x02423190,
            0x0008b061,
            0xb4787896,
            0x00000000,
            0x48484848,
            0x00000048,
            0x1620121e,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00870000,
            0x00000024,
            0x34050100,
        }
    }
};


