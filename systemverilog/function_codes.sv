`ifndef FUNCTION_CODES_PACKAGE
`define FUNCTION_CODES_PACKAGE
`include "riscv_instr.sv"
package function_codes;
  typedef enum logic [9:0] {
    ADD  = {riscv_instr::ADD[31:25], riscv_instr::ADD[14:12]},
    AND  = {riscv_instr::AND[31:25], riscv_instr::AND[14:12]},
    OR   = {riscv_instr::OR[31:25], riscv_instr::OR[14:12]},
    XOR  = {riscv_instr::XOR[31:25], riscv_instr::XOR[14:12]},
    SLT  = {riscv_instr::SLT[31:25], riscv_instr::SLT[14:12]},
    SLTU = {riscv_instr::SLTU[31:25], riscv_instr::SLTU[14:12]},
    SLL  = {riscv_instr::SLL[31:25], riscv_instr::SLL[14:12]},
    SRL  = {riscv_instr::SRL[31:25], riscv_instr::SRL[14:12]},
    SRA  = {riscv_instr::SRA[31:25], riscv_instr::SRA[14:12]},
    SUB  = {riscv_instr::SUB[31:25], riscv_instr::SUB[14:12]}
  } op_funct7_func3_t;
  typedef enum logic [2:0] {
    ADDI = riscv_instr::ADDI[14:12],
    ANDI = riscv_instr::ANDI[14:12],
    ORI = riscv_instr::ORI[14:12],
    XORI = riscv_instr::XORI[14:12],
    SLTI = riscv_instr::SLTI[14:12],
    SLTIU = riscv_instr::SLTIU[14:12],
    SLLI = riscv_instr::SLLI[14:12],
    SRLI_SRAI = riscv_instr::SRLI[14:12]
  } op_imm_func3_t;
  typedef enum logic [2:0] {
    BEQ  = riscv_instr::BEQ[14:12],
    BNE  = riscv_instr::BNE[14:12],
    BLT  = riscv_instr::BLT[14:12],
    BLTU = riscv_instr::BLTU[14:12],
    BGE  = riscv_instr::BGE[14:12],
    BGEU = riscv_instr::BGEU[14:12]
  } branch_func3_t;
endpackage
`endif
