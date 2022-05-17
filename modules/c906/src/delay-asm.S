

.global get_arch_counter_asm 
get_arch_counter_asm: 
    csrr a0, time 
    ret 



