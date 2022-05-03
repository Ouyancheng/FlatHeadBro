#ifndef INTERRUPT_H 
#define INTERRUPT_H 
#include <stdint.h>
#include "utilities.h"
#include "csr-read-write.h"
#include "plic.h" 
#include "clint.h" 
extern unsigned direct_interrupt_trampoline; 
extern unsigned interrupt_vector;
// supervisor mode software interrupt enable 
#define MIE_SSIE (1 << 1) 
// machine mode software interrupt enable 
#define MIE_MSIE (1 << 3)
// supervisor mode timer interrupt enable 
#define MIE_STIE (1 << 5)
// machine mode timer interrupt enable 
#define MIE_MTIE (1 << 7) 
// supervisor mode external interrupt enable 
#define MIE_SEIE (1 << 9)
// machine mode external interrupt enable 
#define MIE_MEIE (1 << 11) 
// machine mode performance counter overflow interrupt enable 
#define MIE_MOIE (1 << 17)

/**
 * When MODE=Direct, all traps into machine mode cause the pc to be set to the address in the BASE field. 
 * When MODE=Vectored, all synchronous exceptions into machine mode cause the pc to be set to the address in the BASE field, 
 * whereas interrupts cause the pc to be set to the address in the BASE field plus four times the interrupt cause number. 
 * For example, a machine-mode timer interrupt (see Table 3.6) causes the pc to be set to BASE+0x1c. 
 * Setting MODE=Vectored may impose an additional alignment constraint on BASE, requiring up to 4 × XLEN-byte alignment.
 * 
 * NOTE: When vectored interrupts are enabled, 
 * interrupt cause 0, which corresponds to user-mode software interrupts, 
 * are vectored to the same location as synchronous exceptions. 
 * This ambiguity does not arise in practice, 
 * since user-mode software interrupts are either disabled or delegated to a less-privileged mode
 */
enum interrupt_handler_mode {
    /// use the same interrupt handler for all interrupts 
    INTERRUPT_HANDLER_DIRECT = 0b00, 
    /// use a vector of handlers for each interrupt, handler = base + 4 * exception code 
    INTERRUPT_HANDLER_VECTOR = 0b01, 
};
/** 
 * sets the interrupt base vector 
 * @param address the address of the interrupt handler 
 * @param mode the interrupt handling mode -- 
 *      if INTERRUPT_HANDLER_VECTOR, the address will be the address of an interrupt vector  
 *      if INTERRUPT_HANDLER_DIRECT, the address will be the address of the single universal interrupt handler 
 * @return the mtvec register value 
 */
uintptr_t set_interrupt_vector(uintptr_t address, enum interrupt_handler_mode mode); 
/**
 * enables all interrupts 
 * @return the mie register value 
 */
uintptr_t enable_all_interrupts(void); 
/** 
 * disabls all interrupts 
 * @return the mie register value  
 */
uintptr_t disable_all_interrupts(void); 

// __attribute__((interrupt("machine"))) only applies to function with no parameter 
void interrupt_handler(uintptr_t cause, uintptr_t pc, uintptr_t fault_address); 
/// the cause of the interrupt 
enum INTERRUPT_CAUSE {
    USER_SOFTWARE_INTERRUPT       = 0, 
    SUPERVISOR_SOFTWARE_INTERRUPT = 1, 
    MACHINE_SOFTWARE_INTERRUPT    = 3, 
    // ------------------------------------
    USER_TIMER_INTERRUPT          = 4, 
    SUPERVISOR_TIMER_INTERRUPT    = 5, 
    MACHINE_TIMER_INTERRUPT       = 7, 
    // ------------------------------------
    USER_EXTERNAL_INTERRUPT       = 8, 
    SUPERVISOR_EXTERNAL_INTERRUPT = 9, 
    MACHINE_EXTERNAL_INTERRUPT    = 11 
};
/// the cause of the exception 
enum EXCEPTION_CAUSE {
    INSTRUCTION_ADDRESS_MISALIGNED_EXCEPTION = 0, 
    INSTRUCTION_ACCESS_FAULT_EXCEPTION       = 1, 
    ILLEGAL_INSTRUCTION_EXCEPTION            = 2, 
    BREAKPOINT_EXCEPTION                     = 3, 
    LOAD_ADDRESS_MISALIGNED_EXCEPTION        = 4, 
    LOAD_ACCESS_FAULT_EXCEPTION              = 5, 
    STORE_AMO_ADDRESS_MISALIGNED_EXCEPTION   = 6, 
    STORE_AMO_ADDRESS_FAULT_EXCEPTION        = 7, 
    ENVIRONMENT_CALL_FROM_U_MODE_EXCEPTION   = 8, 
    ENVIRONMENT_CALL_FROM_S_MODE_EXCEPTION   = 9, 
    ENVIRONMENT_CALL_FROM_M_MODE_EXCEPTION   = 11, 
    INSTRUCTION_PAGE_FAULT_EXCEPTION         = 12, 
    LOAD_PAGE_FAULT_EXCEPTION                = 13, 
    STORE_AMO_PAGE_FAULT_EXCEPTION           = 15, 
};

#define add_imm_to_mexception_return_pc(imm) add_imm_to_csr("mepc", imm)
#define add_var_to_mexception_return_pc(val) add_var_to_csr("mepc", val)
#define add_imm_to_sexception_return_pc(imm) add_imm_to_csr("sepc", imm)
#define add_var_to_sexception_return_pc(val) add_var_to_csr("sepc", val)
#define add_imm_to_uexception_return_pc(imm) add_imm_to_csr("uepc", imm)
#define add_var_to_uexception_return_pc(val) add_var_to_csr("uepc", val)
#endif 
