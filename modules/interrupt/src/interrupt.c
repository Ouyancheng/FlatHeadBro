#include "interrupt.h"
#include "mcsr-ext.h"
#include "csr-read-write.h"
#include "printf.h"


uintptr_t set_interrupt_vector(uintptr_t address, enum interrupt_handler_mode mode) {
    uintptr_t base = (address & (~((uintptr_t)0b11))); 
    base |= (uintptr_t)mode & 0b11; 
    write_csr(MTVEC, base); 
    return base; 
}


uintptr_t enable_all_interrupts(void) {
    uintptr_t bitmask = MIE_SSIE | MIE_MSIE | MIE_STIE | MIE_MTIE | MIE_SEIE | MIE_MEIE | MIE_MOIE; 
    csr_read_and_set_bit(MIE, bitmask); 
    return bitmask; 
}

uintptr_t disable_all_interrupts(void) {
    uintptr_t bitmask = MIE_SSIE | MIE_MSIE | MIE_STIE | MIE_MTIE | MIE_SEIE | MIE_MEIE | MIE_MOIE; 
    csr_read_and_clear_bit(MIE, bitmask); 
    return bitmask; 
}

void interrupt_handler(uintptr_t cause, uintptr_t pc, uintptr_t fault_address) {
    printf("interrupt arises! cause=0x%x, pc=%x, fault_address=%x\n", cause, pc, fault_address); 
    return; 
}
void exception_trampoline(void) __attribute__((interrupt("machine")));
void exception_trampoline(void) {
    printf("hello interrupt!\n");
}
/*
Here's what clang generates (better than gcc): 
0000000040001d88 <exception_trampoline>:
40001d88: 13 01 01 e8  	addi	sp, sp, -384
40001d8c: 23 3c 11 16  	sd	ra, 376(sp)
40001d90: 23 38 51 16  	sd	t0, 368(sp)
40001d94: 23 34 61 16  	sd	t1, 360(sp)
40001d98: 23 30 71 16  	sd	t2, 352(sp)
40001d9c: 23 3c a1 14  	sd	a0, 344(sp)
40001da0: 23 38 b1 14  	sd	a1, 336(sp)
40001da4: 23 34 c1 14  	sd	a2, 328(sp)
40001da8: 23 30 d1 14  	sd	a3, 320(sp)
40001dac: 23 3c e1 12  	sd	a4, 312(sp)
40001db0: 23 38 f1 12  	sd	a5, 304(sp)
40001db4: 23 34 01 13  	sd	a6, 296(sp)
40001db8: 23 30 11 13  	sd	a7, 288(sp)
40001dbc: 23 3c c1 11  	sd	t3, 280(sp)
40001dc0: 23 38 d1 11  	sd	t4, 272(sp)
40001dc4: 23 34 e1 11  	sd	t5, 264(sp)
40001dc8: 23 30 f1 11  	sd	t6, 256(sp)
40001dcc: 27 3c 01 0e  	fsd	ft0, 248(sp)
40001dd0: 27 38 11 0e  	fsd	ft1, 240(sp)
40001dd4: 27 34 21 0e  	fsd	ft2, 232(sp)
40001dd8: 27 30 31 0e  	fsd	ft3, 224(sp)
40001ddc: 27 3c 41 0c  	fsd	ft4, 216(sp)
40001de0: 27 38 51 0c  	fsd	ft5, 208(sp)
40001de4: 27 34 61 0c  	fsd	ft6, 200(sp)
40001de8: 27 30 71 0c  	fsd	ft7, 192(sp)
40001dec: 27 3c a1 0a  	fsd	fa0, 184(sp)
40001df0: 27 38 b1 0a  	fsd	fa1, 176(sp)
40001df4: 27 34 c1 0a  	fsd	fa2, 168(sp)
40001df8: 27 30 d1 0a  	fsd	fa3, 160(sp)
40001dfc: 27 3c e1 08  	fsd	fa4, 152(sp)
40001e00: 27 38 f1 08  	fsd	fa5, 144(sp)
40001e04: 27 34 01 09  	fsd	fa6, 136(sp)
40001e08: 27 30 11 09  	fsd	fa7, 128(sp)
40001e0c: 27 3c c1 07  	fsd	ft8, 120(sp)
40001e10: 27 38 d1 07  	fsd	ft9, 112(sp)
40001e14: 27 34 e1 07  	fsd	ft10, 104(sp)
40001e18: 27 30 f1 07  	fsd	ft11, 96(sp)
40001e1c: 27 3c 81 04  	fsd	fs0, 88(sp)
40001e20: 27 38 91 04  	fsd	fs1, 80(sp)
40001e24: 27 34 21 05  	fsd	fs2, 72(sp)
40001e28: 27 30 31 05  	fsd	fs3, 64(sp)
40001e2c: 27 3c 41 03  	fsd	fs4, 56(sp)
40001e30: 27 38 51 03  	fsd	fs5, 48(sp)
40001e34: 27 34 61 03  	fsd	fs6, 40(sp)
40001e38: 27 30 71 03  	fsd	fs7, 32(sp)
40001e3c: 27 3c 81 01  	fsd	fs8, 24(sp)
40001e40: 27 38 91 01  	fsd	fs9, 16(sp)
40001e44: 27 34 a1 01  	fsd	fs10, 8(sp)
40001e48: 27 30 b1 01  	fsd	fs11, 0(sp)
40001e4c: 37 25 00 40  	lui	a0, 262146
40001e50: 13 05 a5 5a  	addi	a0, a0, 1450
40001e54: 97 f0 ff ff  	auipc	ra, 1048575
40001e58: e7 80 c0 eb  	jalr	-324(ra)
40001e5c: 87 3d 01 00  	fld	fs11, 0(sp)
40001e60: 07 3d 81 00  	fld	fs10, 8(sp)
40001e64: 87 3c 01 01  	fld	fs9, 16(sp)
40001e68: 07 3c 81 01  	fld	fs8, 24(sp)
40001e6c: 87 3b 01 02  	fld	fs7, 32(sp)
40001e70: 07 3b 81 02  	fld	fs6, 40(sp)
40001e74: 87 3a 01 03  	fld	fs5, 48(sp)
40001e78: 07 3a 81 03  	fld	fs4, 56(sp)
40001e7c: 87 39 01 04  	fld	fs3, 64(sp)
40001e80: 07 39 81 04  	fld	fs2, 72(sp)
40001e84: 87 34 01 05  	fld	fs1, 80(sp)
40001e88: 07 34 81 05  	fld	fs0, 88(sp)
40001e8c: 87 3f 01 06  	fld	ft11, 96(sp)
40001e90: 07 3f 81 06  	fld	ft10, 104(sp)
40001e94: 87 3e 01 07  	fld	ft9, 112(sp)
40001e98: 07 3e 81 07  	fld	ft8, 120(sp)
40001e9c: 87 38 01 08  	fld	fa7, 128(sp)
40001ea0: 07 38 81 08  	fld	fa6, 136(sp)
40001ea4: 87 37 01 09  	fld	fa5, 144(sp)
40001ea8: 07 37 81 09  	fld	fa4, 152(sp)
40001eac: 87 36 01 0a  	fld	fa3, 160(sp)
40001eb0: 07 36 81 0a  	fld	fa2, 168(sp)
40001eb4: 87 35 01 0b  	fld	fa1, 176(sp)
40001eb8: 07 35 81 0b  	fld	fa0, 184(sp)
40001ebc: 87 33 01 0c  	fld	ft7, 192(sp)
40001ec0: 07 33 81 0c  	fld	ft6, 200(sp)
40001ec4: 87 32 01 0d  	fld	ft5, 208(sp)
40001ec8: 07 32 81 0d  	fld	ft4, 216(sp)
40001ecc: 87 31 01 0e  	fld	ft3, 224(sp)
40001ed0: 07 31 81 0e  	fld	ft2, 232(sp)
40001ed4: 87 30 01 0f  	fld	ft1, 240(sp)
40001ed8: 07 30 81 0f  	fld	ft0, 248(sp)
40001edc: 83 3f 01 10  	ld	t6, 256(sp)
40001ee0: 03 3f 81 10  	ld	t5, 264(sp)
40001ee4: 83 3e 01 11  	ld	t4, 272(sp)
40001ee8: 03 3e 81 11  	ld	t3, 280(sp)
40001eec: 83 38 01 12  	ld	a7, 288(sp)
40001ef0: 03 38 81 12  	ld	a6, 296(sp)
40001ef4: 83 37 01 13  	ld	a5, 304(sp)
40001ef8: 03 37 81 13  	ld	a4, 312(sp)
40001efc: 83 36 01 14  	ld	a3, 320(sp)
40001f00: 03 36 81 14  	ld	a2, 328(sp)
40001f04: 83 35 01 15  	ld	a1, 336(sp)
40001f08: 03 35 81 15  	ld	a0, 344(sp)
40001f0c: 83 33 01 16  	ld	t2, 352(sp)
40001f10: 03 33 81 16  	ld	t1, 360(sp)
40001f14: 83 32 01 17  	ld	t0, 368(sp)
40001f18: 83 30 81 17  	ld	ra, 376(sp)
40001f1c: 13 01 01 18  	addi	sp, sp, 384
40001f20: 73 00 20 30  	mret	
*/








